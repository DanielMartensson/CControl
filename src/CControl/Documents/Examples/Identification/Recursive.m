%% Example made by Daniel Mårtensson - 2019-10-08
%% To run this example. You need to install Matavecontrol and Mataveid

% Real life model of a water tank
G = tf([1], [1 2 1]); 

% Time
t = linspace(0, 10, 100);

% Input signals
u1 = linspace(1, 1, 50);
u2 = linspace(2, 2, 50);
u = [u1 u2];

% Simulation of the model - It will give you a graphical plot what type of data you need
y = lsim(G, u, t);

% Put some noise to output
%y = y + 0.03*randn(1, length(y));

% Identification with recursive least squares and model order 5 and delay 0 and lambda 1
[Gd, Hd, sys, K] = rls(u, y, 5, 5, 5, t(2)-t(1), 0, 1);

% Transfer function to state space - With no integration
sysd = tf2ss(Gd);
disp('INTEGRATION FALSE')
sysd

% Implement integration
sysdi.A = [sysd.A zeros(size(sysd.A, 2), 1); sysd.C*sysd.A 1];
sysdi.B = [sysd.B; sysd.C*sysd.B];
sysdi.C = [zeros(1, size(Gd.num, 2)) 1];
disp('INTEGRATION TRUE')
sysdi

disp('Kalman K')
K

% Create input u, output y and state vector x
u = [2.2];

y = [5.134];
 
x = [2.32;
		 4.12;
		 1.34;
	   2.11;
	   5.21];

disp('Our current state vector is:')
x

% Estimate our future state at x(k+1)
x = sysd.A*x - K*sysd.C*x + sysd.B*u + K*y;
disp('Our estimated state vector is:')
x
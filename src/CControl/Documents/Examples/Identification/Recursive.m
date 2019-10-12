%% Example made by Daniel Mårtensson - 2019-10-08
%% To run this example. You need to install Matavecontrol and Mataveid

% Real life model of a water tank
G = tf([1], [1 2 1]); 

% Input signals
u1 = linspace(1, 1, 50);
u2 = linspace(2, 2, 50);
u = [u1 u2];

% Simulation of the model - It will give you a graphical plot what type of data you need
y = lsim(G, u, t);

% Put some noise to output
y = y + 0.03*randn(1, length(y));

% Identification with recursive least squares and model order 5 and delay 0 and lambda 1
[Gd, Hd, sys, K] = rls(u, y, 5, 5, 5, t(2)-t(1), 0, 1);

% Transfer function to state space - With no integration
sysd = tf2ss(Gd);
disp('INTEGRATION FALSE')
sysd

% Implement integration
sysd.A = [sysd.A zeros(size(sysd.A, 2), 1); sysd.C*sysd.A 1];
sysd.B = [sysd.B; sysd.C*sysd.B];
sysd.C = [zeros(1, size(Gd.num, 2)) 1];
disp('INTEGRATION TRUE')
sysd

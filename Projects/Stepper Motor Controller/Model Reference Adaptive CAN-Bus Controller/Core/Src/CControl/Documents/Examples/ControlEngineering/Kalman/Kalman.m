%% Example made by Daniel Mårtensson - 2019-10-08
%% To run this example. You need to install Matavecontrol

% Real life model
sys = ss(0, [0 1; -2.1 -1.3], [0 0; 2.1 0.1], [1.2 0; 0 0.3]);

% Turn it to discrete with sample time h = 0.5
sysd = c2d(sys, 0.5);

% Kompute the kalman gain matrix with Q = 2*eye(2), R = 1.3*[1 2; 1 5]
Q = 2*eye(2);
R = 1.3*[1 2; 1 5];
K = lqe(sysd, Q, R);

% Create input u, output y and state vector x
u = [2.2;
		 4.3];

y = [5.134;
		 0.131];
 
x = [2.32;
		 4.12];

disp('Our current state vector is:')
x

% Estimate our future state at x(k+1)
x = sysd.A*x - K*sysd.C*x + sysd.B*u + K*y;
disp('Our estimated state vector is:')
x

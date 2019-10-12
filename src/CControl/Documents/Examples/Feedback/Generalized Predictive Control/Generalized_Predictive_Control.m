%% Example made by Daniel Mårtensson - 2019-10-08
%% To run this example. You need to install Matavecontrol

function Generalized_Predictive_Control()
  % Model
  G = tf(4.1, [2 4.2 4]);

  % Input signal
  % Only positive u because our PWM onto microcontroller does not have negative input
  t = linspace(0, 20, 200);
  u = 3*sin(t);
  u(u < 0) = 0;
  
  % Simulation
  y = lsim(G, u, t);
  
  % Add noise to output if you want
  y = y + 0.05*randn(1, length(y));
  
  % Estimate a transfer function 
  [Gd, Hd, sys, K] = rls(u, y, 6, 6, 6, t(2)-t(1), 0, 1);
  
  % Convert Gd to a state space model
  sysd = tf2ss(Gd);

  % Implement integral action
  sysdIntegral.A = [sysd.A zeros(size(sysd.A, 2), 1); sysd.C*sysd.A 1];
  sysdIntegral.B = [sysd.B; sysd.C*sysd.B];
  sysdIntegral.C = [zeros(1, size(Gd.num, 2)) 1]

  % We select N = 50 horizon 
  PHI = phiMat(sysdIntegral.A, sysdIntegral.C, 50);
  GAMMA = gammaMat(sysdIntegral.A, sysdIntegral.B, sysdIntegral.C, 50);
  
  % Compute our best inputs with ALPHA = 0.1 and r = 12.5
  ALPHA = 0.1; % Prevent dead-beat control 
  r = 12.5; % Reference
  
  % Initial state vector. Estimate this with a kalman filter
  x = [zeros(length(sysd.A), 1)]; 
  xy = y(end); % This is the state for the output. Because we have an augmented sysd model
  x0 = [x; 
        xy];
  
  % The CControl uses Gaussian Elimination, not inverse - Will result the same
  U = inv(GAMMA'*GAMMA + ALPHA*eye(size(GAMMA)))*GAMMA'*(r-PHI*x0); 
  
  % Goal: We select the first value that we are going to use as input value
  disp('Our U(1) value')
  U(1)
  
  % Goal: Update our state vector
  disp('Current state vector x')
  x
  x = sysd.A*x - K*sysd.C*x + sysd.B*U(1) + K*y(end);
  disp('Our estimated state vector is:')
  x
  xy
  
end

function PHI = phiMat(A, C, N)

  % Create the special Observabillity matrix
  PHI = [];
  for i = 1:N
    PHI = vertcat(PHI, C*A^i);
  end
  
end

function GAMMA = gammaMat(A, B, C, N)
  
  % Create the lower triangular toeplitz matrix
  GAMMA = [];
  for i = 1:N
    GAMMA = horzcat(GAMMA, vertcat(zeros((i-1)*size(C*A*B, 1), size(C*A*B, 2)),cabMat(A, B, C, N-i+1)));
  end
  
end

function CAB = cabMat(A, B, C, N)
  
  % Create the column for the GAMMA matrix
  CAB = [];
  for i = 0:N-1
    CAB = vertcat(CAB, C*A^i*B);
  end
  
end

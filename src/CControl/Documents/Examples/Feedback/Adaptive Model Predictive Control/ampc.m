
function [retval] = ampc()
  
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
  %y = y + 0.05*randn(1, length(y));
  
  % Estimate a transfer function 
  [Gd, Hd, sys, K] = rls(u, y, 6, 6, 6, t(2)-t(1), 0, 1);
  
  % Convert Gd to a state space model
  sysd = tf2ss(Gd);

  % Implement integral action
  sysdIntegral.A = [sysd.A zeros(size(sysd.A, 2), 1); sysd.C*sysd.A 1];
  sysdIntegral.B = [sysd.B; sysd.C*sysd.B];
  sysdIntegral.C = [zeros(1, size(Gd.num, 2)) 1];
  sysdIntegral.D = [0];
  sysdIntegral.sampleTime = sysd.sampleTime;
  sysdIntegral.delay = 0;
  sysdIntegral.type = 'SS';
  
  % Set alpha for prevent dead beat control = stiff input signals 
  alpha = 1.0;
  % Reference
  r = 12.5;
  % Set the horizon
  horizon = 10;
  
  % Compute the PHI matrix now!
  PHI = phiMat(sysdIntegral.A, sysdIntegral.C, horizon);
  % Compute the GAMMA matrix now
  GAMMA = gammaMat(sysdIntegral.A, sysdIntegral.B, sysdIntegral.C, horizon);
  % Compute the tuning matrix - Set it to identity matrix
  Q = alpha*eye(size(GAMMA, 1), size(GAMMA, 1));
  % Compute H matrix
  H = GAMMA'*Q*GAMMA;
  
  % Create the matrix for saturation on inputs
  Ay = GAMMA;
  
  % Initial state vector. Estimate this with a kalman filter
  x = [zeros(length(sysd.A), 1)]; 
  xy = y(end); % This is the state for the output. Because we have an augmented sysd model
  x0 = [x; 
        xy];
  
  % Comput best inputs - Try a computation
  g = GAMMA'*(Q*PHI*x0 - Q*repmat(r, horizon, 1));
  ylb = repmat(0, horizon, 1) - PHI*x0;
  yub = repmat(20, horizon, 1) - PHI*x0;
  ulb = repmat(0, horizon, 1);
  uub = repmat(20, horizon, 1);
  u = qp([], H, g, [], [], ulb, uub, ylb, Ay, yub)  % Replace qp with quadprog if you are using MATLAB
    
  % Simulation
  t = 0:sysdIntegral.sampleTime:length(u)/10;
  sysdIntegral
  lsim(sysdIntegral, u', t);
    
  
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



function [retval] = ampc()
  
  % Set the horizon
  horizon = 10;
  
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
  sysdIntegral.C = [zeros(1, size(Gd.num, 2)) 1]

  % We select N = 50 horizon 
  PHI = phiMat(sysdIntegral.A, sysdIntegral.C, 50);
  GAMMA = gammaMat(sysdIntegral.A, sysdIntegral.B, sysdIntegral.C, 50);
  
  % Set alpha for prevent dead beat control = stiff input signals 
  alpha = 1.0;
  
  % Compute the PHI matrix now!
  PHI = phiMat(sysdIntegral.A, sysdIntegral.C, horizon);
  % Compute the GAMMA matrix now
  GAMMA = gammaMat(sysdIntegral.A, sysdIntegral.B, sysdIntegral.C, horizon);
  % Compute the tuning matrix - Set it to identity matrix
  Q = alpha*eye(size(GAMMA, 1), size(GAMMA, 1));
  % Compute H matrix
  H = GAMMA'*Q*GAMMA;
  
  % Create g vector. g = g0*x - g1*r where x is the state vector and r is the reference vector
  g0 = GAMMA'*Q*PHI;
  g1 = GAMMA'*Q;
  % Create the matrix for saturation on inputs
  Ay = GAMMA;

  % Comput best inputs - Try a computation
  gg = GAMMA'(Q*PHI*x - GAMMA'*Q*r1*repmat(r, horizon, 1));
  ylb = repmat(y_lb, horizon, 1) - PHI*x0;
  yub = repmat(y_ub, horizon, 1) - PHI*x0;
  ulb = repmat(u_lb, horizon, 1);
  uub = repmat(u_ub, horizon, 1);
  u = qp([], H, g, [], [], ulb, uub, ylb, Ay, yub)  % Replace qp with quadprog if you are using MATLAB
    
  
    
  
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


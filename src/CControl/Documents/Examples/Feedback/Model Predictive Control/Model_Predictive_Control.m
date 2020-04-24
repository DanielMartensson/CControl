function [u] = Model_Predictive_Control()
  
  % Model
  sys = ss(0, [0 1; -1 -1], [0; 2], [1 0]);
  sysd = c2d(sys, 0.5);
  
  % Initial state
  x0 = [4;2];
  
  % Alpha - Prevent dead beat control
  alpha = 0.1;
  
  % Reference and horizon
  r = 12.5;
  horizon = 200;
  
  % Compute the PHI matrix now!
  PHI = phiMat(sysd.A, sysd.C, horizon);
  % Compute the GAMMA matrix now
  GAMMA = gammaMat(sysd.A, sysd.B, sysd.C, horizon);
  
  % Use linprog in MATLAB
  Y = repmat(r, horizon, 1) - PHI*x0;
  A = GAMMA'*GAMMA +alpha*eye(size(GAMMA));
  c = A'*Y;
  b = GAMMA'*Y;
  u = glpk(c, A, b, repmat(0, 1, horizon), [], repmat("U", 1, horizon), repmat("C", 1, horizon), -1);
  
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


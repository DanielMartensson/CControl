function [u] = Model_Predictive_Control()
  
  % Model
  sys = ss(0, [0 1; -1 -1], [0; 2], [1 0]);
  sysd = c2d(sys, 0.5);
  
  % Initial state
  x0 = [4;2];
  
  % Alpha - Only used for linear programming - See commented code in lmpc.m file
  regularization = 0;
  
  % Reference and horizon
  r = linspace(12.5, 12.5, 100);
  N = 20; % Horizon
  t = linspace(0, 20, 100);
  
  % Do MPC
  [y, T, X, U] = lmpc(sysd, N, r, t, regularization, x0);
  
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


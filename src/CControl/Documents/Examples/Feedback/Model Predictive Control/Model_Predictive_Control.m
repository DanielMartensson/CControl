% Call this function with Model_Predictive_Control(A, B, C, Horizon, r, x0)
% Where A, B, C are system matricies and alpha is tuning factor so you don't
% Use lmpc.m from Matavecontrol if you want to try a time based simulation before 

function [u] = Model_Predictive_Control(varargin)
  if(length(varargin) < 1)
    error('Missing A-matrix and B-matrix')
  else
    A = varargin{1};
    % Check if sys.A is square matrix
    if(size(A,1) ~= size(A,2))
      error('A is not a square');
    end
  end
  
  if(length(varargin) < 2)
    error('Missing B-matrix')
  else
    B = varargin{2};
    % Check if sys.A and sys.B have the same lenght of rows
    if(size(A, 1) ~= size(B, 1))
      error('A and B have not the same row length');
    end
  end
  
  if(length(varargin) < 3)
    sprintf('C matrix assumed to be a diagonal %ix%i matrix', size(varargin{1}, 1),size(varargin{1}, 2))
    C = eye(size(varargin{1}, 1));
  else
    C = varargin{3};
    % Check if sys.A and sys.C have the same lenght of columns 
    if(size(A, 2) ~= size(C, 2))
      error('A and C have not the same column length');
    end
  end
  
  if(length(varargin) < 4)
    sprintf('Horizon assumend to be 10');
    horizon = 10;
  else
    horizon = varargin{4};
  end
  
  % Get the reference vector R
  if(length(varargin) >= 5)
    r = varargin{5};
  else
    error('Missing the reference vector R');
  end
  
  % Get the initial state vector x0
  if(length(varargin) >= 6)
    x0 = varargin{6};
  else
    x0 = zeros(size(A, 1), 1);
  end
  
  % Compute the PHI matrix now!
  PHI = phiMat(A, C, horizon);
  % Compute the GAMMA matrix now
  GAMMA = gammaMat(A, B, C, horizon);
  
  Y = repmat(r, horizon, 1) - PHI*x0;
  % Use linprog in MATLAB
  A = GAMMA'*GAMMA + 0.1*eye(size(GAMMA));
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


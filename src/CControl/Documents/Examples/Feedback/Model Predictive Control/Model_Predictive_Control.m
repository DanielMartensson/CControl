%% Call this function with Model_Predictive_Control(A, B, C, Horizon, alpha, r, ulb, uub, ylb, yub, x0)
%% Where A, B, C are system matricies and alpha is tuning factor so you don't
%% control MPC with dead-beat signals = very jumpy. alpha Should be lower than 1.0
%% Use lmpc.m from Matavecontrol if you want to try a time based simulation before 

function [H, Ay, g0, g1, ylb, yub, ulb, uub] = Model_Predictive_Control(varargin)
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
  
  if(length(varargin) < 5)
    sprintf('alpha assumend to be 1');
    alpha = 1.0;
  else
    alpha = varargin{5};
  end
  
  % Get the reference vector R
  if(length(varargin) >= 6)
    r = varargin{6};
  else
    error('Missing the reference vector R');
  end
  
  % Get the lower bound vector u_lb 
  if(length(varargin) >= 7)
    u_lb = varargin{7};
  else
    error('Need lower bound vector u_lb')
  end
    
  % Get the upper bound vector u_ub 
  if(length(varargin) >= 8)
    u_ub = varargin{8};
  else
    error('Need lower bound vector u_ub')
  end
    
  % Get the minimum output signal limit
  if(length(varargin) >= 9)
    y_lb = varargin{9};
  else
    error('Need minimum input signal limit')
  end
    
  % Get the maximum output signal limit
  if(length(varargin) >= 10)
    y_ub = varargin{10};
  else
    error('Need maximum input signal limit')
  end
    
  % Get the initial trajectory vector x
  if(length(varargin) >= 11)
    x0 = varargin{11};
  else
    x0 = zeros(size(A, 1), 1);
  end
  
  % Compute the PHI matrix now!
  PHI = phiMat(A, C, horizon);
  % Compute the GAMMA matrix now
  GAMMA = gammaMat(A, B, C, horizon);
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
  g = g0*x0 - g1*repmat(r, horizon, 1)
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


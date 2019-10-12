

function Autotuning_Predictive_Control()
  % Model
  G = tf(4.1, [2 4.2 4]);

  % Input signal
  % Only positive u because our PWM onto microcontroller does not have negative input
  t = linspace(0, 20, 200);
  u = 3*sin(t);
  u(u < 0) = 0;
  
  % Simulation
  y = lsim(G, u, t);

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
  
  % Get our control law L
  ALPHA = 0.1;
  G = inv(GAMMA'*GAMMA + ALPHA*eye(size(GAMMA)))*GAMMA';
  L = (G*PHI)(1:size(sysdIntegral.B,2), 1:size(sysdIntegral.A,1));
  Lx = L(1:end-1)
  Li = L(end)
  
  % Get our reference gain Kr
  Kr = (G*repmat(1, 50, 1))(1:size(sysdIntegral.B,2), 1)

  
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
%% Enter your discrete state space model as argument
%% You need to have Matavecontrol installed

function [retval] = Linear_Quadratic_Integral_Control()
  % This file will create the following matrices
  % * A matrix
  % * B matrix
  % * C matrix
  % * D matrix
  % * K matrix
  % * L matrix
  % * Li matrix

  % The file will also create the following constans
  % * ADIM
  % * RDIM
  % * YDIM

  % NOTICE THAT rdim = ydim. Always!

  % Create model
  sys = ss(0, [0 1; -1 -1], [0; 2], [1 0]);
  sysd = c2d(sys, 0.5)
  
  % Our reference, state vector, state integral vector and integral speed, output
  r = 25;
  x = [0;0];
  xi = [0];
  qi = 0.1;
  y = 0;

  % Write your discrete state space model here:
  model = sysd;

  % Enter the Q matrix and R matrix for kalman steady state matrix 
  Q = eye(size(model.A));
  R = 1;

  % Change the matrices for the LQI and LQR matrix
  xn = size(model.C, 1);
  Qc = eye(size(model.A) + size(model.B, 2));
  Rc = 1;

  % Questions are done. Let's start the computing. 
  % Create the Kalman steady state matrix
  K = lqe(model, Q, R)

  % Create the Kalman steady state matrix
  [L, Li] = lqi(model, Qc, Rc)

  % Write all to a file named results.txt
  fileID = fopen('results.txt','w');
  WriteMatrix(fileID, 'float A[ADIM * ADIM] = {\n', model.A);
  WriteMatrix(fileID, 'float B[ADIM * RDIM] = {\n', model.B);
  WriteMatrix(fileID, 'float C[YDIM * ADIM] = {\n', model.C);
  WriteMatrix(fileID, 'float D[YDIM * RDIM] = {\n', model.D);
  WriteMatrix(fileID, 'float K[ADIM * YDIM] = {\n', K);
  WriteMatrix(fileID, 'float L[RDIM * ADIM] = {\n', L);
  WriteMatrix(fileID, 'float Li[RDIM] = {\n', Li);
  fclose(fileID);
  
  % Done! Copy all the matrices from the results.txt to your C-file
  
  % Do a simulation with LQI
  xi(1) = xi(1) + r - y; % Integration
  u = Li/(1-qi)*r - (L*x - Li*xi)
  
end

% This function write a matrix as an C-array
function WriteMatrix(fileID, topic, matrix)
  fprintf(fileID,topic);
  for i = 1:size(matrix, 1)
    if(i < size(matrix, 1))
      text = sprintf('%f,' , matrix(i, :));
      fprintf(fileID,'%s\n', text);
    else
      text = sprintf('%f,' , matrix(i, :));
      fprintf(fileID,'%s};\n\n', text(1, 1:length(text) - 1));
    end
  end
end

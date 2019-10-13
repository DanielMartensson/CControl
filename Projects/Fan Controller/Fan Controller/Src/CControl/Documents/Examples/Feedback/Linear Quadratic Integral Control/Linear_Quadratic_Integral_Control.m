%% Enter your discrete state space model as argument
%% You need to have Matavecontrol installed

function [retval] = Linear_Quadratic_Integral_Control(varargin)
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

  % The purpose of this file is that you only need to paste the matrices into the C-file of the STM32 embedded project
  % We start with some questions:

  % Write your discrete state space model here:
  model = varargin{1};

  % Enter the Q matrix and R matrix for kalman steady state matrix 
  Q = eye(size(model.A));
  R = 1;

  % Change the matrices for the LQI and LQR matrix
  xn = size(model.C, 1);
  Qc = eye(size(model.A) + size(model.B, 2));
  Rc = 1;

  % Questions are done. Let's start the computing. 
  % Create the Kalman steady state matrix
  K = lqe(model, Q, R);

  % Create the Kalman steady state matrix
  [L, Li] = lqi(model, Qc, Rc);

  % Write all to a file named results.txt
  fileID = fopen('results.txt','w');
  WriteMatrix(fileID, 'const float A[ADIM * ADIM] = {\n', model.A);
  WriteMatrix(fileID, 'const float B[ADIM * RDIM] = {\n', model.B);
  WriteMatrix(fileID, 'const float C[YDIM * ADIM] = {\n', model.C);
  WriteMatrix(fileID, 'const float D[YDIM * RDIM] = {\n', model.D);
  WriteMatrix(fileID, 'const float K[ADIM * YDIM] = {\n', K);
  WriteMatrix(fileID, 'const float L[RDIM * ADIM] = {\n', L);
  WriteMatrix(fileID, 'const float Li[RDIM * 1] = {\n', Li);
  fclose(fileID);

  % Done! Copy all the matrices from the results.txt to your C-file
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

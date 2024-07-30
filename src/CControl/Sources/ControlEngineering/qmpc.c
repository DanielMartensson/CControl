/*
 * qmpc.c
 *
 *  Created on: 28:e Juli mars 2024
 *      Author: Daniel Mårtensson
 */

#include "controlengineering.h"


/*
 * Model predictive control with quadratic programming and constraints on outputs and inputs
 * Hint: Look up qmpc.m in Matavecontrol
 * 
 * GAMMA[(N*row_c)*(N*column_b)]
 * PHI[(N*row_c)*row_a]
 * x[row_a]
 * u[column_b]
 * Umax[column_b]
 * S[row_c]
 * r[row_c]
 */
void qmpc(const float GAMMA[], const float PHI[], const float x[], float u[], const float Umax[], const float S[], const float r[], const size_t row_a, const size_t row_c, const size_t column_b, const size_t N, const float lambda, const bool has_integration_action, const float integration_constant) {
	/*
	% Solve: R = PHI*x + GAMMA*U with quadratic programming: Min: 1/2x^TQx + c^Tx, S.t: Ax <= b, x >= 0
    % Q = a*eye(size(GAMMA))
    % cqp = GAMMA'*Q*(PHI*x - R)
    % bqp = [S + R - PHI*x; Umax; Umax*0]
    % aqp = [GAMMA; I; -I]
    % qqp = GAMMA'*GAMMA + Q
	*/
	
	/* Create R vector */
	float* R = (float*)malloc(N * row_c * sizeof(float));
	size_t i, j;
	for (i = 0; i < N; i++) {
		for (j = 0; j < row_c; j++) {
			R[i * row_c + j] = r[j];
		}
	}

	/* Create PHIx vector */
	float* PHIx = (float*)malloc(N * row_c * sizeof(float));
	mul(PHI, x, PHIx, N * row_c, row_a, 1);

	/* Create bqp = [S + R - PHIx; Umax; Umax*0] vector */
	float* bqp = malloc((N * row_c + N * column_b + N * column_b) * sizeof(float));
	memset(bqp, 0, (N * row_c + N * column_b + N * column_b) * sizeof(float));
	for(i = 0; i < N; i++) {
        for (j = 0; j < row_c; j++) {
            bqp[i * row_c + j] = S[j] + R[i] - PHIx[i];
        }
	}
	for (i = N * row_c; i < N * row_c + N; i++) {
		for (j = 0; j < column_b; j++) {
			bqp[i * column_b + j] = Umax[j];
		}
	}

	/* Transpose GAMMA */
	float* GAMMAT = (float*)malloc((N * row_c) * (N * column_b) * sizeof(float));
	memcpy(GAMMAT, GAMMA, (N * row_c) * (N * column_b) * sizeof(float));
	tran(GAMMAT, N * row_c, N * column_b);

	/* Create qqp = GAMMAT * GAMMA matrix */
	float* qqp = (float*)malloc(N * column_b * N * column_b * sizeof(float));
	mul(GAMMAT, GAMMA, qqp, N * column_b, N * row_c, N * column_b);

	/* Add regularization to qqp matrix */
	for (i = 0; i < N * column_b; i++) {
		qqp[i * N * column_b + i] += lambda;
	}

	/* Create aqp = [GAMMA; I; -I] */
	float* aqp = (float*)malloc((N * row_c + N * column_b + N * column_b) * (N * column_b) * sizeof(float));
	memset(aqp, 0, (N * row_c + N * column_b + N * column_b) * (N * column_b) * sizeof(float));
	memcpy(aqp, GAMMA, (N * row_c) * (N * column_b) * sizeof(float));
	for (i = 0; i < N * column_b; i++) {
		aqp[i * (N * column_b) + i + (N * row_c) * (N * column_b)] = 1.0f;
		aqp[i * (N * column_b) + i + (N * row_c + N * column_b) * (N * column_b)] = -1.0f;
	}

	/* Create PHIx_R vector */
	float* PHIx_R = (float*)malloc(N * row_c * sizeof(float));
	for (i = 0; i < N * row_c; i++) {
		PHIx_R[i] = PHIx[i] - R[i];
	}

	/* Create cqp vector */
	float* cqp = (float*)malloc(N * column_b * sizeof(float));
	mul(GAMMAT, PHIx_R, cqp, N * column_b, N * row_c, 1);
	scalar(cqp, lambda, N * column_b);

	/* Find the optimal solution with quadprog - We are using R as the output, instead of u */
	quadprog(qqp, cqp, aqp, bqp, NULL, NULL, R, N * row_c + N * column_b + N * column_b, 0, N * column_b, false);

    /* We select the best input values, depending on if we have integration behavior or not in our model */
	if(has_integration_action){
		/* Set first R to u */
		for(i = 0; i < column_b; i++) {
			u[i] = R[i] - integration_constant * u[i];
		}
	}else{
		/* Set last R to u */
		for(i = 0; i < column_b; i++) {
			u[i] = R[N * column_b - column_b + i];
		}
	}

	/* Free */
	free(R);
	free(PHIx);
	free(GAMMAT);
	free(bqp);
	free(qqp);
	free(cqp);
	free(aqp);
	free(PHIx_R);
}

/*
% Use Model Predictive Control with integral action and quadratic programming
% Input: sysd(Discrete state space model), N(Horizon number), R(Reference vector), T(End time), a(lambda regularization parameter), Umax(Maximum input signal, optional), S(Slack variables for output, optional), I(integral parameter 0 to 1, optional), x0(Initial state, optional)
% Output: y(Output signal), T(Discrete time vector), X(State vector), U(Output signal)
% Example 1: [Y, T, X, U] = mc.qmpc(sysd, N, R, T)
% Example 2: [Y, T, X, U] = mc.qmpc(sysd, N, R, T, a)
% Example 3: [Y, T, X, U] = mc.qmpc(sysd, N, R, T, a, Umax)
% Example 4: [Y, T, X, U] = mc.qmpc(sysd, N, R, T, a, Umax, S)
% Example 5: [Y, T, X, U] = mc.qmpc(sysd, N, R, T, a, Umax, S, I)
% Example 6: [Y, T, X, U] = mc.qmpc(sysd, N, R, T, a, Umax, S, I, x0)
% Author: Daniel Mårtensson 2022 September 3
% Update 2023-02-18: Faster quadprog, also renamed quadprog2 to quadprog
% Update 2024-07-29: Boundaries for input signals and output signals
% Update 2024-07-30: Soft constraints S on output

function [Y, T, X, U] = qmpc(varargin)
  % Check if there is any input
  if(isempty(varargin))
    error ('Missing model')
  end

  % Get model type
  type = varargin{1}.type;
  % Check if there is a TF or SS model
  if(strcmp(type, 'SS' ))
    % Get A, B, C, D matrecies
    sys = varargin{1};
    % Check if the model is discrete! MPC can only be discrete in this case.
    if sys.sampleTime <= 0
      error('Only discrete state space models');
    end
    A = sys.A;
    B = sys.B;
    C = sys.C;
    D = sys.D;
    delay = sys.delay;
    sampleTime = sys.sampleTime;

    % Get the horizon number
    if(length(varargin) >= 2)
      N = varargin{2};
    else
      error('Missing the horizon number');
    end

    % Get the reference vector R
    if(length(varargin) >= 3)
      try
        R = repmat(varargin{3}, N/length(varargin{3}), 1);
      catch
        error('Try to change the horizon N number so it can be divided with length of reference R vector');
      end
    else
      error('Missing the reference vector R');
    end

    % Get the total time
    if(length(varargin) >= 4)
      t = 0:sampleTime:varargin{4};
    else
      error('Missing the end time T');
    end

    % Get the lambda
    if(length(varargin) >= 5)
      a = varargin{5};
    else
      a = 0;
    end

    % Get the max input value
    if(length(varargin) >= 6)
      Umax = repmat(varargin{6}, N/length(varargin{6}), 1);
    else
      Umax = R;
    end

    % Get the slack variable value
    if(length(varargin) >= 7)
      S = repmat(varargin{7}, N/length(varargin{7}), 1);
    else
      S = 0;
    end

    % Get the integral action parameter
    if(length(varargin) >= 8)
      I = varargin{8};
    else
      I = 0; % Max integral action
    end

    % Get the initial trajectory vector x
    if(length(varargin) >= 9)
      x = varargin{9};
    else
      x = zeros(size(A, 1), 1);
    end

    % Check if the system has integration behaviour already
    abseigenvalues = abs(mc.pole(sys));
    if(max(abseigenvalues) < 1)
      % Add integral action - It's very good and pratical!
      % A = [A B; 0 I]
      % B = [0; I]
      % C = [C 0]
      % x = [x; u(k-1)]
      am = size(A, 1);
      bn = size(B, 2);
      cm = size(C, 1);
      A = [A B; zeros(bn, am) eye(bn, bn)];
      B = [zeros(am, bn); eye(bn, bn)];
      C = [C zeros(cm, bn)];
      x = [x; zeros(bn, 1)];
    end

    % Check if it's MATLAB or Octave
    isOctave = exist('OCTAVE_VERSION', 'builtin') ~= 0;

    % Find the observability matrix PHI and lower triangular toeplitz matrix GAMMA of C*PHI
    PHI = phiMat(A, C, N);
    GAMMA = gammaMat(A, B, C, N);

    % Solve: R = PHI*x + GAMMA*U with quadratic programming: Min: 1/2x^TQx + c^Tx, S.t: Ax <= b, x >= 0
    % Q = a*eye(size(GAMMA))
    % cqp = GAMMA'*Q*(PHI*x - R)
    % bqp = [S + R - PHI*x; Umax; 0]
    % aqp = [GAMMA; I; -I]
    % qqp = GAMMA'*GAMMA + Q

    % Create the q-matrix for QP-solver
    Q = a*eye(size(GAMMA));
    qqp = GAMMA'*GAMMA + Q;

    % Constraints on: Upper bounds outputs, Upper bounds inputs, Lower bounds inputs
    aqp = [GAMMA; eye(length(Umax), size(GAMMA, 2)); -eye(length(Umax), size(GAMMA, 2))];

    % Loop and save the data
    past_inputs = zeros(1, size(B, 2));
    delta = zeros(1, size(B, 2));
    for k = 1:length(t)
      % Return states and input signals
      X(:,k) = x;
      U(:,k) = delta; % First element!

      % Compute outputs
      Y(:,k) = C*x + D*delta; % size(B, 2) = If multiple inputs...

      % Update states
      % For applying this MPC regulator in reality, then state vector x should be the "sensors" if C = identity and D = 0
      x = A*x + B*delta;

      % Update the objective function
      cqp = GAMMA'*Q*(PHI*x - R);

      % Update the constraints
      bqp = [S + R-PHI*x; Umax; Umax*0];

      % Quadratic programming
      if(isOctave == 1)
        [u, ~, e] = qp ([], qqp, cqp, [], [], [], [], [], aqp, bqp);
        if(e.info == 3)
          error('Quadratic programming QP could not optimize input signals. Try increase the horizion N number.');
        end
      else
        [u, solution] = mc.quadprog(qqp, cqp, aqp, bqp); % Used for MATLAB users
        if(solution == false)
          error('Quadratic programming quadprog could not optimize input signals. Try to decrease the horizion N number or remove/change lambda regularization. Perhaps increase the slack variable.');
        end
      end

      % Do integral action by compare new inputs with old inputs
      delta = u(1:size(B, 2)) - I*past_inputs;
      past_inputs = u(1:size(B, 2));
    end

    % Change t and y vector and u so the plot look like it is discrete - Important!
    for(i = 1:2:length(Y)*2)
      leftPart = Y(:,1:i);
      rightPart = Y(:,(i+1):end);
      Y = [leftPart Y(:,i) rightPart];
    end

    for(i = 1:2:length(t)*2)
      leftPart = t(1:i);
      rightPart = t((i+1):end);
      t = [leftPart t(i) rightPart];
    end

    for(i = 1:2:length(U)*2)
      leftPart = U(:,1:i);
      rightPart = U(:,(i+1):end);
      U = [leftPart U(:,i) rightPart];
    end

    for(i = 1:2:length(X)*2)
      leftPart = X(:,1:i);
      rightPart = X(:,(i+1):end);
      X = [leftPart X(:,i) rightPart];
    end

    % Just remove the first one
    T = t(:,2:length(t));
    % And the last one
    Y = Y(:,1:(length(Y)-1));
    % And for U and X too
    U = U(:,1:(length(U)-1));
    X = X(:,1:(length(X)-1));
    % Now we have two vectors which look like a discrete signal

    % Plot - How many subplots?
    for i = 1:size(C,1)
      subplot(size(C,1),1,i)
      plot(T, Y(i,:));
      ylabel(strcat('y', num2str(i)));
      if (sampleTime > 0)
        xlabel(strcat(num2str(sampleTime), ' time unit/sample'));
      else
        xlabel('Time units');
      end
      grid on
    end
  end
  title('Model Predictive Control With Integral Action And Quadratic Programming')
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
*/
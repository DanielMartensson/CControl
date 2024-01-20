/*
 * svm.c
 *
 *  Created on: 6 maj 2023
 *      Author: Daniel M�rtensson
 */

#include "machinelearning.h"

/*
 * Do Support Vector Machine
 * X[m*n]
 * y[m]
 * w[n]
 * b = Single value
 * C = Upper boundary of the solution of the quadratic programming can be
 * lambda = Makes it faster to compute, with cost of accuracy
 * This function return weigths w and bias b, the accuracy of the model and if the QP-solver could optimize.
 * 
 * To verify an unknown measurement:
 * class_ID = sign(w*x + b), where x is an unknown measurement vector
 * 
 */
bool svm(const float X[], const float y[], float w[], float* b, float* accuracy, const float C, const float lambda, const size_t row, const size_t column) {
	/* Create Q = (y*y').*(X*X') */
	float* Q = (float*)malloc(row * row * sizeof(float));
	float* Q0 = Q;
	memset(Q, 0, row * row * sizeof(float));
	size_t i, j, k;
	for (i = 0; i < row; i++) {
		for (j = 0; j < row; j++) {
			for (k = 0; k < column; k++) {
				Q[j] += y[i] * y[j] * X[i * column + k] * X[j * column + k];
			}
		}
		Q += row;
	}
	Q = Q0;

	/* Add regularization Q = Q + lambda*I */
	for (i = 0; i < row; i++) {
		Q[i] += lambda;
		Q += row;
	}
	Q = Q0;

	/* Create c */
	float* c = (float*)malloc(row * sizeof(float));
	for (i = 0; i < row; i++) {
		c[i] = -1.0f;
	}

	/*
	 * Create inequality constraints for alpha as they were equality constraints
	 * Aeq = y';
	 * G = [eye(size(Q)); -eye(size(Q)); Aeq; -Aeq]
	 */
	const size_t row_g = row + row + 2;
	float* G = (float*)malloc(row_g * row * sizeof(float));
	float* G0 = G;
	memset(G, 0, row_g * row * sizeof(float));
	for (i = 0; i < row; i++) {
		G[i] = 1.0f;
		G += row;
	}
	for (i = 0; i < row; i++) {
		G[i] = -1.0f;
		G += row;
	}
	memcpy(G, y, row * sizeof(float));
	G += row;
	for (i = 0; i < row; i++) {
		G[i] = -y[i];
	}
	G = G0;

	/*
	 * Create and upper and lower bounds of alpha. Notice that lb = 0 and beq = 0
	 * h = [ub; -lb; beq; -beq]
	 * C is the upper limit of alpha
	 */
	float* h = (float*)malloc(row_g * sizeof(float));
	memset(h, 0, (row + row + 2) * sizeof(float));
	for (i = 0; i < row; i++) {
		h[i] = C;
	}

	/*
	 * Do Support Vector Machine
	 * Min 1/2x^TQx + c^Tx
	 *     Gx = h
	 *     lb <= x <= ub
	 */
	float* alpha = (float*)malloc(row * sizeof(float));
	bool solution = quadprog(Q, c, G, h, NULL, NULL, alpha, row_g, 0, row, false);

	/* Find weights w = (alpha.*y')*X */
	float* X0 = X;
	const float tol = 1e-05f;
	for (j = 0; j < column; j++) {
		/* Support vectors have non zero lagrange multipliers */
		X = X0;
		w[j] = 0.0f;
		for (i = 0; i < row; i++) {
			if (alpha[i] > tol) {
				w[j] += alpha[i] * y[i] * X[j];
			}
			X += column;
		}
	}

	/* Find bias b = mean(y - X*w') */
	*b = 0.0f;
	X = X0;
	float d;
	size_t counter = 0;
	for (i = 0; i < row; i++) {
		/* Support vectors have non zero lagrange multipliers */
		if (alpha[i] > tol) {
			d = 0.0f;
			for (j = 0; j < column; j++) {
				d += X[j] * w[j];
			}
			*b += y[i] - d;
			counter++;
		}
		X += column;
	}
	*b /= ((float)counter);
	X = X0;

	/* Predict */
	float predicted_class_ID, actual_class_ID;
	counter = 0;
	for (i = 0; i < row; i++) {
		/* Compute prediction */
		d = 0.0f;
		for (j = 0; j < column; j++) {
			d += w[j] * X[j];
		}
		X += column;
		predicted_class_ID = sign(d + *b);
		actual_class_ID = y[i];

		/* Count if the prediction is right */
		if ((actual_class_ID > 0 && predicted_class_ID > 0) || (actual_class_ID < 0 && predicted_class_ID < 0)) {
			counter++;
		}
	}

	/* Compute accuracy */
	*accuracy = ((float)counter) / ((float)row);

	/* Free */
	free(Q);
	free(c);
	free(G);
	free(h);
	free(alpha);

	/* Return if the solution was found */
	return solution;
}

/* GNU Octave code
	% Linear Support Vector Machine
	% Input: x(Data matrix), y(Labels), C(Upper bondary), lambda(Regularization)
	% Example: [w, b, accuracy] = lsvm(x, y, C, lambda);
	% Author: Daniel M�rtensson, May 06, 2023
	% To verify the model, just use:
	% class_ID = sign(w*x + b), where x is an unknown measurement vector

	function [w, b, accuracy] = lsvm(varargin)
	  % Check if there is any input
	  if(isempty(varargin))
		error('Missing inputs')
	  end

	  % Get data X
	  if(length(varargin) >= 1)
		x = varargin{1};
	  else
		error('Missing data x')
	  end

	  % Get labels y
	  if(length(varargin) >= 2)
		y = varargin{2};
	  else
		error('Missing labels y')
	  end

	  % Get C
	  if(length(varargin) >= 3)
		C = varargin{3};
		if(C < 0 )
		  C = eps;
		end
	  else
		error('Missing C')
	  end

	  % Get lambda
	  if(length(varargin) >= 4)
		lambda = varargin{4};
	  else
		error('Missing lambda')
	  end

	  % Compute rows and columns
	  [m, n] = size(x);

	  % Train Support Vector Machine
	  Q = (y*y').*(x*x');
	  Q = Q + lambda*eye(size(Q));
	  c = -ones(size(y));
	  Aeq = y';
	  beq = 0;
	  lb = zeros(size(y));
	  ub = C*ones(size(y));
	  G = [eye(size(Q)); -eye(size(Q)); Aeq; -Aeq];
	  h = [ub; -lb; beq; -beq];
	  [alpha, solution] = quadprog(Q, c, G, h);

	  % Support vectors have non zero lagrange multipliers
	  tol = 1e-5;
	  sv_idx = find(alpha > tol);

	  % Find weights and bias
	  w = (alpha(sv_idx).* y(sv_idx))' * x(sv_idx, :);
	  b = mean(y(sv_idx)-x(sv_idx,:)*w');

	  % Check if the training result well
	  counter = 0;
	  for i = 1:m
		% Predict
		predicted_class_ID = sign(w*x(i, :)' + b);
		actual_class_ID = y(i);

		% Count if the prediction is right
		if(or(and(actual_class_ID > 0, predicted_class_ID > 0), and(actual_class_ID < 0, predicted_class_ID < 0)))
		  counter = counter + 1;
		end

	  end

	  % Compute accuracy
	  accuracy = counter/m;
	end

*/

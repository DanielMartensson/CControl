/*
 * quadprogslim.c
 *
 *  Created on: 27 aug. 2022
 *      Author: Daniel Mårtensson
 */

#include "optimization.h"

static STATUS_CODES optislim(const float Q[], const float c[], const float A[], const float b[], float x[], const size_t row_a, const size_t column_a);

/**
 * This is quadratic programming with optimized Hildreth's method by Luenberger, 1969, Wismer and Chattergy, 1978.
 * The algorithm is slightly changed and even more optimized by Daniel Mårtensson(2025) in both size and speed.
 * Min 1/2x^TQx + c^Tx
 * S.t Ax <= b
 *     Gx = h
 *
 * If you want to do maximization, then turn Q and c negative. The constraints are the same
 *
 * Max 1/2x^T(-Q)x + (-c)^Tx
 * S.t Ax <= b
 *     Gx = h
 *
 * Call this function with the sizes
 * Q [column_a*column_a]	// Symmetric matrix
 * c [column_a]				// Objective function
 * A [row_a*column_a]		// Inequality constraint matrix
 * b [row_a]				// Inequality constraint vector
 * G [row_g*column_a]		// Equality constraint matrix
 * h [row_g]				// Equality constraint vector
 * x [column_a]				// Solution
 */
STATUS_CODES quadprogslim(const float Q[], const float c[], const float A[], const float b[], const float G[], const float h[], float x[], const size_t row_a, const size_t row_g, const size_t column_a, const bool equality_constraints_are_used) {
	if (equality_constraints_are_used) {
		/* Create multiple inequality constraints. Those are going to be equality constranits */
		float* A_long = (float*)malloc((row_a + row_g + row_g) * column_a * sizeof(float));
		float* A_long0 = A_long;
		float* b_long = (float*)malloc((row_a + row_g + row_g) * sizeof(float));
		float* b_long0 = b_long;

		/* Copy over A_long = [A; G; -G] */
		memcpy(A_long, A, row_a * column_a * sizeof(float));
		A_long += row_a * column_a;
		size_t row_g_column_a = row_g * column_a;
		memcpy(A_long, G, row_g_column_a * sizeof(float));
		A_long += row_g * column_a;
		size_t i;
		for (i = 0; i < row_g_column_a; i++) {
			A_long[i] = -G[i];
		}
		A_long = A_long0; 
		
		/* Copy over b_long = [b; h; -h] */
		memcpy(b_long, b, row_a * sizeof(float));
		b_long += row_a;
		memcpy(b_long, h, row_g * sizeof(float));
		b_long += row_g;
		for (i = 0; i < row_g; i++) {
			b_long[i] = -h[i];
		}
		b_long = b_long0;

		/* Optimize */
		const STATUS_CODES status = optislim(Q, c, A_long, b_long, x, row_a + row_g + row_g, column_a);

		/* Free */
		free(A_long);
		free(b_long);

		/* Return status */
		return status;
	}
	else {
		return optislim(Q, c, A, b, x, row_a, column_a);
	}
}

static void linsolve_upper_tran_triangular(const float A[], float x[], const float b[], const size_t row) {
	int32_t i, j;
	for (i = row - 1; i >= 0; i--) {
		/* Start with b[i] and substract the values that already has been solved */
		x[i] = b[i]; 

		/* A[j * row] because A is lower triangular but is interpreted as upper triangular */
		for (j = i + 1; j < row; j++) {
			x[i] -= A[i + j * row] * x[j];
		}

		/* Divide with the element of the diangonal */
		x[i] /= A[i * row + i];
	}
}

static STATUS_CODES optislim(const float Q[], const float c[], const float A[], const float b[], float x[], const size_t row_a, const size_t column_a){
	/* Declare */
	size_t i, j, k;

	/* Create the cholesky lower triangular matrix L from Q */
	float* L = (float*)malloc(column_a * column_a * sizeof(float));
	float* y = (float*)malloc(column_a * sizeof(float));
	if (!chol(Q, L, column_a)) {
		free(L);
		free(y);
		return STATUS_NAN;
	}

	/* Use Cholesky factorization to solve x from Qx = c because Q is square and symmetric */
	linsolve_lower_triangular(L, y, c, column_a);
	linsolve_upper_tran_triangular(L, x, y, column_a);

	/* Turn x negative */
	for (i = 0; i < column_a; i++) {
		x[i] = -x[i];
#ifdef _MSC_VER
		if (_isnanf(x[i])) {
			free(L);
			free(y);
			return STATUS_NAN;
		}
#elif  __GNUC__
		if (_isnanf(x[i])) {
			free(L);
			free(y);
			return STATUS_NAN;
		}
#else
		if (isnanf(x[i])) {
			free(L);
			free(y);
			return STATUS_NAN;
		}
#endif
	}

	/* Check how many rows are A*x > b 
	j = 0;
	for (i = 0; i < row_a; i++) {
		float K = dot(A + i * column_a, x, column_a);

		/ Constraints difference /
		K = b[i] - K;

		/ Check constraint violation /
		if (K < 0.0f) {
			j++;
		}
	}
	if (j == 0) {
		/ No violation /
		free(L);
		free(y);
		return STATUS_OK;
	}*/

	/* Allocate memory for special case P */
	float* P = (float*)malloc(column_a * sizeof(float));

	/* Solve lambda from H* lambda = -K, where lambda >= 0 */
	float* lambda = (float*)calloc(row_a, sizeof(float));
	for (i = 0; i < MAX_ITERATIONS; i++) {
		/* Find lambda */
		float v = 0.0f;
		for (j = 0; j < row_a; j++) {
			/* Check how many rows are A*x > b */
			float K = dot(A + j * column_a, x, column_a);

			/* Constraints difference */
			K = b[j] - K;

			/* Check constraint violation */
			if (K >= 0.0f) {
				continue;
			}

			/* Solve QP = A' (Notice that we are using a little trick here so we can avoid A') */       
			linsolve_lower_triangular(L, y, A + j * column_a, column_a);
			linsolve_upper_tran_triangular(L, P, y, column_a);

			/* Multiply H = A*Q*A' */
			float Hii = 1.0f;
			float w = 0.0f;
			for (k = 0; k < row_a; k++) {
				/* Compute H */
				const float H = dot(A + k * column_a, P, column_a);

				/* Save H(i,i) when we are at the diagonal */
				if (j == k) {
					Hii = H;
				}

				/* w = H(i, :)*lambda */
				w += H * lambda[k];
			}

			/* Find a solution */
			w = -1.0f / Hii * (K + w - Hii * lambda[j]);
			Hii = vmax(0.0f, w);
			v += Hii - lambda[j];
			lambda[j] = Hii;
		}

		/* Negative times negative becomes positive */
		v = v * v;

#ifdef _MSC_VER
		if (_isnanf(v)) {
			free(L);
			free(y);
			free(P);
			free(lambda);
			return STATUS_NAN;
		}

		if (v < MIN_VALUE) {
			break;
		}
#elif __GNUC__
		 if (_isnanf(v)) {
			 free(L);
			 free(y);
			 free(P);
			 free(lambda);
			 return STATUS_NAN;
		 }

		 if (v < MIN_VALUE) {
			 break;
		 }
#else
		if (isnanf(v)) {
			free(L);
			free(y);
			free(P);
			free(lambda);
			return STATUS_NAN;
		}

		if (v < MIN_VALUE) {
			break;
		}
#endif
	}
	
	/* Compute solution x */
	for (j = 0; j < row_a; j++) {
		linsolve_lower_triangular(L, y, A + j * column_a, column_a);
		linsolve_upper_tran_triangular(L, P, y, column_a);
		for (k = 0; k < column_a; k++) {
		  x[k] -= P[k] * lambda[j];
		}
	}

	/* Free */
	free(lambda);
	free(P);
	free(L);
	free(y);

	/* If i equal to MAX_ITERATIONS, then it did not find a solution */
	return i < MAX_ITERATIONS ? STATUS_OK : STATUS_NOT_OPTIMAL_SOLUTION;
}

/* GNU Octave code:
 *
	% This is quadratic programming with Hildreth's method
	% Min 1/2x^TQx + c^Tx
	% S.t Ax <= b
	%
	% If you want to do maximization, then turn Q and c negative. The constraints are the same
	%
	% Max 1/2x^T(-Q)x + (-c)^Tx
	% S.t Ax <= b
	%
	% Input: Q(Symmetric matrix), c(Objective function), A(Constraint matrix), b(Constraint vector)
	% Output: x(Solution vector), solution(boolean flag)
	% Example 1: [x, solution] = mc.quadprog(Q, c, A, b)
	% Author: Daniel Mårtensson 2022 September 3

	function [x, solution] = quadprog(Q, c, A, b)
	  % Assume that the solution is true
	  solution = true;

	  % Same as in C code for Functions.h at CControl
	  MIN_VALUE = 1e-11;
	  MAX_ITERATIONS = 10000;

	  % Unconstrained solution
	  x = -linsolve(Q, c);

	  % Constraints difference
	  K = b - A*x;

	  % Check constraint violation
	  if(sum(K < 0) == 0)
		return; % No violation
	  end

	  % Create P
	  P = linsolve(Q, A');

	  % Create H = A*Q*A'
	  H = A*P;

	  % Solve lambda from H*lambda = -K, where lambda >= 0
	  [m, n] = size(K);
	  lambda = zeros(m, n);
	  for km = 1:MAX_ITERATIONS
		lambda_p = lambda;

		% Use Gauss Seidel
		for i = 1:m
		  w = -1.0/H(i,i)*(K(i) + H(i,:)*lambda - H(i,i)*lambda(i));
		  lambda(i) = max(0, w);
		end

		% Check if the minimum convergence has been reached
		w = (lambda - lambda_p)'*(lambda - lambda_p);
		if (w < MIN_VALUE)
		  break;
		end

		% Check if the maximum iteration have been reached
		if(km == MAX_ITERATIONS)
		  solution = false;
		  return;
		end
	  end

	  % Find the solution: x = -inv(Q)*c - inv(Q)*A'*lambda
	  x = x - P*lambda;
	end
 */

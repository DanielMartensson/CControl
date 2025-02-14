/*
 * quadprog.c
 *
 *  Created on: 27 aug. 2022
 *      Author: Daniel Mårtensson
 */

#include "optimization.h"

static bool opti(const float Q[], const float c[], const float A[], const float b[], float x[], const size_t row_a, const size_t column_a);

/**
 * This is quadratic programming with optimized Hildreth's method by Daniel Mårtensson
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
bool quadprog(const float Q[], const float c[], const float A[], const float b[], const float G[], const float h[], float x[], const size_t row_a, const size_t row_g, const size_t column_a, const bool equality_constraints_are_used) {
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
		const bool status = opti(Q, c, A_long, b_long, x, row_a + row_g + row_g, column_a);

		/* Free */
		free(A_long);
		free(b_long);

		/* Return status */
		return status;
	}
	else {
		return opti(Q, c, A, b, x, row_a, column_a);
	}
}


static bool opti(const float Q[], const float c[], const float A[], const float b[], float x[], const size_t row_a, const size_t column_a) {
	/* Declare */
	size_t i, j, k;

	/* Use Cholesky factorization to solve x from Qx = c because Q is square and symmetric */
	linsolve_chol(Q, x, c, column_a);

	/* Turn x negative */
	for (i = 0; i < column_a; i++) {
		x[i] = -x[i];

		/* If x was nan - return false - Use higher value on lambda constant! */
#ifdef _MSC_VER
		if (_isnanf(x[i])) {
			return false;
		}
#else
		if (isnanf(x[i])) {
			return false;
		}
#endif
	}

	/* Check how many rows are A*x > b */
	j = 0;
	for (i = 0; i < row_a; i++) {
		float K = dot(A + i * column_a, x, column_a);

		/* Constraints difference */
		K = b[i] - K;

		/* Check constraint violation */
		if (K < 0.0f) {
			j++;
		}
	}
	if (j == 0) {
		/* No violation */
		return true;
	}

	/* Solve QP = A' (Notice that we are using a little trick here so we can avoid A') */
	float* P = (float*)malloc(row_a * column_a * sizeof(float));
	for (i = 0; i < row_a; i++) {
		linsolve_chol(Q, P + i * column_a, A + i * column_a, column_a);
	}

	/* Create lambda and lambda past */
	float* lambda = (float*)malloc(row_a * sizeof(float));
	memset(lambda, 0, row_a * sizeof(float));

	/* Count how many constraints A*x > b */
	float v;
	for (i = 0; i < MAX_ITERATIONS; i++) {
		/* Find lambda */
		float K, w;
		v = 0.0f;
		for (j = 0; j < row_a; j++) {
			/* Check how many rows are A*x > b */
			K = dot(A + j * column_a, x, column_a);

			/* Constraints difference */
			K = b[j] - K;

			/* Multiply H = A*Q*A' */
			float Hii = 1.0f;
			w = 0.0f;
			for (k = 0; k < row_a; k++) {
				/* Compute H */
				const float H = dot(A + k * column_a, P + j * column_a, column_a);

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

#ifdef _MSC_VER
		if (v < MIN_VALUE || _isnanf(v)) {
			break;
		}
#else
		if (v < MIN_VALUE || isnanf(v)) {
			break;
		}
#endif
	}
	print(lambda, 1, row_a);
	for (j = 0; j < column_a; j++) {
		for (k = 0; k < row_a; k++) {
			x[j] -= P[k * column_a + j] * lambda[k];
		}
	}

	/* Free */
	free(lambda);
	free(P);

	/* If i equal to MAX_ITERATIONS, then it did not find a solution */
#ifdef _MSC_VER
	return i < MAX_ITERATIONS && !_isnanf(v);
#else
	return i < MAX_ITERATIONS && !isnanf(v);
#endif
}

/*static bool opti(const float Q[], const float c[], const float A[], const float b[], float x[], const size_t row_a, const size_t column_a);
  
   THIS IS OLD CODE!

 * This is quadratic programming with Hildreth's method
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
 
bool quadprog(const float Q[], const float c[], const float A[], const float b[], const float G[], const float h[], float x[], const size_t row_a, const size_t row_g, const size_t column_a, const bool equality_constraints_are_used) {
	if (equality_constraints_are_used) {
		// Create multiple inequality constraints. Those are going to be equality constranits
		float* A_long = (float*)malloc((row_a + row_g + row_g) * column_a * sizeof(float));
		float* A_long0 = A_long;
		float* b_long = (float*)malloc((row_a + row_g + row_g) * sizeof(float));
		float* b_long0 = b_long;

		// Copy over A_long = [A; G; -G]
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
		
		// Copy over b_long = [b; h; -h]
		memcpy(b_long, b, row_a * sizeof(float));
		b_long += row_a;
		memcpy(b_long, h, row_g * sizeof(float));
		b_long += row_g;
		for (i = 0; i < row_g; i++) {
			b_long[i] = -h[i];
		}
		b_long = b_long0;

		// Optimize
		const bool status = opti(Q, c, A_long, b_long, x, row_a + row_g + row_g, column_a);

		// Free
		free(A_long);
		free(b_long);

		// Return status
		return status;
	}
	else {
		return opti(Q, c, A, b, x, row_a, column_a);
	}
}


static bool opti(const float Q[], const float c[], const float A[], const float b[], float x[], const size_t row_a, const size_t column_a){
	// Declare
	size_t i, j, k;
	
	// Use Cholesky factorization to solve x from Qx = c because Q is square and symmetric
	linsolve_chol(Q, x, c, column_a);

	// Save address 
	const float* Ai = A;

	// Turn x negative
	for (i = 0; i < column_a; i++) {
		x[i] = -x[i];

		// If x was nan - return false - Use higher value on lambda constant!
#ifdef _MSC_VER
		if (_isnanf(x[i])) {
			return false;
		}
#else
		if (isnanf(x[i])) {
			return false;
		}
#endif
	}

	// Count how many constraints A*x > b
	float *K = (float*)malloc(row_a * sizeof(float));
	size_t violations = 0;
	float value;
	for(i = 0; i < row_a; i++){

		// Check how many rows are A*x > b
		value = 0.0f;
		for(j = 0; j < column_a; j++){
			value += Ai[j] * x[j];
			// value += A[i*column_a + j]*x[j];
		}
		Ai += column_a;

		// Constraints difference
		K[i] = b[i] - value;

		// Check constraint violation
		if (K[i] < 0.0f) {
			violations++;
		}
	}

	// No violation
	if (violations == 0) {
		free(K);
		return true;
	}

	// Solve QP = A' (Notice that we are using a little trick here so we can avoid A')
	float *P = (float*)malloc(row_a * column_a * sizeof(float));
	float *P0 = P;
	Ai = A;
	for (i = 0; i < row_a; i++) {
		linsolve_chol(Q, P, Ai, column_a);
		// linsolve_gauss(Q, P, Ai, column_a, column_a, 0.0f); - Old
		P += column_a;
		Ai += column_a;
	}
	P = P0;
	tran(P, row_a, column_a);

	// Multiply H = A*Q*A'
	float *H = (float*)malloc(row_a * row_a * sizeof(float));
	float *Hj;
	mul(A, P, H, row_a, column_a, row_a);

	// Solve lambda from H*lambda = -K, where lambda >= 0
	float *lambda = (float*)malloc(row_a * sizeof(float));
	memset(lambda, 0, row_a * sizeof(float));
	float *lambda_p = (float*)malloc(row_a * sizeof(float));
	float w;
	for(i = 0; i < MAX_ITERATIONS; i++){
		// Update
		memcpy(lambda_p, lambda, row_a * sizeof(float));

		// Use Gauss Seidel
		Hj = H;
		float v = 0.0f;
		for (j = 0; j < row_a; j++) {
			// w = H(i, :)*lambda
			w = 0.0f;
			for (k = 0; k < row_a; k++) {
				w += Hj[k] * lambda[k];
			}
			
			// Find a solution
			w = -1.0f / Hj[j] * (K[j] + w - Hj[j]*lambda[j]);
			v += vmax(0.0f, w) - lambda[j];
			Hj += row_a;
			lambda[j] = vmax(0.0f, w);
		}

		// Check if we should break - Found the optimal solution
		w = 0.0f;
		for (j = 0; j < row_a; j++) {
			value = lambda[j] - lambda_p[j];
			w += value * value;
		}
#ifdef _MSC_VER
		if (v < MIN_VALUE || _isnanf(v)) {
			break;
		}
#else
		if (w < MIN_VALUE || isnanf(w)) {
			break;
		}
#endif
	}

	// Solve x = x + P*lambda (Notice that x is negative (see above))
	float *Plambda = (float*)malloc(column_a * sizeof(float));
	mul(P, lambda, Plambda, column_a, row_a, 1);
	for (j = 0; j < column_a; j++) {
		x[j] -= Plambda[j];
	}

	// Free
	free(K);
	free(P);
	free(H);
	free(lambda);
	free(lambda_p);
	free(Plambda);

	// If w was nan - return false 
#ifdef _MSC_VER
	if (_isnanf(w)) {
		return false;
	}
#else
	if (isnanf(w)) {
		return false;
	}
#endif

	// If i equal to MAX_ITERATIONS, then it did not find a solution
	return i < MAX_ITERATIONS;
}*/

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

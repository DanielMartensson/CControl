/*
 * quadprog.c
 *
 *  Created on: 27 aug. 2022
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"


/**
 * This is quadratic programming with Hildreth's method
 * Min 1/2x^TQx + c^Tx
 * S.t Ax <= b
 *
 * If you want to do maximization, then turn Q and c negative. The constraints are the same
 *
 * Max 1/2x^T(-Q)x + (-c)^Tx
 * S.t Ax <= b
 *
 * Call this function with the sizes
 * Q [n*n] // Symmetric matrix
 * c [n]  // Objective function
 * A [m*n] // Constraint matrix
 * b [m] // Constraint vector
 * x [n] // Solution
 */
bool quadprog(float Q[], float c[], float A[], float b[], float x[], uint16_t row_a, uint16_t column_a){
	/* Declare */
	uint16_t i, j, k;
	
	/* Use gaussian elimination to solve x from Qx = c because Q is square and symmetric */
	linsolve_gauss(Q, x, c, column_a, column_a, 0.0f);

	/* Save address */
	float* Ai = A;

	/* Turn x negative */
	for (i = 0; i < column_a; i++) {
		x[i] = -x[i];
	}

	/* Count how many constraints A*x > b */
	float *K = (float*)malloc(row_a * sizeof(float));
	uint16_t violations = 0;
	float value;
	for(i = 0; i < row_a; i++){

		/* Check how many rows are A*x > b */
		value = 0.0f;
		for(j = 0; j < column_a; j++){
			value += Ai[j] * x[j];
			/* value += A[i*column_a + j]*x[j]; */
		}
		Ai += column_a;

		/* Constraints difference */
		K[i] = b[i] - value;

		/* Count violation */
		if (K[i] <= 0.0f) {
			violations++;
		}
	}

	/* Check constraint violation */
	if (violations == 0) {
		free(K);
		return true;
	}

	/* Solve QP = A' (Notice that we are using a little trick here so we can avoid A') */
	float *P = (float*)malloc(row_a * column_a * sizeof(float));
	for (i = 0; i < row_a; i++) {
		linsolve_gauss(Q, &P[i * column_a], &A[i * column_a], column_a, column_a, 0.0f);
	}
	tran(P, row_a, column_a);

	/* Multiply H = A*Q*A' */
	float *H = (float*)malloc(row_a * row_a * sizeof(float));
	float *Hj;
	mul(A, P, H, row_a, column_a, row_a);

	/* Solve lambda from H*lambda = -K, where lambda >= 0 */
	float *lambda = (float*)malloc(row_a * sizeof(float));
	memset(lambda, 0, row_a * sizeof(float));
	float *lambda_p = (float*)malloc(row_a * sizeof(float));
	float w;
	for(i = 0; i < 1000; i++){
		/* Update */
		memcpy(lambda_p, lambda, row_a * sizeof(float));

		/* Use Gauss Seidel */
		Hj = H;
		for (j = 0; j < row_a; j++) {
			/* w = H(i, :)*lambda */
			w = 0.0f;
			for (k = 0; k < row_a; k++) {
				w += Hj[k] * lambda[k];
			}
			
			/* Find a solution */
			w = -1.0f / Hj[j] * (K[j] + w - Hj[j]*lambda[j]);
			Hj += row_a;
			lambda[j] = vmax(0.0f, w);
		}

		/* Check if we should break - Found the optimal solution */
		w = 0.0f;
		for (j = 0; j < row_a; j++) {
			value = lambda[j] - lambda_p[j];
			w += value * value;
		}
		if (w < FLT_EPSILON) {
			break;
		}
	}

	/* Solve x = x + P*lambda (Notice that x is negative (see above)) */
	float *Plambda = (float*)malloc(column_a * sizeof(float));
	mul(P, lambda, Plambda, column_a, row_a, 1);
	for (j = 0; j < column_a; j++) {
		x[j] -= Plambda[j];
	}

	/* Free */
	free(K);
	free(P);
	free(H);
	free(lambda);
	free(lambda_p);
	free(Plambda);
	return i <= 1000 ? true : false; /* If i was over 1000, then it did not find a solution */
}

/* GNU Octave code:
 *
 * function [x, solution] = quadprog(Q, c, A, b)
	  % Assume that the solution is true
	  solution = true;

	  % Set number of iterations
	  number_of_iterations = 255;

	  % Same as in C code
	  FLT_EPSILON = 1.19209290e-07;

	  % Unconstrained solution
	  x = -linsolve(Q, c);

	  % Constraints difference
	  K = b - A*x;

	  % Check constraint violation
	  if(sum(K <= 0) == 0)
		return; % No violation
	  end

	  % Create P
	  P = linsolve(Q, A');

	  % Create H = A*Q*A'
	  H = A*P;

	  % Solve lambda from H*lambda = -K, where lambda >= 0
	  [m, n] = size(K);
	  lambda = zeros(m, n);
	  for km = 1:number_of_iterations
		lambda_p = lambda;

		% Use Gauss Seidel
		for i = 1:m
		  w = -1.0/H(i,i)*(K(i) + H(i,:)*lambda - H(i,i)*lambda(i));
		  lambda(i) = max(0, w);
		end

		% Check if the minimum convergence has been reached
		w = (lambda - lambda_p)'*(lambda - lambda_p);
		if (w < FLT_EPSILON)
		  break;
		end

		% Check if the maximum iteration have been reached
		if(km == 255)
		  solution = false;
		  return;
		end
	  end

	  % Find the solution: x = -inv(Q)*c - inv(Q)*A'*lambda
	  x = x - P*lambda;
	end
 */

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
 * 		x >= 0
 *
 * If you want to do maximization, then turn Q and c negative. The constraints are the same
 *
 * Max 1/2x^T(-Q)x + (-c)^Tx
 * S.t Ax <= b
 * 		x >= 0
 *
 * Call this function with the sizes
 * Q [n*n] // Symmetric matrix
 * c [n]  // Objective function
 * A [m*n] // Constraint matrix
 * b [m] // Constraint vector
 * x [n] // Solution
 */
bool quadprog(float Q[], float c[], float A[], float b[], float x[], uint8_t row_a, uint8_t column_a){
	/* Use gaussian elimination to solve x from Qx = c because Q is square and symmetric */
	linsolve_gauss(Q, x, c, column_a, column_a, 0.0f);

	// Save address
	float *Ai = A;

	/* Turn x negative */
	for(uint8_t i = 0; i < column_a; i++)
		x[i] = -x[i];

	/* Count how many constraints A*x > b */
	uint8_t k = 0;
	float value;
	for(uint8_t i = 0; i < row_a; i++){

		/* Check how many rows are A*x > b */
		value = 0.0f;
		for(uint8_t j = 0; j < column_a; j++){
			value += Ai[j] * x[j];
			Ai += column_a;
			//value += A[i*column_a + j]*x[j];
		}
		if(value > b[i])
			k++;
	}

	/* If none - We have an optimal solution already */
	if(k == 0)
		return true;

	/* Solve QX = A' (Notice that we are using a little trick here so we can avoid A') */
	float X[row_a * column_a];
	for(uint8_t i = 0; i < row_a; i++)
		linsolve_gauss(Q, &X[i*column_a], &A[i*column_a], column_a, column_a, 0.0f);
	tran(X, row_a, column_a);

	/* Multiply P = A*X */
	float P[row_a * row_a];
	float *Pj;
	mul(A, X, P, row_a, column_a, row_a);

	/* Multiply d = A*Y + b (Notice that we are using A*x where x is negative (see above), but then later d = -d + b) */
	float d[row_a];
	mul(A, x, d, row_a, column_a, 1);
	for(uint8_t i = 0; i < row_a; i++)
		d[i] = -d[i] + b[i];

	/* Time to find the optimal solution */
	float lambda[row_a];
	memset(lambda, 0, row_a * sizeof(float));
	float lambda_p[row_a];
	float w;
	for(uint8_t i = 0; i < 255; i++){
		/* Update */
		memcpy(lambda_p, lambda, row_a * sizeof(float));

		Pj = P;
		for(uint8_t j = 0; j < row_a; j++){
			/* Do w = P(i, :)*lambda - P(i, i)*lambda(i, 1) + d(i, 1) */
			w = 0.0f;
			for(uint8_t k = 0; k < row_a; k++){
				w += Pj[k] * lambda[k];
				//w += P[j*row_a + k] * lambda[k];
			}
			w = w - Pj[j] * lambda[j] + d[j];
			//w = w - P[j*row_a + j]*lambda[j] + d[j];

			/* Find maximum */
			lambda[j] = vmax(0.0f, -w / Pj[j]);
			//lambda[j] = vmax(0, -w/P[j*row_a + j]);
			Pj += row_a;
		}

		/* Check if we should break - Found the optimal solution */
		w = 0.0f;
		for(uint8_t j = 0; j < row_a; j++)
			w += (lambda[j] - lambda_p[j])*(lambda[j] - lambda_p[j]);
		if(w < FLT_EPSILON)
			break;
		if(i == 255)
			return false; // No solution found
	}

	/* Solve x = x + X*lambda (Notice that x is negative (see above)) */
	float Xlambda[column_a];
	mul(X, lambda, Xlambda, column_a, row_a, 1);
	for(uint8_t i = 0; i < column_a; i++)
		x[i] -= Xlambda[i];
	return true;
}

/* GNU Octave code:
 *
 *
 * function [x, solution] = quadprog(Q, c, A, b)
	  x = -linsolve(Q, c);
	  solution = true;

	  [n1,m1]=size(A);
	  k = 0;
	  for i=1:n1
		if (A(i,:)*x > b(i))
		  k = k +1;
		end
	  end
	  if (k==0)
		return;
	  end

	  X = linsolve(Q, A');
	  P = A*X;

	  d = A*x;
	  d = -d + b;

	  [n,m] = size(d);
	  lambda = zeros(n,m);
	  for km = 1:255
	   lambda_p = lambda;
	   for i=1:n
		w = P(i,:)*lambda - P(i,i)*lambda(i,1) + d(i, 1);
		lambda(i,1) = max(0,-w/P(i,i));
	   end
	   w = (lambda - lambda_p)'*(lambda - lambda_p);
	   if (w < 10e-8)
		   break;
	   end
	   if(km == 255)
		  solution = false;
		  return;
		end
	  end

	  x = x - X*lambda;
	end
 */

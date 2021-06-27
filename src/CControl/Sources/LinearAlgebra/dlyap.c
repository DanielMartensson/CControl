/*
 * dlyap.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Discrete Lyapunov equation
 * Solves A * P * A' - P + Q = 0
 * A, P, Q need to be square and Q need to be positive and symmetric
 * A [m*n]
 * Q [m*n]
 * P [m*n]
 * n == m
 */
void dlyap(float* A, float* P, float* Q, uint16_t row){
	// Create an zero large matrix M
	float M[row*row*row*row]; // row_a^2 * row_a^2

	// Create a temporary B matrix
	float B[row*row];

	// Fill the M matrix
	for(uint16_t k = 0; k < row; k++){
		for(uint16_t l = 0; l < row; l++){
			memcpy(B, A, row*row*sizeof(float)); // B = A*A(k, l);
			for(uint16_t i = 0; i < row*row; i++)
				B[i] *= A[row*k + l];
			insert(B, M, row, row, row*row, row*k, row*l);
		}
	}

	// Turn M negative but add +1 on diagonals
	for(uint16_t i = 0; i < row*row; i++)
		for(uint16_t j = 0; j < row*row; j++)
			if(i == j)
				M[row*row*i + j] = - M[row*row*i + j] + 1;
			else
				M[row*row*i + j] = - M[row*row*i + j];

	/*
	 * Solve with LUP-Decomposition
	 * MP=Q, where P is our solution
	 */
	linsolve_lup(M, P, Q, row*row);
}

/*
 * GNU Octave code:
 *  function P = dlyap(A)
		% This function solves A * P * A' - P + Q = 0

		n = length(A);
		M = zeros(n^2, n^2);
		Q = zeros(n,n);

		% Create the symmetric postitive matrix Q
		for i = 0:n-1
		  for j = 0:n-1
			Q(i+1, j+1) = 1 + j + i;
		  end
		end

		for k = 0 : n-1
			for l = 0 : n-1
				B = A * A(k+1,l+1);
				M( (1:n) + n*k, (1:n) + n*l) = B;
			end
		end

		M = - M + eye(n^2, n^2);
		K = Q(:);
		x = linsolve(M, K);
		P = reshape(x, n, n);

		% Check if this is zero
		A * P * A' - P + Q
	end
 */

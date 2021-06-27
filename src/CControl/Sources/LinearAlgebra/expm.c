/*
 * expm.c
 *
 *  Created on: 8 août 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Find matrix exponential, return A as A = expm(A)
 * A[m*n]
 * m == n
 */
void expm(float A[], uint16_t row){
	// Create zero matrix
	float E[row*row];
	memset(E, 0, sizeof(E));
	// Create identity matrices
	float F[row*row];
	float T[row*row];
	memset(F, 0, sizeof(F));
	memset(T, 0, sizeof(T));
	for(uint16_t i = 0; i < row; i++){
		F[i*row + i] = 1;
		T[i*row + i] = 1;
	}
	uint8_t k = 1;
	while (norm(T, row, row, 1) > 0){
		// E = E + F
		for(uint16_t i = 0; i < row*row; i++){
			E[i] = E[i] + F[i];
		}
		// F = A*F/k (we are borrowing T)
		mul(A, F, T, row, row, row);
		for(uint16_t i = 0; i < row*row; i++){
			F[i] = T[i]/k;
		}
		// T = E + F - E ( This is faster than T = F)
		for(uint16_t i = 0; i < row*row; i++){
			T[i] = E[i] + F[i] - E[i];
		}
		k++;
	}
	memcpy(A, E, sizeof(E));
}

/*
 * MATLAB:
 * function E = expm2(A)
	  E = zeros(size(A));
	  F = eye(size(A));
	  T = eye(size(A));
	  k = 1;
	  while norm(T,1) > 0
		E = E + F;
		F = A*F/k;
		k = k+1;
		T = E+F-E;
	  end
	end
 */


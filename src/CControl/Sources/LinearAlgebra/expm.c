/*
 * expm.c
 *
 *  Created on: 8 août 2020
 *      Author: Daniel Mårtensson
 */

#include "linearalgebra.h"

/*
 * Find matrix exponential, return A as A = expm(A)
 * A[m*n]
 * m == n
 */
bool expm(float A[], const size_t row){
	/* Decleration */
	size_t i;

	/* Create zero matrix */
	size_t row_row_size = row * row * sizeof(float);
	float *E = (float*)malloc(row_row_size);
	memset(E, 0, row_row_size);
	/* Create identity matrices */
	float *F = (float*)malloc(row_row_size);
	float *T = (float*)malloc(row_row_size);
	memset(F, 0, row_row_size);
	memset(T, 0, row_row_size);
	for(i = 0; i < row; i++){
		F[i * row + i] = 1;
		T[i * row + i] = 1;
	}
	float k = 1.0f;
	size_t iterations = 0;
	while (norm(T, row, row, NORM_METHOD_L1) > 0.0f && iterations < MAX_ITERATIONS){
		/* E = E + F */
		for(i = 0; i < row*row; i++){
			E[i] = E[i] + F[i];
		}
		/* F = A*F/k (we are borrowing T) */
		mul(A, F, T, row, row, row);
		for(i = 0; i < row*row; i++){
			F[i] = T[i]/k;
		}
		/* T = E + F - E ( This is faster than T = F) */
		for(i = 0; i < row*row; i++){
			T[i] = E[i] + F[i] - E[i];
		}
		k++;
		iterations++;
	}
	memcpy(A, E, row_row_size);

	/* Free */
	free(E);
	free(F);
	free(T);

	/* Return status */
	return iterations < MAX_ITERATIONS;
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


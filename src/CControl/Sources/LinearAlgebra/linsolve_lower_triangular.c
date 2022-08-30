/*
 * linsolve_lower_triangular.c
 *
 *  Created on: 2 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Solve with forward substitution. This can be used with Cholesky decomposition
 * A [m*n] need to be lower triangular and square
 * b [m]
 * x [n]
 * n == m
 */
void linsolve_lower_triangular(float A[], float x[], float b[], uint16_t row) {
	// Save address
	float *A0 = &A[0];

	// Time to solve x from Ax = b.
	memset(x, 0, row*sizeof(float));
	float sum;
	for (uint16_t i = 0; i < row; i++) {
		sum = 0.0f;
		for (uint16_t j = 0; j < i; j++)
			sum += A0[j] * x[j];
			//sum = sum + A[row*i + j] * x[j];

		x[i] = (b[i] - sum) / A[row*i + i];
		A0 += row;
	}
}


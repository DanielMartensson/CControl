/*
 * linsolve_lower_triangular.c
 *
 *  Created on: 2 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Configurations.h"
#include "../../Headers/Functions.h"

/*
 * Solve with forward substitution. This can be used with Cholesky decomposition
 * A [m*n] need to be lower triangular and square e.g cholesky L
 * b [m]
 * x [n]
 * n == m
 */
void linsolve_lower_triangular(float *A, float *x, float *b, int row) {
	// Time to solve x from Ax = b.
	memset(x, 0, row*sizeof(float));
	float sum;
	for (int i = 0; i < row; i++) {
		sum = 0;
		for (int j = 0; j < i; j = j + 1){
			sum = sum + *(A + row*i + j) * *(x + j);
		}
		*(x + i) = (*(b + i) - sum) / *(A + row*i + i);
	}
}


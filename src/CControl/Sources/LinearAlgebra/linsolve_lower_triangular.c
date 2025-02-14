/*
 * linsolve_lower_triangular.c
 *
 *  Created on: 2 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "linearalgebra.h"

/*
 * Solve with forward substitution. This can be used with Cholesky decomposition
 * A [m*n] need to be lower triangular and square
 * b [m]
 * x [n]
 * n == m
 */
void linsolve_lower_triangular(const float A[], float x[], const float b[], const size_t row) {
	/* Time to solve x from Ax = b */
	memset(x, 0, row * sizeof(float));
	size_t i;
	for (i = 0; i < row; i++) {
		const float s = dot(A, x, i);
		x[i] = (b[i] - s) / A[i];
		A += row;
	}
}


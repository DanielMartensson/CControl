/*
 * linsolve_chol.c
 *
 *  Created on: 10 juni 2021
 *      Author: Daniel Mårtensson
 */


#include "../../Headers/Functions.h"

/*
 * This solves Ax=b with Cholesky decomposition
 * A [m*n] (symmetric and positive definite)
 * x [n]
 * b [m]
 * n == m
 */
void linsolve_chol(float A[], float x[], float b[], uint16_t row) {
	float L[row*row];
	chol(A, L, row);
	float y[row];
	linsolve_lower_triangular(L, y, b, row);
	tran(L, row, row);
	linsolve_upper_triangular(L, x, y, row);
}

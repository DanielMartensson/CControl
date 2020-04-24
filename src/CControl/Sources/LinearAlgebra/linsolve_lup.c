/*
 * linsolve_lup.c
 *
 *  Created on: 2 mars 2020
 *      Author: hp
 */

#include "../../Headers/Configurations.h"
#include "../../Headers/Functions.h"

/*
 * This solves Ax=b with LUP-decomposition
 * LU [m*n]
 * P [n]
 * b [m]
 * n == m
 */
int linsolve_lup(float *A, float *x, float *b, int row) {

	float LU[row*row];
	int P[row];
	int status = lup(A, LU, P,row);

	// forward substitution with pivoting
	for (int i = 0; i < row; ++i) {
		*(x + i) = *(b + *(P + i));

		for (int j = 0; j < i; ++j)
			*(x + i) = *(x + i) - *(LU + row * *(P + i) + j) * *(x + j);
	}

	// backward substitution with pivoting
	for (int i = row - 1; i >= 0; --i) {
		for (int j = i + 1; j < row; ++j)
			*(x + i) = *(x + i) - *(LU + row * *(P + i) + j) * *(x + j);

		*(x + i) = *(x + i) / *(LU + row * *(P + i) + i);
	}

	return status;
}

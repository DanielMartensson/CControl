/*
 * linsolve_lup.c
 *
 *  Created on: 2 mars 2020
 *      Author: hp
 */

#include "../../Headers/Functions.h"

/*
 * This solves Ax=b with LUP-decomposition
 * A [m*n]
 * x [n]
 * b [m]
 * n == m
 * Returns 1 == Success
 * Returns 0 == Fail
 */
uint8_t linsolve_lup(float *A, float *x, float *b, int row) {

	float LU[row*row];
	int P[row];
	uint8_t status = lup(A, LU, P,row);
	if(status == 0)
		return 0;

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

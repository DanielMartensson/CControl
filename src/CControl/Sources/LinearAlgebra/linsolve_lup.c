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
uint8_t linsolve_lup(float A[], float x[], float b[], uint16_t row) {

	float LU[row*row];
	uint8_t P[row];
	uint8_t status = lup(A, LU, P,row);
	if(status == 0)
		return 0;

	// forward substitution with pivoting
	for (uint16_t i = 0; i < row; ++i) {
		x[i] = b[P[i]];

		for (uint16_t j = 0; j < i; ++j)
			x[i] = x[i] - LU[row * P[i] + j] * x[j];
	}

	// backward substitution with pivoting
	for (uint16_t i = row - 1; i >= 0; --i) {
		for (uint16_t j = i + 1; j < row; ++j)
			x[i] = x[i] - LU[row * P[i] + j] * x[j];

		x[i] = x[i] / LU[row * P[i] + i];

		// Important because when i = -1, then i will become 65535
		if(i == 0)
			break;
	}

	return status;
}

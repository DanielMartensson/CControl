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
	/* Decleration */
	int32_t i, j;

	float *LU = (float*)malloc(row * row * sizeof(float));
	uint8_t *P = (uint8_t*)malloc(row * sizeof(uint8_t));
	uint8_t status = lup(A, LU, P,row);
	if(status == 0)
		return 0;

	/* forward substitution with pivoting */
	for (i = 0; i < row; ++i) {
		x[i] = b[P[i]];
		for (j = 0; j < i; ++j) {
			x[i] = x[i] - LU[row * P[i] + j] * x[j];
		}
	}

	/* backward substitution with pivoting */
	for (i = row - 1; i >= 0; --i) {
		for (j = i + 1; j < row; ++j) {
			x[i] = x[i] - LU[row * P[i] + j] * x[j];
		}
		x[i] = x[i] / LU[row * P[i] + i];
	}

	/* Free */
	free(LU);
	free(P);

	return status;
}

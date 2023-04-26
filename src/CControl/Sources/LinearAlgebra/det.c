/*
 * det.c
 *
 *  Created on: 2 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Computes the determinant of square matrix A
 * A [m*n]
 * n == m
 * Return: Determinant value, or 0 for singular matrix
 */
float det(float A[], uint16_t row) {
	/* Decleration */
	uint16_t i;

	float determinant = 1.0;
	float *LU = (float*)malloc(row * row * sizeof(float));
	uint8_t *P = (uint8_t*)malloc(row * row * sizeof(uint8_t));
	uint8_t status = lup(A, LU, P, row);
	if (status == 0) {
		return 0; /* matrix is singular */
	}

	for (i = 0; i < row; ++i) {
		determinant *= LU[row * P[i] + i];
	}

	uint16_t j = 0;
	for (i = 0; i < row; ++i) {
		if (P[i] != i) {
			++j;
		}
	}

	if (j && (j - 1) % 2 == 1) {
		determinant = -determinant;
	}

	/* Free */
	free(LU);
	free(P);

	return determinant;
}

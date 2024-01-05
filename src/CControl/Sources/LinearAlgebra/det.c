/*
 * det.c
 *
 *  Created on: 2 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "linearalgebra.h"

/*
 * Computes the determinant of square matrix A
 * A [m*n]
 * n == m
 * Return: Determinant value, or 0 for singular matrix
 */
float det(float A[], size_t row) {
	/* Decleration */
	size_t i;

	float determinant = 1.0f;
	float *LU = (float*)malloc(row * row * sizeof(float));
	int *P = (int*)malloc(row * row * sizeof(int));
	bool ok = lup(A, LU, P, row);
	if (!ok) {
		return 0.0f; /* matrix is singular */
	}

	/* Special case for LAPACK */
#if defined(CLAPACK_USED) || defined(MKL_LAPACK_USED)
	for (i = 0; i < row; i++) {
		P[i] -= 1;
	}
#endif

	for (i = 0; i < row; ++i) {
		determinant *= LU[row * P[i] + i];
	}

	size_t j = 0;
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

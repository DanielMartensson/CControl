/*
 * lup.c
 *
 *  Created on: 2 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

 /* Include LAPACK routines */
#ifdef LAPACK_ANSI_C_USED
#include "Lapack/lapack.h"
#endif

/*
 * Do LU-decomposition with partial pivoting
 * A [m*n]
 * LU [m*n]
 * P [n]
 * n == m
 * Returns true == Success
 * Returns false == Fail
 */
bool lup(float A[], float LU[], size_t P[], size_t row) {
#ifdef LAPACK_ANSI_C_USED
	integer m = row, lda = row, n = row;
	size_t rowrow = row * row;
	memcpy(LU, A, rowrow * sizeof(float));
	integer* ipiv = (integer*)malloc(row * sizeof(integer));
	real* a = (real*)malloc(rowrow * sizeof(real));
	integer info;
	size_t i, j;
	/* This is transpose */
	real* a0 = a;
	for (i = 0; i < row; i++) {
		a = a0;
		a += i;
		for (j = 0; j < row; j++) {
			a[0] = A[j];
			a += row;
		}
		A += row;
	}
	a = a0;
	sgetrf_(&m, &n, a, &lda, ipiv, &info);
	/* This is transpose */
	for (i = 0; i < row; i++) {
		a = a0;
		a += i;
		for (j = 0; j < row; j++) {
			LU[j] = a[0];
			a += row;
		}
		LU += row;

		/* Pivots */
		P[i] = ipiv[i] - 1U;
	}

	/* Reset */
	a = a0;

	free(ipiv);
	free(a);
	return info == 0 ? true : false;
#else
	/* Variables */
	size_t ind_max, tmp_int;

	/* If not the same */
	if (A != LU) {
		memcpy(LU, A, row * row * sizeof(float));
	}

	/* Create the pivot vector */
	size_t i, j, k;
	for (i = 0; i < row; ++i) {
		P[i] = i;
	}

	for (i = 0; i < row - 1; ++i) {
		ind_max = i;
		for (j = i + 1; j < row; ++j) {
			if (fabsf(LU[row * P[j] + i]) > fabsf(LU[row * P[ind_max] + i])) {
				ind_max = j;
			}
		}

		tmp_int = P[i];
		P[i] = P[ind_max];
		P[ind_max] = tmp_int;

		if (fabsf(LU[row * P[i] + i]) < MIN_VALUE) {
			return false; /* matrix is singular (up to tolerance) */
		}

		for (j = i + 1; j < row; ++j) {
			LU[row * P[j] + i] = LU[row * P[j] + i] / LU[row * P[i] + i];

			for (k = i + 1; k < row; ++k) {
				LU[row * P[j] + k] = LU[row * P[j] + k] - LU[row * P[i] + k] * LU[row * P[j] + i];
			}
		}
	}
	return true; /* Solved */
#endif
}


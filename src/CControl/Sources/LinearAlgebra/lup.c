/*
 * lup.c
 *
 *  Created on: 2 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "linearalgebra.h"

 /* Include LAPACK routines */
#ifdef CLAPACK_USED
#include "CLapack/clapack.h"
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
bool lup(float A[], float LU[], int P[], size_t row) {
#ifdef CLAPACK_USED
	integer m = row, lda = row, n = row, info;
	memcpy(LU, A, row * row * sizeof(float));
	sgetrf_(&m, &n, LU, &lda, P, &info);
	return info == 0;
#elif defined(MKL_LAPACK_USED)
	memcpy(LU, A, row * row * sizeof(float));
	bool status = LAPACKE_sgetrf(LAPACK_COL_MAJOR, row, row, LU, row, P) == 0;
	/* Return status */
	return status;
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


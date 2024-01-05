/*
 * chol.c
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
 * Create A = L*L^T
 * A need to be symmetric positive definite
 * A [m*n]
 * L [m*n]
 * n == m
 */
bool chol(float A[], float L[], size_t row) {
#ifdef CLAPACK_USED
	integer info;
	integer n = row, lda = row;
	memcpy(L, A, row * row * sizeof(float));
	spotrf_("U", &n, L, &lda, &info);
	size_t i, j;
	/* Turn to lower matrix */
	for (i = 0; i < row; i++) {
		for (j = 0; j < row; j++) {
			if (j > i) {
				L[i * row + j] = 0.0f;
			}
		}
	}
	return info == 0;
#elif defined(MKL_LAPACK_USED)
	memcpy(L, A, row * row * sizeof(float));
	bool status = LAPACKE_spotrf(LAPACK_ROW_MAJOR, 'L', row, L, row) == 0;
	/* Turn to lower matrix */
	size_t i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < row; j++) {
			if (j > i) {
				L[i * row + j] = 0.0f;
			}
		}
	}
	return status;
#else
	/* Save address */
	float* Li = L;
	float* Lj;
	float* Ai = A;

	memset(L, 0, row * row * sizeof(float));
	float s;
	size_t i, j, k;
	for (i = 0; i < row; i++) {
		Lj = L;
		for (j = 0; j <= i; j++) {
			s = 0.0f;
			for (k = 0; k < j; k++) {
				s += Li[k] * Lj[k]; /* s += L[row * i + k] * L[row * j + k]; */
			}

			/* We cannot divide with zero L[row * j + j] */
			if (fabsf(Lj[j]) < MIN_VALUE) {
				Lj[j] = FLT_EPSILON;
				/* L[row * j + j] = FLT_EPSILON; // Same as eps command in MATLAB */
			}
			Li[j] = (i == j) ? sqrtf(Ai[i] - s) : (1.0f / Lj[j] * (Ai[j] - s));
			/* L[row * i + j] = (i == j) ? sqrtf(A[row * i + i] - s) : (1.0f / L[row * j + j] * (A[row * i + j] - s)); */
			Lj += row;
		}
		Li += row;
		Ai += row;
	}
#endif
	return true;
}
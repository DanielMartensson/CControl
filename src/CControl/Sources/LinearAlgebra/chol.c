/*
 * chol.c
 *
 *  Created on: 2 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

 /* Include LAPACK routines */
#ifdef LAPACK_USED
#include "Lapack/lapack.h"
#endif

/*
 * Create A = L*L^T
 * A need to be symmetric positive definite
 * A [m*n]
 * L [m*n]
 * n == m
 */
bool chol(float A[], float L[], size_t row) {
#ifdef LAPACK_USED
	integer info;
	integer n = row, lda = row;
	float* Acopy = (float*)malloc(row * row * sizeof(float));
	memcpy(Acopy, A, row * row * sizeof(float));
	spotrf_("U", &n, Acopy, &lda, &info);
	size_t i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < row; j++) {
			if (i < j) {
				Acopy[i * row + j] = 0.0f;
			}
		}
	}
	memcpy(L, Acopy, row * row * sizeof(float));
	return info == 0 ? true : false;
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
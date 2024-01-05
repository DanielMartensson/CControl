/*
 * linsolve_lup.c
 *
 *  Created on: 2 mars 2020
 *      Author: hp
 */

#include "linearalgebra.h"

 /* Include LAPACK routines */
#ifdef CLAPACK_USED
#include "CLapack/clapack.h"
#endif

/*
 * This solves Ax=b with LUP-decomposition
 * A [m*n]
 * x [n]
 * b [m]
 * n == m
 * Returns true == Success
 * Returns false == Fail
 */
bool linsolve_lup(float A[], float x[], float b[], size_t row) {
	float* LU = (float*)malloc(row * row * sizeof(float));
	int* P = (int*)malloc(row * sizeof(int));
	bool status = lup(A, LU, P, row);
#ifdef CLAPACK_USED
	integer n = row, nrhs = 1, lda = row, ldb = row, info;
	float* bcopy = (float*)malloc(ldb * sizeof(float));
	memcpy(bcopy, b, ldb * sizeof(float));
	sgetrs_("T", &n, &nrhs, LU, &lda, P, bcopy, &ldb, &info);
	memcpy(x, bcopy, n * sizeof(real));
	free(bcopy);
	status = (info == 0) && status;
#elif defined(MKL_LAPACK_USED)
	float* bcopy = (float*)malloc(row * sizeof(float));
	memcpy(bcopy, b, row * sizeof(float));
	status = LAPACKE_sgetrs(LAPACK_COL_MAJOR, 'T', row, 1, LU, row, P, bcopy, row) == 0;
	memcpy(x, bcopy, row * sizeof(float));
	free(bcopy);
#else
	/* Decleration */
	int32_t i, j;

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
#endif

	/* Free */
	free(LU);
	free(P);

	/* Return status */
	return status;
}

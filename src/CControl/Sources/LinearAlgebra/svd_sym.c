/*
 * svd_sym.c
 *
 *  Created on: 30 Juli 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

 /* Include LAPACK routines */
#include "Lapack/lapack.h"

/*
 * This uses LAPACK routine SSYEVD to compute SVD of a symmetric matrix A.
 * The matrix A will contain the eigenvectors as U and V does.
 * A [m*n]
 * U [m*m]
 * S [n]
 * V [n*n]
 * n == m
 * Return true = Success
 * Return false = fail
 */
bool svd_sym(float A[], size_t row, float U[], float S[], float V[]) {
	/* Settings for ssyevd routine */
	char jobz = 'V';
	char uplo = 'U';
	integer n = row;
	real* a = A;
	integer lda = row;
	real* w = (float*)malloc(row * sizeof(float));
	real work_optimization;
	real* work;
	integer lwork = -1;
	integer iwork_optimization;
	integer* iwork;
	integer liwork = -1;
	integer info;

	/* Query and allocate the optimal workspace */
	ssyevd_(&jobz, &uplo, &n, a, &lda, w, &work_optimization, &lwork, &iwork_optimization, &liwork, &info);

	/* Allocate memory */
	lwork = (integer)work_optimization;
	work = (real*)malloc(lwork * sizeof(real));
	liwork = iwork_optimization;
	iwork = (integer*)malloc(liwork * sizeof(integer));

	/* Solve */
	ssyevd_(&jobz, &uplo, &n, a, &lda, w, work, &lwork, iwork, &liwork, &info);

	/* Move over w to S */
	size_t i;
	for (i = 0; i < row; i++) {
		S[i] = w[row-i-1];
	}

	/* Move over a to U and V */
	size_t j, s = 0;
	for (i = 0; i < row; i++) {
		for (j = 0; j < row; j++) {
			U[j * row + row - i - 1] = a[s++];
		}
	}
	memcpy(V, U, row * row * sizeof(float));

	/* Free */
	free(w);
	free(work);
	free(iwork);

	/* Return status */
	return info == 0 ? true : false;
}
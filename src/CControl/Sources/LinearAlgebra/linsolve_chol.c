/*
 * linsolve_chol.c
 *
 *  Created on: 10 juni 2021
 *      Author: Daniel Mårtensson
 */

#include "linearalgebra.h"

 /* Include LAPACK routines */
#ifdef CLAPACK_USED
#include "CLapack/clapack.h"
#endif

/*
 * This solves Ax=b with Cholesky decomposition
 * A [m*n] (symmetric and positive definite)
 * x [n]
 * b [m]
 * n == m
 * return true = success
 * return false = fail
 */
bool linsolve_chol(float A[], float x[], float b[], size_t row) {
#ifdef CLAPACK_USED
	/* Settings */
	integer n = row, nrhs = 1, lda = row, ldb = row, info;

	/* Copy over */
	float* Acopy = (float*)malloc(row * row * sizeof(float));
	memcpy(Acopy, A, row * row * sizeof(float));
	memcpy(x, b, row * sizeof(float));

	/* Solve */
	sposv_("U", &n, &nrhs, Acopy, &lda, x, &ldb, &info);
	
	/* Free */
	free(Acopy);

	/* Return status */
	return info == 0;
#elif defined(MKL_LAPACK_USED)
	/* Copy over */
	float* Acopy = (float*)malloc(row * row * sizeof(float));
	memcpy(Acopy, A, row * row * sizeof(float));
	memcpy(x, b, row * sizeof(float));

	/* Solve */
	bool status = LAPACKE_sposv(LAPACK_COL_MAJOR, 'U', row, 1, Acopy, row, x, row) == 0;

	/* Free */
	free(Acopy);

	/* Return status */
	return status;
#else
	float* L = (float*)malloc(row * row * sizeof(float));
	bool status = chol(A, L, row);
	float* y = (float*)malloc(row * sizeof(float));
	linsolve_lower_triangular(L, y, b, row);
	tran(L, row, row);
	linsolve_upper_triangular(L, x, y, row);
	free(L);
	free(y);

	/* Return status */
	return status;
#endif
}

/*
 * inv.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

static bool solve(float x[], float b[], size_t P[], float LU[], size_t row);

/* Include LAPACK routines */
#ifdef LAPACK_USED
#include "Lapack/lapack.h"
#endif

/*
 * A to A^(-1)
 * Notice that only square matrices are only allowed.
 * Finding inverse should only be used for last solution.
 * Finding inverse is very cost expensive. Better to solve Ax=b instead
 * A[m*n]
 * n == m
 * Returns true == Success
 * Returns false == Fail
 */
bool inv(float A[], size_t row) {
#ifdef LAPACK_USED
	/* First use LU factorization */
	integer m = row, n = row, lda = row, info, lwork;
	integer* ipiv = (integer*)malloc(row * sizeof(integer));
	sgetrf_(&m, &n, A, &lda, ipiv, &info);

	/* Allocate memory */
	real wkopt;
	lwork = -1;
	sgetri_(&m, A, &lda, ipiv, &wkopt, &lwork, &info);
	lwork = (integer)wkopt;
	real* work = (real*)malloc(lwork * sizeof(real));

	/* Solve */
	sgetri_(&m, A, &lda, ipiv, work, &lwork, &info);

	/* Return status */
	return info == 0 ? true : false;
#else
	/* Create iA matrix */
	float *iA = (float*)malloc(row * row * sizeof(float));
	float *A0 = iA; 

	/* Create temporary matrix */
	float *tmpvec = (float*)malloc(row * sizeof(float));
	memset(tmpvec, 0, row*sizeof(float));

	/* Check if the determinant is 0 */
	float *LU = (float*)malloc(row * row * sizeof(float));
	size_t *P = (size_t*)malloc(row * sizeof(size_t));
	bool ok = lup(A, LU, P, row);
    if (ok) {
        /* Create the inverse */
        size_t i;
        for (i = 0; i < row; i++) {
            tmpvec[i] = 1.0f;
            if (!solve(iA, tmpvec, P, LU, row)) {
                ok = false;
                break;
            }
            tmpvec[i] = 0.0f;
			iA += row;
        }
        if (ok) {
            /* Transpose of iA */
			iA = A0; /* Reset position */
            tran(iA, row, row);

            /* Copy over iA -> A */
            memcpy(A, iA, row * row * sizeof(float));
        }
    }
	/* Free */
    free(tmpvec);
	free(iA);
	free(LU);
	free(P);

	return ok;
#endif
}

static bool solve(float x[], float b[], size_t P[], float LU[], size_t row){
	/* forward substitution with pivoting */
	int32_t i, j;
	for (i = 0; i < row; ++i) {
		x[i] = b[P[i]];
		for (j = 0; j < i; ++j) {
			x[i] = x[i] - LU[row * P[i] + j] * x[j];
		}
	}

	/* backward substitution with pivoting */
	for (i = row - 1; i >= 0; --i) {
		for (j = i + 1; j < row; ++j)
			x[i] = x[i] - LU[row * P[i] + j] * x[j];
		
		/* Just in case if we divide with zero */
		if (fabsf(LU[row * P[i] + i]) > MIN_VALUE) {
			x[i] = x[i] / LU[row * P[i] + i];
		}
		else {
			return false;
		}
	}
	
	return true; /* No problems */
}

/*
 * GNU Octave code:
 *   >> A = [3 4 5; 2 5 6; 5 6 7];
	 >> inv(A)
	 ans =

	 0.25000  -0.50000   0.25000
	 -4.00000   1.00000   2.00000
	 3.25000  -0.50000  -1.75000

	 >>
 */

/*
 * tran.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

 /* Include LAPACK routines */
#ifdef CLAPACK_USED
#include "CLapack/clapack_functions.h"
#endif

/*
 * Turn A into transponse A^T
 */
void tran(float A[], size_t row, size_t column) {
#ifdef MKL_USED
	/* Create identy matrix */
	float* I = (float*)malloc(row * row * sizeof(float));
	memset(I, 0, row * row * sizeof(float));
	float* I0 = I;
	size_t i;
	for (i = 0; i < row; i++) {
		I0[i] = 1.0f;
		I0 += row;
	}

	/* Create output matrix */
	float* At = (float*)malloc(column * row * sizeof(float));

	/* Compute At = alpha*A'*I + beta*At */
	cblas_sgemm(CblasRowMajor, CblasTrans, CblasNoTrans, column, row, row, 1.0f, A, column, I, row, 0.0f, At, row);

	/* Copy over */
	memcpy(A, At, column * row * sizeof(float));

	/* Free */
	free(I);
	free(At);
#else
	/* Decleration */
	float *B = (float*)malloc(row * column * sizeof(float));
	float* transpose = NULL;
	float *ptr_A = A;
	size_t i, j;

	for (i = 0; i < row; i++) {
		transpose = &B[i];
		for (j = 0; j < column; j++) {
			*transpose = *ptr_A;
			ptr_A++;
			transpose += row;
		}
	}

	/* Copy! */
	memcpy(A, B, row*column*sizeof(float));

	/* Free */
	free(B);
#endif
}

/*
 * GNU Octave code:
 * >> A = [4 23 5; 2 45 5]
	A =

		4   23    5
		2   45    5

	>> A'
	ans =

		4    2
	   23   45
		5    5

	>>
 */

/*
 * mul.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "linearalgebra.h"

 /* Include LAPACK routines */
#ifdef CLAPACK_USED
#include "CLapack/clapack.h"
#endif

/*
 * C = A*B
 * A [row_a*column_a]
 * B [column_a*column_b]
 * C [row_a*column_b]
 */
void mul(const float A[], const float B[], float C[], const size_t row_a, const size_t column_a, const size_t column_b) {
#ifdef CLAPACK_USED
	/* Check if A is symmetric */
	if (issymmetric(A, row_a, column_a)) {
		/* Check if it's vector */
		if (column_b == 1U) {
			real alpha = 1.0f, beta = 0.0f;
			integer m = row_a, n = column_a, lda = row_a, incx = 1, incy = 1;
			ssymv_("U", &n, &alpha, A, &lda, B, &incx, &beta, C, &incy);
		}
		else {
			real alpha = 1.0f, beta = 0.0f;
			integer m = row_a, n = column_b, lda = row_a, ldb = row_a, ldc = row_a;
			/* Copy B with transpose */
			float* Bcopy = (float*)malloc(column_a * column_b * sizeof(float));
			float* Bcopy0 = Bcopy;
			size_t i, j;
			for (i = 0; i < column_a; i++) {
				Bcopy = Bcopy0;
				Bcopy += i;
				for (j = 0; j < column_b; j++) {
					Bcopy[0] = B[j];
					Bcopy += column_a;
				}
				B += column_b;
			}
			Bcopy = Bcopy0;
			ssymm_("L", "U", &m, &n, &alpha, A, &lda, Bcopy, &ldb, &beta, C, &ldc);
			tran(C, column_b, row_a);
			free(Bcopy);
		}
	}
	else {
		/* Copy A with transpose */
		float* Acopy = (float*)malloc(row_a * column_a * sizeof(float));
		float* Acopy0 = Acopy;
		size_t i, j;
		for (i = 0; i < row_a; i++) {
			Acopy = Acopy0;
			Acopy += i;
			for (j = 0; j < column_a; j++) {
				Acopy[0] = A[j];
				Acopy += row_a;
			}
			A += column_a;
		}
		Acopy = Acopy0;

		/* Vector or matrix for B */
		if (column_b == 1U) {
			real alpha = 1.0f, beta = 0.0f;
			integer m = row_a, n = column_a, lda = row_a, incx = 1, incy = 1;
			sgemv_("N", &m, &n, &alpha, Acopy, &lda, B, &incx, &beta, C, &incy);
		}
		else {
			/* Copy B with transpose */
			float* Bcopy = (float*)malloc(column_a * column_b * sizeof(float));
			float* Bcopy0 = Bcopy;
			for (i = 0; i < column_a; i++) {
				Bcopy = Bcopy0;
				Bcopy += i;
				for (j = 0; j < column_b; j++) {
					Bcopy[0] = B[j];
					Bcopy += column_a;
				}
				B += column_b;
			}
			Bcopy = Bcopy0;
			real alpha = 1.0f, beta = 0.0f;
			integer m = row_a, n = column_b, k = column_a, lda = row_a, ldb = column_a, ldc = row_a;
			sgemm_("N", "N", &m, &n, &k, &alpha, Acopy, &lda, Bcopy, &ldb, &beta, C, &ldc);
			tran(C, column_b, row_a);
			free(Bcopy);
		}

		/* Free */
		free(Acopy);
	}
#elif defined(MKL_LAPACK_USED)
	/* Check if A is symmetric */
	if (issymmetric(A, row_a, column_a)) {
		/* Check if it's vector */
		if (column_b == 1U) {
			cblas_ssymv(CblasRowMajor, CblasUpper, column_a, 1.0f, A, row_a, B, 1, 0.0f, C, 1);
		}
		else {
			int m = row_a;
			int n = column_b;
			int k = column_a;
			float alpha = 1.0f;
			int lda = m;
			int ldb = n;
			float beta = 0.0f;
			int ldc = n;
			cblas_ssymm(CblasRowMajor, CblasLeft, CblasUpper, m, n, alpha, A, lda, B, ldb, beta, C, ldc);
		}
	}
	else {
		/* Vector or matrix for B */
		if (column_b == 1U) {
			cblas_sgemv(CblasRowMajor, CblasNoTrans, row_a, column_a, 1.0f, A, column_a, B, 1, 0.0f, C, 1);
		}
		else {
			int m = row_a;
			int n = column_b;
			int k = column_a;
			float alpha = 1.0f;
			int lda = k;
			int ldb = n;
			float beta = 0.0f;
			int ldc = n;
			cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, k, alpha, A, lda, B, ldb, beta, C, ldc);
		}
	}
#else
	/* Decleration */
	size_t i, j, k;

	/* Data matrix */
	float* data_a, * data_b;

	for (i = 0; i < row_a; i++) {
		/* Then we go through every column of b */
		for (j = 0; j < column_b; j++) {
			data_a = A;
			data_b = &B[j];

			C[0] = 0.0f; /* Reset */
			/* And we multiply rows from a with columns of b */
			for (k = 0; k < column_a; k++) {
				*C += data_a[0] * data_b[0];
				data_a++;
				data_b += column_b;
			}
			C++; /* ;) */
		}
		A += column_a;
	}
#endif
}

/*
 * GNU Octave code:
 *  >> A = [4 23; 2  5];
	>> B = [3; 1];
	>> C = A*B
	C =

	   35
	   11

	>>
 *
 */



/*
 * eig_generalized.c
 *
 *  Created on: 26 mars 2023
 *      Author: Daniel Mårtensson
 */

#include "linearalgebra.h"

 /* Include LAPACK routines */
#ifdef CLAPACK_USED
#include "CLapack/clapack.h"
#endif

 /*
  * Compute eigenvalues and eigenvectors from matrix A and B
  * A[m*n]
  * B[m*n]
  * n == m
  * A^T = A
  * B^T = B
  * dr[m] = Eigenvalues real
  * di[m] = Eigenvalues imaginary
  * wr[m*n] = Eigenvectors real
  * wi[m*n] = Eigenvectors imaginary
  */
bool eig_generalized(float A[], float B[], size_t row, float dr[], float di[], float wr[], float wi[]) {
#ifdef CLAPACK_USED
	/* Check if it's symmetric */
	bool issymmetricA = issymmetric(A, row, row);
	bool issymmetricB = issymmetric(B, row, row);
	
	if (issymmetricA && issymmetricB) {
		/* Settings */
		integer itype = 1, n = row, lda = row, ldb = row, lwork, iwkopt, liwork, info;
		real wkopt;

		/* Copy */
		float* Bcopy = (float*)malloc(row * row * sizeof(float));
		memcpy(Bcopy, B, row * row * sizeof(float));
		memcpy(wr, A, row * row * sizeof(float));

		/* Allocate memory */
		lwork = -1;
		liwork = -1;
		ssygvd_(&itype, "V", "U", &n, wr, &lda, Bcopy, &ldb, dr, &wkopt, &lwork, &iwkopt, &liwork, &info);

		/* Compute */
		lwork = (integer)wkopt;
		real* work = (real*)malloc(lwork * sizeof(real));
		liwork = iwkopt;
		integer* iwork = (integer*)malloc(liwork * sizeof(integer));
		ssygvd_(&itype, "V", "U", &n, wr, &lda, Bcopy, &ldb, dr, work, &lwork, iwork, &liwork, &info);

		/* Transpose of the eigenvectors */
		tran(wr, row, row);

		/* Free */
		free(Bcopy);
		free(work);
		free(iwork);

		/* Return status */
		return info == 0;
	}
	else {
		/* Settings */
		integer n = row, lda = row, ldb = row, info, lwork;
		real* beta = (real*)malloc(n * sizeof(real));
		real wkopt;

		/* Eigenvectors */
		real* vl = (real*)malloc(n * lda * sizeof(real));
		real* vr = (real*)malloc(n * lda * sizeof(real));

		/* Copy */
		float* Bcopy = (float*)malloc(row * row * sizeof(float));
		memcpy(Bcopy, B, row * row * sizeof(float));
		float* Acopy = (float*)malloc(row * row * sizeof(float));
		memcpy(Acopy, A, row * row * sizeof(float));

		/* Allocate memory */
		lwork = -1;
		sggev_("V", "N", &n, Acopy, &lda, Bcopy, &ldb, dr, di, beta, vl, &n, vr, &n, &wkopt, &lwork, &info);

		/* Compute */
		lwork = (integer)wkopt;
		real* work = (real*)malloc(lwork * sizeof(real));
		sggev_("V", "N", &n, Acopy, &lda, Bcopy, &ldb, dr, di, beta, vl, &n, vr, &n, work, &lwork, &info);

		/* Compute the eigenvalues */
		size_t i;
		for (i = 0; i < row; i++) {
			/* Check if it's a real eigenvalue */
			if (fabsf(di[i]) < MIN_VALUE) {
				dr[i] = dr[i] / beta[i];
				di[i] = 0.0f;
			}
			else {
				dr[i] = dr[i] / beta[i];
				di[i] = di[i] / beta[i];
			}
		}

		/* Fill the eigenvectors in row major */
		size_t j, s = 0, t = 0;
		memset(wi, 0, row * row * sizeof(float));
		for (i = 0; i < row; i++) {
			if (fabsf(di[i]) < MIN_VALUE) {
				s = t;
				for (j = 0; j < row; j++) {
					wr[j * row + i] = vl[s++];
				}
				t = s;
			}
			else {
				t = s;
				for (j = 0; j < row; j++) {
					wr[j * row + i] = vl[t++];
				}
				for (j = 0; j < row; j++) {
					wi[j * row + i] = vl[t++];
				}
			}
		}

		/* Free */
		free(beta);
		free(vl);
		free(vr);
		free(Bcopy);
		free(Acopy);

		/* Return status */
		return info == 0;
	}
#elif defined(MKL_LAPACK_USED)
	/* Check if it's symmetric */
	bool issymmetricA = issymmetric(A, row, row);
	bool issymmetricB = issymmetric(B, row, row);
	
	if (issymmetricA && issymmetricB) {
		float* Bcopy = (float*)malloc(row * row * sizeof(float));
		memcpy(Bcopy, B, row * row * sizeof(float));
		memcpy(wr, A, row * row * sizeof(float));
		bool status = LAPACKE_ssygvd(LAPACK_ROW_MAJOR, 1, 'V', 'U', row, wr, row, Bcopy, row, dr) == 0;
		free(Bcopy);
		return status;
	}
	else {
		/* Eigenvectors */
		float* vl = (float*)malloc(row * row * sizeof(float));
		float* vr = (float*)malloc(row * row * sizeof(float));

		/* Beta */
		float* beta = (float*)malloc(row * sizeof(float));

		/* Copy */
		float* Bcopy = (float*)malloc(row * row * sizeof(float));
		memcpy(Bcopy, B, row* row * sizeof(float));
		float* Acopy = (float*)malloc(row * row * sizeof(float));
		memcpy(Acopy, A, row* row * sizeof(float));

		/* Compute */
		bool status = LAPACKE_sggev(LAPACK_COL_MAJOR, 'V', 'N', row, Acopy, row, Bcopy, row, dr, di, beta, vl, row, vr, row) == 0;

		/* Compute the eigenvalues */
		size_t i;
		for (i = 0; i < row; i++) {
			/* Check if it's a real eigenvalue */
			if (fabsf(di[i]) < MIN_VALUE) {
				dr[i] = dr[i] / beta[i];
				di[i] = 0.0f;
			}
			else {
				dr[i] = dr[i] / beta[i];
				di[i] = di[i] / beta[i];
			}
		}

		/* Fill the eigenvectors in row major */
		size_t j, s = 0, t = 0;
		memset(wi, 0, row * row * sizeof(float));
		for (i = 0; i < row; i++) {
			if (fabsf(di[i]) < MIN_VALUE) {
				s = t;
				for (j = 0; j < row; j++) {
					wr[j * row + i] = vl[s++];
				}
				t = s;
			}
			else {
				t = s;
				for (j = 0; j < row; j++) {
					wr[j * row + i] = vl[t++];
				}
				for (j = 0; j < row; j++) {
					wi[j * row + i] = vl[t++];
				}
			}
		}

		/* Free */
		free(vl);
		free(vr);
		free(beta);
		free(Bcopy);
		free(Acopy);

		/* Return status */
		return status;
	}

#else
	/* Copy B */
	float* Bcopy = (float*)malloc(row * row * sizeof(float));
	memcpy(Bcopy, B, row * row * sizeof(float));

	/* Check determinant */
	if (fabsf(det(Bcopy, row)) < MIN_VALUE) {
		/* Do regularization */
		size_t i;
		float* Bcopy0 = Bcopy;
		for (i = 0; i < row; i++) {
			Bcopy0[i] += FLT_EPSILON;
			Bcopy0 += row;
		}
	}

	/* Take inverse on Bcopy */
	inv(Bcopy, row);

	/* Multiply Bcopy * A = X */
	float* X = (float*)malloc(row * row * sizeof(float));
	mul(Bcopy, A, X, row, row, row);

	/* Do eigendecomposition */
	bool status = eig(X, dr, di, wr, wi, row);

	/* Free */
	free(Bcopy);
	free(X);

	/* Return status */
	return status;
#endif
}


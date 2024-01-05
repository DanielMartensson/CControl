/*
 * qr.c
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
 * Householder QR-decomposition
 * A [m*n]
 * Q [m*m]
 * R [m*n]
 *
 * Returns true == Success
 * Returns false == Fail
 */
bool qr(float A[], float Q[], float R[], size_t row_a, size_t column_a, bool only_compute_R) {
#ifdef CLAPACK_USED
	integer m = row_a, n = column_a, lda = row_a, lwork, info;
	real wkopt;
	real* tau = (real*)malloc((row_a > column_a ? column_a : row_a) * sizeof(real));

	/* Fill R with transpose */
	size_t i, j;
	float* R0 = R;
	for (i = 0; i < row_a; i++) {
		R = R0;
		R += i;
		for (j = 0; j < column_a; j++) {
			R[0] = A[j];
			R += row_a;
		}
		A += column_a;
	}
	R = R0;

	/* Allocate memory */
	lwork = -1;
	sgeqrf_(&m, &n, R, &lda, tau, &wkopt, &lwork, &info);
	lwork = (integer)wkopt;
	real* work = (real*)malloc(lwork * sizeof(real));

	/* Solve */
	sgeqrf_(&m, &n, R, &lda, tau, work, &lwork, &info);

	/* Clean Q */
	if (Q != NULL) {
		memset(Q, 0, row_a * row_a * sizeof(float));
	}

	/* Only compute Q */
	if (!only_compute_R) {
		/* R to Q */
		float* Q0 = Q;
		for (i = 0; i < row_a; i++) {
			Q = Q0;
			Q += i;
			R = R0;
			R += i;
			for (j = 0; j < column_a; j++) {
				if (i > j) {
					Q[0] = R[0];
				}
				else {
					break;
				}
				Q += row_a;
				R += row_a;
			}
		}
		Q = Q0;
		R = R0;

		/* Compute Q */
		if (m <= n) {
			/* Allocate */
			lwork = -1;
			sorgqr_(&m, &m, &m, Q, &m, tau, &wkopt, &lwork, &info);
			lwork = (integer)wkopt;
			free(work);
			work = (real*)malloc(lwork * sizeof(real));

			/* Solve */
			sorgqr_(&m, &n, &m, Q, &m, tau, work, &lwork, &info);
		}
		else {
			/* Allocate */
			lwork = -1;
			sorgqr_(&m, &n, &n, Q, &m, tau, &wkopt, &lwork, &info);
			lwork = (integer)wkopt;
			free(work);
			work = (real*)malloc(lwork * sizeof(real));

			/* Solve */
			sorgqr_(&m, &n, &n, Q, &m, tau, work, &lwork, &info);
		}

		/* Swap with transpose */
		tran(Q, row_a, row_a);
	}

	/* Swap with transpose */
	tran(R, column_a, row_a);

	/* Clean R */
	for (i = 0; i < row_a; i++) {
		for (j = 0; j < column_a; j++) {
			if (i > j) {
				R[j] = 0.0f;
			}
			else {
				break;
			}
		}
		R += column_a;
	}

	/* Free */
	free(work);
	free(tau);

	/* Return status */
	return info == 0;
#elif defined(MKL_LAPACK_USED)
	/* Fill R with transpose */
	size_t i, j;
	float* R0 = R;
	for (i = 0; i < row_a; i++) {
		R = R0;
		R += i;
		for (j = 0; j < column_a; j++) {
			R[0] = A[j];
			R += row_a;
		}
		A += column_a;
	}
	R = R0;

	/* Create settings */
	float* tau = (float*)malloc((row_a > column_a ? column_a : row_a) * sizeof(float));

	/* Find R matrix */
	bool status = LAPACKE_sgeqrf(LAPACK_COL_MAJOR, row_a, column_a, R, row_a, tau) == 0;

	/* Clean Q */
	if (Q != NULL) {
		memset(Q, 0, row_a * row_a * sizeof(float));
	}

	/* Only compute Q */
	if (!only_compute_R) {
		/* R to Q */
		float* Q0 = Q;
		for (i = 0; i < row_a; i++) {
			Q = Q0;
			Q += i;
			R = R0;
			R += i;
			for (j = 0; j < column_a; j++) {
				if (i > j) {
					Q[0] = R[0];
				}
				else {
					break;
				}
				Q += row_a;
				R += row_a;
			}
		}
		Q = Q0;
		R = R0;

		if (row_a <= column_a) {
			status = LAPACKE_sorgqr(LAPACK_COL_MAJOR, row_a, column_a, row_a, Q, row_a, tau) == 0;
		}
		else {
			status = LAPACKE_sorgqr(LAPACK_COL_MAJOR, row_a, column_a, column_a, Q, row_a, tau) == 0;
		}

		/* Swap with transpose */
		tran(Q, row_a, row_a);
	}

	/* Swap with transpose */
	tran(R, column_a, row_a);

	/* Clean R */
	for (i = 0; i < row_a; i++) {
		for (j = 0; j < column_a; j++) {
			if (i > j) {
				R[j] = 0.0f;
			}
			else {
				break;
			}
		}
		R += column_a;
	}

	/* Free */
	free(tau);

	/* Return status */
	return status;
#else
	/* Declare */
	size_t row_a_row_a = row_a*row_a;
	size_t l = row_a - 1 < column_a ? row_a - 1 : column_a;
	size_t i, k;
	float s, Rk, r;
	float *W = (float*)malloc(row_a * sizeof(float));
	float *WW = (float*)malloc(row_a_row_a * sizeof(float));
	float *Hi = (float*)malloc(row_a_row_a * sizeof(float));
	float *H = (float*)malloc(row_a_row_a * sizeof(float));
	float *HiH = (float*)malloc(row_a_row_a * sizeof(float));
	float *HiR = (float*)malloc(row_a * column_a * sizeof(float)); 

	/* Give A to R */
	memcpy(R, A, row_a*column_a*sizeof(float));

	/* Save address */
	float *Ri, *kR, *iH, *iHi;

	/* Turn H into identity matrix */
	memset(H, 0, row_a_row_a*sizeof(float));
	iH = H;
	for(i = 0; i < row_a; i++){
		iH[i] = 1.0f;
		iH += row_a;
		/* H[i*row_a + i] = 1.0f; */
	}

	/* Do house holder transformations */
	kR = R;
	for(k = 0; k < l; k++){
		/* Do L2 norm */
		s = 0.0f;
		Ri = R;
		Ri += column_a*k;
		for(i = k; i < row_a; i++){
			s += Ri[k] * Ri[k];
			Ri += column_a;
			/* s += R[i*column_a + k] * R[i*column_a + k]; */
		}
		s = sqrtf(s);

		/* Find Rk */
		Rk = kR[k];
		kR += column_a;
		/* Rk = R[k*column_a + k]; */

		/* Do sign */
		if (Rk < 0.0f) {
			s = -s;
		}

		/* Compute r */
		r = sqrtf(2 * s * (Rk + s));

		/* Fill W */
		memset(W, 0, row_a*sizeof(float));
		W[k] = (Rk + s) / r;
		Ri = R;
		Ri += column_a*k;
		for(i = k+1; i < row_a; i++){
			Ri += column_a;
			W[i] = Ri[k] / r;
			/* W[i] = R[i*column_a + k] / r; */
		}

		/* WW = W*W' */
		mul(W, W, WW, row_a, 1, row_a);

		/* Fill Hi matrix */
		for (i = 0; i < row_a_row_a; i++) {
			Hi[i] = -2.0f * WW[i];
		}

		/* Use identity matrix on Hi */
		iHi = Hi;
		for(i = 0; i < row_a; i++){
			iHi[i] += 1.0f;
			iHi += row_a;
			/* Hi[i*row_a + i] += 1.0f; */
		}

		/* HiH = Hi * H -> HiH = H */
		if(!only_compute_R) {
			mul(Hi, H, HiH, row_a, row_a, row_a);
			memcpy(H, HiH, row_a_row_a*sizeof(float));
		}

		/* HiR = Hi * R -> HiR = R */
		mul(Hi, R, HiR, row_a, row_a, column_a);
		memcpy(R, HiR, row_a*column_a*sizeof(float));
	}

	/* Clean Q */
	if (Q != NULL) {
		memset(Q, 0, row_a * row_a * sizeof(float));
	}

	/* Do inverse on H and give it to Q */
	bool ok = true;
	if(!only_compute_R) {
		ok = inv(H, row_a);
		memcpy(Q, H, row_a_row_a*sizeof(float));
	}

	/* Clean R */
	size_t j;
	for (i = 0; i < row_a; i++) {
		for (j = 0; j < column_a; j++) {
			if (i > j) {
				R[j] = 0.0f;
			}
			else {
				break;
			}
		}
		R += column_a;
	}

	/* Free */
	free(W);
	free(WW); 
	free(Hi); 
	free(H); 
	free(HiH);
	free(HiR);

	return ok;
#endif
}

/*
 * GNU Octave code:
 *  function [Q,R] = qr(A)
	  % Perform QR decompostion by using Householder transformation
	  % Input  - A is an m x n matrix
	  % Output - Q, R : QR decomposition results
	  [m,n] = size(A);
	  R = A;
	  H = eye(m);
	  for k = 1:min(m-1,n)
		  s = norm(R(k:m,k));
		  Rk = R(k,k);
		  if (Rk < 0)
			  s=-s;
		  end
		  r = sqrt(2*s*(Rk+s));
		  W = zeros(m,1);
		  W(k) = (Rk+s)/r;
		  W(k+1:m) = R(k+1:m,k)/r;
		  Hi=eye(m)-2*W*W';
		  HiH=Hi*H;
		  H = HiH;
		  HiR = Hi*R;
		  R = HiR;
	  end
	  Q = inv(H);
	end
 */

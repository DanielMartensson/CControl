/*
 * pdist2.c
 *
 *  Created on: 22 juli 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Compute pdist2
 * A[row_a*column_a]
 * B[row_b*column_a]
 * C[row_a*row_b]
 */
void pdist2(float A[], float B[], float C[], size_t row_a, size_t column_a, size_t row_b, PDIST2_METRIC metric) {
	/* Get size of bytes */
	const size_t sizeBytesA = row_a * column_a * sizeof(float);
	const size_t sizeBytesB = row_b * column_a * sizeof(float);
	size_t i, j;
	switch (metric) {
	case PDIST2_METRIC_SQEUCLIDEAN: {
		/* MATLAB: Bt = B' */
		float* Bt = (float*)malloc(sizeBytesB);
		memcpy(Bt, B, sizeBytesB);
		tran(Bt, row_b, column_a);

		/* 
		 * MATLAB: AA = sum(A.*A, 2)
		 */
		float* AtimesA = (float*)malloc(sizeBytesA);
		for (i = 0; i < row_a * column_a; i++) {
			AtimesA[i] = A[i] * A[i];
		}
		float* AA = (float*)malloc(row_a * sizeof(float));
		sum(AtimesA, AA, row_a, column_a, false);

		/* 
		 * MATLAB: BB = sum(Bt*Bt, 1);
		 */
		float* BttimesBt = (float*)malloc(sizeBytesB);
		for (i = 0; i < row_b * column_a; i++) {
			BttimesBt[i] = Bt[i] * Bt[i];
		}
		float* BB = (float*)malloc(row_b * sizeof(float));
		sum(BttimesBt, BB, column_a, row_b, true);

		/* MATLAB: C = AA + BB + 2*A*Bt */
		mul(A, Bt, C, row_a, column_a, row_b);
		for (i = 0; i < row_a; i++) {
			for (j = 0; j < row_b; j++) {
				C[j] = - 2 * C[j];
				C[j] += BB[j];
				C[j] += AA[i];
			}
			C += row_b;
		}

		/* Free */
		free(AtimesA);
		free(BttimesBt);
		free(AA);
		free(BB);
		break;
	}
	case PDIST2_METRIC_L2: {
		pdist2(A, B, C, row_a, column_a, row_b, PDIST2_METRIC_SQEUCLIDEAN);
		for (i = 0; i < row_a * row_b; i++) {
			C[i] = sqrtf(C[i]);
		}
		break;
	}
	}
}

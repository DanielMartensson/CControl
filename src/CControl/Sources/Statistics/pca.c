/*
 * pca.c
 *
 *  Created on: 24 mars 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

static void average(float A[], float mu[], uint16_t row, uint16_t column);
static void centerData(float A[], float mu[], uint16_t row, uint16_t column);

 /*
  * Principal Component Analysis (PCA)
  * This compute the PCA of A and return eigenvetors U of A and average mu of A
  * A[m*n]
  * mu[m]
  * U[m*m]
  */

void pca(float A[], float mu[], float U[], uint8_t num_components, uint16_t row, uint16_t column) {
	/* Average data */
	average(A, mu, row, column);

	/* Center data */
	centerData(A, mu, row, column);

	/* Compute [U, S, V] = svd(A) */
	float *S = (float*)malloc(column * sizeof(float));
	float *V = (float*)malloc(column * column * sizeof(float));
	if (row == column) {
		svd_jacobi_one_sided(A, row, U, S, V);
	}else {
		svd_golub_reinsch(A, row, column, U, S, V);
	}

	/* Free */
	free(S);
	free(V);
}

static void average(float A[], float mu[], uint16_t row, uint16_t column) {
	uint16_t i;
	for (i = 0; i < row; i++) {
		/* Find the average of each column */
		mu[i] = mean(A, column);

		/* Jump to next column */
		A += column;
	}
}

static void centerData(float A[], float mu[], uint16_t row, uint16_t column) {
	/* Center data A = A - mu */
	uint16_t i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < column; j++) {
			A[j] -= mu[i];
		}

		/* Jump to next column */
		A += column;
	}
}
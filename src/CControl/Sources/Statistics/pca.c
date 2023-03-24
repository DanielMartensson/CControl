/*
 * pca.c
 *
 *  Created on: 24 mars 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

 /*
  * Principal Component Analysis (PCA)
  * This compute the PCA of X and return Y
  * X[m*n]
  * U[m*m]
  */

void pca(float A[], float U[], uint16_t row, uint16_t column) {
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
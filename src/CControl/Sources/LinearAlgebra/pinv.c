/*
 * pinv.c
 *
 *  Created on: 13 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Pseudo inverse by using Singular Value Decomposition
 * A [m*n]
 * m >= n
 * A+ = V*inv(S)*U'
 * This return matrix A [n*m] - Reversed size
 */
void pinv(float A[], uint16_t row, uint16_t column){
	/* Decleration */
	uint16_t i, j;

	/* Use Golub and Reinch if row != column */
	float *U = (float*)malloc(row * column * sizeof(float));
	float *S = (float*)malloc(column * sizeof(float));
	float *V = (float*)malloc(column * column * sizeof(float));
	if (row == column) {
		svd_jacobi_one_sided(A, row, U, S, V);
	}
	else {
		svd_golub_reinsch(A, row, column, U, S, V);
	}

	/* Do inv(S) */
	for (i = 0; i < column; i++) {
		S[i] = 1.0f / S[i]; /* Create inverse diagonal matrix */
	}

	/* Transpose U' */
	tran(U, row, column);

	/* U = S*U' */
	for (i = 0; i < row; i++) {
		for (j = 0; j < column; j++) {
			U[row * j + i] = S[j] * U[row * j + i];
		}
	}

	/* Do pinv now: A = V*U */
	mul(V, U, A, column, column, row);

	/* Free */
	free(U);
	free(S);
	free(V);

}


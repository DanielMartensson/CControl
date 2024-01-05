/*
 * era.c
 *
 *  Created on: 14 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "systemidentification.h"

/*
 * Eigensystem Realization Algorithm.
 * u [m*n] 		 // Input signal
 * y [m*n] 		 // Output signal
 * A [ADIM*ADIM] // System matrix with dimension ADIM*ADIM
 * B [ADIM*inputs_outputs] // Input matrix with dimension ADIM*inputs_outputs
 * C [inputs_outputs*ADIM] // Output matrix with dimension inputs_outputs*ADMIN
 */
void era(float u[], float y[], size_t row, size_t column, float A[], float B[], float C[], size_t row_a, size_t inputs_outputs) {
	/* Decleration */
	size_t i, j;

	/* Markov parameters - Impulse response */
	float *g = (float*)malloc(row * column * sizeof(float));
	okid(u, y, g, row, column);

	/* Compute the correct dimensions for matrix H */
	size_t row_h = row*(column/2);
	size_t column_h = column/2;

	/* Create Half Hankel matrix */
	float *H = (float*)malloc(row_h * column_h * sizeof(float));
	hankel(g, H, row, column, row_h, column_h, 1); /* Need to have 1 shift for this algorithm */

	/* Do SVD on the half hankel matrix H */
	float *U = (float*)malloc(row_h * column_h * sizeof(float));
	float *S = (float*)malloc(column_h * sizeof(float));
	float *V = (float*)malloc(column_h * column_h * sizeof(float));
	svd(H, row_h, column_h, U, S, V);
	/*svd_golub_reinsch(H, row_h, column_h, U, S, V);*/

	/* Re-create another hankel with shift = 2 */
	hankel(g, H, row, column, row_h, column_h, 2); /* Need to have 2 shift for this algorithm */

	/* Create C and B matrix */
	for(i = 0; i < row_a; i++){
		for (j = 0; j < inputs_outputs; j++) {
			C[j * row_a + i] = U[j * column_h + i] * sqrtf(S[i]); /* C = U*S^(1/2) */
		}

		for (j = 0; j < inputs_outputs; j++) {
			B[i * inputs_outputs + j] = sqrtf(S[i]) * V[j * column_h + i]; /* B = S^(1/2)*V^T */
		}
	}

	/* A = S^(-1/2)*U^T*H*V*S^(-1/2) */

	/* V = V*S^(-1/2) */
	for (i = 0; i < column_h; i++) {
		for (j = 0; j < column_h; j++) {
			V[j * column_h + i] = V[j * column_h + i] * sqrtf(1 / S[i]);
		}
	}

	/* U = S^(-1/2)*U^T */
	tran(U, row_h, column_h);
	for (i = 0; i < row_h; i++) {
		for (j = 0; j < column_h; j++) {
			U[j * row_h + i] = sqrtf(1 / S[j]) * U[j * row_h + i];
		}
	}


	/* Create A matrix: T = H*V */
	float *Temp = (float*)malloc(row_h * column_h * sizeof(float)); /* Temporary */
	mul(H, V, Temp, row_h, column_h, column_h);

	/* Now, multiply V = U(column_h, row_h)*Temp(row_h, column_h). U is transpose! */
	mul(U, Temp, V, column_h, row_h, column_h);

	/* Get the elements of V -> A */
	cut(V, column_h, A, 0, row_a-1, 0, row_a-1);

	/* Free */
	free(g);
	free(H);
	free(U);
	free(S);
	free(V);
	free(Temp);
}



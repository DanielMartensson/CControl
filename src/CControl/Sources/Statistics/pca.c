/*
 * pca.c
 *
 *  Created on: 24 mars 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

static void center_data(float X[], uint16_t row, uint16_t column);
static void compute_components(float X[], float W[], uint8_t components, uint16_t row, uint16_t column);

 /*
  * Principal Component Analysis (PCA)
  * This compute the PCA of X and return components W
  * X[m*n]
  * W[m*components]
  */

void pca(float X[], float W[], uint8_t components, uint16_t row, uint16_t column) {
	/* Average and center data */
	center_data(X, row, column);

	/* Get components */
	compute_components(X, W, components, row, column);
}

static void center_data(float X[], uint16_t row, uint16_t column) {
	uint16_t i, j;
	float mu;
	for (i = 0; i < row; i++) {
		/* Average data mu = mean(X, 2) */
		mu = mean(X, column);

		/* Center the data X = X - mu */
		for (j = 0; j < column; j++) {
			X[j] -= mu;
		}

		/* Jump to next row */
		X += column;
	}
}

static void compute_components(float X[], float W[], uint8_t components, uint16_t row, uint16_t column) {
	/* Compute [U, S, V] = svd(A) */
	float* U = (float*)malloc(row * column * sizeof(float));
	float* U0 = U;
	float* S = (float*)malloc(column * sizeof(float));
	float* V = (float*)malloc(column * column * sizeof(float));
	if (row == column) {
		svd_jacobi_one_sided(X, row, U, S, V);
	}
	else {
		svd_golub_reinsch(X, row, column, U, S, V);
	}

	/* Get the components from U */
	uint16_t i, bytes_shift = components * sizeof(float);
	for (i = 0; i < row; i++) {
		memcpy(W, U, bytes_shift); /* Move data from U to W, then shift the array position */
		W += components;
		U += column;
	}

	/* Reset */
	U = U0;

	/* Free */
	free(U);
	free(S);
	free(V);
}

/* 
 * GNU Octave code:
 *
 * X = [-1.0337     -0.43158      -1.7464     -0.61109       1.1577
       1.0955       0.1066      0.39597     -0.42132       2.3321
      -0.7158     -0.88635     -0.07532    -0.097022     -0.64252
       1.7255      -1.8957      0.86026     -0.93985      0.12185
      0.31585       2.0008     -0.35329     -0.65319     -0.57178
      0.15334     -0.35022       2.1064       0.3132    -0.089738
     -0.34019      0.81315      0.26919      -1.2984    -0.037872];

	
	mu = mean(X, 2)
	X = X - mu;
	[U, S, V] = svd(X);
	components = 4;
	U(:, 1:components)
 */
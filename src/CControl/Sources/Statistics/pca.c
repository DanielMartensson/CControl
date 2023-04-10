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
 * W[n*components]
 * P[m*components]
 */

void pca(float X[], float W[], float P[], uint8_t components, uint16_t row, uint16_t column) {
	/* Copy the data X -> Y */
	float* Y = (float*)malloc(row * column * sizeof(float));
	memcpy(Y, X, row * column * sizeof(float));

	/* Average and center data Y = Y - mean(Y) */
	center_data(Y, row, column);

	/* Do covariance Z = cov(Y) */
	float* Z = (float*)malloc(column * column * sizeof(float));
	covm(Y, Z, row, column);

	/* Get components [U, S, V] = svd(Z) */
	compute_components(Z, W, components, column, column);

	/* Project P = X*W */
	mul(X, W, P, row, column, components);

	/* Free */
	free(Y);
	free(Z);
}

static void center_data(float X[], uint16_t row, uint16_t column) {
	uint16_t i, j;
	float mu;
	float* X0 = X;
	tran(X, row, column);
	for (i = 0; i < column; i++) {
		/* Average data mu = mean(X) */
		mu = mean(X, row);

		/* Center the data X = X - mu */
		for (j = 0; j < row; j++) {
			X[j] -= mu;
		}

		/* Jump to next row */
		X += row;
	}
	X = X0;
	tran(X, column, row);
}

static void compute_components(float X[], float W[], uint8_t components, uint16_t row, uint16_t column) {
	/* Compute [U, S, V] = svd(A) */
	float* U = (float*)malloc(row * column * sizeof(float));
	float* S = (float*)malloc(column * sizeof(float));
	float* V = (float*)malloc(column * column * sizeof(float));
	float* V0 = V;
	svd_jacobi_one_sided(X, column, U, S, V);

	/* Get the components from V */
	uint16_t i, bytes_shift = components * sizeof(float);
	for (i = 0; i < column; i++) {
		memcpy(W, V, bytes_shift); /* Move data from V to W, then shift the array position */
		W += components;
		V += column;
	}

	/* Reset */
	V = V0;

	/* Free */
	free(U);
	free(S);
	free(V);
}

/*
 * GNU Octave code:
 *
 * X = [1 3 10;
		2 4 12;
		3 6 13;
		4 10 16;
		5 13 20;
		30 34 104;
		31 56 120;
		32 74 127;
		33 80 128;
		34 89 131;
		35 103 139];

	mu = mean(X);
	Y = X - mu;
	[U, S, V] = svd(Y, 0);
	components = 2;
	W = V(:, 1:components);
	P = X*W

	figure(1)
	scatter3(X(:, 1), X(:, 2), X(:, 3))
	figure(2)
	scatter(P(:, 2), P(:, 1));

 */
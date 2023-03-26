/*
 * eig_sym_generalized.c
 *
 *  Created on: 26 mars 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

 /*
  * Compute eigenvalues and eigenvectors from a symmetrical square matrix A and B
  * Notice that a square symmetrical matrix can never have complex eigenvalues and eigenvalues!
  * A [m*n]
  * B [m*n]
  * n == m
  * A^T = A
  * B^T = B
  * d [m] // Eigenvalues
  * A will become eigenvectors!
  */
void eig_sym_generalized(float A[], float B[], uint16_t row, float d[]) {
	/* Do cholesky factorization of B */
	float* L = (float*)malloc(row * row * sizeof(float));
	chol(B, L, row);

	/* Do Y = linsolve(L, A) */
	uint16_t i;
	float* Y = (float*)malloc(row * row * sizeof(float));
	float* Y0 = Y;
	float* A0 = A;
	tran(A, row, row); /* We transpose this so we can use it as a vector */
	for (i = 0; i < row; i++) {
		linsolve_lower_triangular(L, Y, A, row);
		A += row;
		Y += row;
	}

	/* Reset Y and A */
	Y = Y0; 
	A = A0;

	/* Transpose Y */
	tran(Y, row, row);

	/* Create Linv = inv(L') */
	float* Linv = (float*)malloc(row * row * sizeof(float));
	memcpy(Linv, L, row * row * sizeof(float));
	tran(Linv, row, row);
	inv(Linv, row);

	/* Multiply X = Y*Linv */
	float* X = (float*)malloc(row * row * sizeof(float));
	mul(Y, Linv, X, row, row, row);

	/* Do Eigendecomposition */
	eig_sym(X, row, d);

	/* Copyu over X to A */
	memcpy(A, X, row * row * sizeof(float));

	/* Free*/
	free(L);
	free(Y);
	free(Linv);
	free(X);
}

/*
 * GNU Octave code:
 * % Create two symmetric matrices
	A = [9.5605,  6.8163,  -1.9425,  -5.7558,    3.555,
		   6.8163,  9.7506,  0.26247,  -3.7964,   4.7512,
		  -1.9425, 0.26247,   3.4206,   2.9018,  0.45186,
		  -5.7558, -3.7964,   2.9018,   7.0731,  -2.5701,
			  3.555,  4.7512,  0.45186,  -2.5701,   2.6984];

	B = [1.4511,  0.29316, -1.1534, -0.35456,  0.44274,
			0.29316,   5.8465, -1.0767,   1.7268,   1.1316,
			-1.1534,  -1.0767,  7.5729,  -4.1207,   2.6296,
		 -0.35456,   1.7268, -4.1207,   5.7797,   -3.505,
		  0.44274,   1.1316,  2.6296,   -3.505,   4.5628];

	% Use cholesky decomposition
	L = chol(B, 'lower');
	Y = linsolve(L, A);
	X = Y*inv(L');
	[V, D] = eig(X)

	Output:

	V =

		 -0.84678     -0.15424      0.32617      0.37097     -0.12316
		 -0.24415     -0.14048     -0.86391    0.0052716     -0.41748
		  -0.2584     -0.35732     -0.26912     -0.22775      0.82538
		   0.1337      -0.8195      0.25365     -0.36878     -0.33197
		  0.37244     -0.39651     -0.10252      0.82127      0.13814

	D =

	Diagonal Matrix

		   8.9341            0            0            0            0
				0       3.6618            0            0            0
				0            0       0.8743            0            0
				0            0            0    0.0043923            0
				0            0            0            0      0.10919
 */
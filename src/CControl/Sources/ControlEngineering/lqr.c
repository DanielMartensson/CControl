/*
 * lqr.c
 *
 *  Created on: 28 Januari 2025
 *      Author: Daniel Mårtensson
 */

#include "controlengineering.h"

 /*
  * Compute R'*Y*U = I
  * R[row_r * column_r]
  * Y[row_y * column_y]
  * U[row_u * column_u]
  * I[column_r * column_u]
  */
static void RTYUI(const float R[], const float Y[], const float U[], float I[], const size_t row_r, const size_t column_r, const size_t row_y, const size_t column_y, const size_t row_u, const size_t column_u) {
	/* Turn R into transpose */
	float* RT = (float*)malloc(row_r * column_r * sizeof(float));
	memcpy(RT, R, row_r * column_r * sizeof(float));
	tran(RT, row_r, column_r);

	/* Compute YU = Y*U */
	float* YU = (float*)malloc(row_y * column_u * sizeof(float));
	mul(Y, U, YU, row_y, column_y, column_u);

	/* Compute I = RT*YU */
	mul(RT, YU, I, column_r, row_r, column_u);

	/* Free */
	free(RT);
	free(YU);
}

/*
 * Discrete Linear Quadratic Regulator
 * Find the control law matrix L = inv(B'*X*B + R)*B'*X*A;
 * iterations = Iterations for the ODE solver e.g 1000
 * sampleTime = Sample time for the discrete matrices A and B
 * A[row_a * row_a]
 * B[row_a * column_b]
 * Q[row_a * row_a]
 * R[column_b * column_b]
 * L[column_b * row_a]
 */
void lqr(const size_t iterations, const float sampleTime, const float A[], const float B[], const float Q[], const float R[], float L[], const size_t row_a, const size_t column_b) {
	/* Create the X solution to the discrete Algebraic Riccati Equation */
	float* X = (float*)malloc(row_a * row_a * sizeof(float));

	/* Find the solution */
	dare(iterations, sampleTime, A, B, Q, R, X, row_a, column_b);

	/* Compute B'*X*B + R = BTXBpR */
	float* BTXBpR = (float*)malloc(column_b * column_b * sizeof(float));
	RTYUI(B, X, B, BTXBpR, row_a, column_b, row_a, row_a, row_a, column_b);
	size_t i;
	for (i = 0; i < column_b * column_b; i++) {
		BTXBpR[i] += R[i];
	}

	/* Do inverse of BTXBpR */
	inv(BTXBpR, column_b);

	/* Compute B'*X*A = BTXA */
	float* BTXA = (float*)malloc(column_b * row_a * sizeof(float));
	RTYUI(B, X, A, BTXA, row_a, column_b, row_a, row_a, row_a, row_a);

	/* Compute BTXBpR * BTXA = L */
	mul(BTXBpR, BTXA, L, column_b, column_b, row_a);

	/* Free */
	free(X);
	free(BTXBpR);
	free(BTXA);
}
/*
 * lqe.c
 *
 *  Created on: 28 Januari 2025
 *      Author: Daniel Mårtensson
 */

#include "controlengineering.h"

/*
 * Discrete Linear Quadratic Estimator
 * Find the gain matrix K = lqr(A', C', Q, R)'
 * iterations = Iterations for the ODE solver e.g 1000
 * sampleTime = Sample time for the discrete matrices A and C
 * A[row_a * row_a]
 * C[row_c * row_a]
 * Q[row_a * row_a]
 * R[row_c * row_c]
 * K[row_a * row_c]
 */
void lqe(const size_t iterations, const float sampleTime, const float A[], const float C[], const float Q[], const float R[], float K[], const size_t row_a, const size_t row_c) {
	/* Turn A to transpose */
	float* AT = (float*)malloc(row_a * row_a * sizeof(float));
	memcpy(AT, A, row_a * row_a * sizeof(float));
	tran(AT, row_a, row_a);

	/* Turn C to transpose */
	float* CT = (float*)malloc(row_c * row_a * sizeof(float));
	memcpy(CT, C, row_c * row_a * sizeof(float));
	tran(CT, row_c, row_a);

	/* Compute LQR with duality method */
	lqr(iterations, sampleTime, AT, CT, Q, R, K, row_a, row_c);

	/* Free */
	free(AT);
	free(CT);
	
	/* Transpose on K */
	tran(K, row_c, row_a);
}
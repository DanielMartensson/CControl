/*
 * kalman.c
 *
 *  Created on: 30 june. 2024
 *      Author: Daniel Mårtensson
 */

#include "controlengineering.h"

static void state_estimation_propagation(const float A[], const float B[], const float C[], float x[], const float u[], float y[], const size_t row_a, const size_t row_c, const size_t column_b);
static void error_covariance_propagation(const float A[], const float Q[], float P[], const size_t row_a);
static void kalman_gain_matrix(const float P[], const float C[], const float R[], float K[], const size_t row_a, const size_t row_c);

/*
 * This is linear Kalman Filter. 
 * A linear Kalman Filter is made for control engineering only due to it's iteration speed and robustness against failing.
 * 
 */
void kalman(const float A[], const float B[], const float C[], float P[], const Q[], const R[], const float u[], float x[], float y[], const size_t row_a, const size_t row_c, const size_t column_b) {
	state_estimation_propagation(A, B, C, x, u, y, row_a, row_c, column_b);
	error_covariance_propagation(A, Q, P, row_a);

}

static void state_estimation_propagation(const float A[], const float B[], const float C[], float x[], const float u[], float y[], const size_t row_a, const size_t row_c, const size_t column_b) {
	/* Compute Ax */
	float* Ax = (float*)malloc(row_a * sizeof(float));
	mul(A, x, Ax, row_a, row_a, 1);

	/* Compute Bu */
	float* Bu = (float*)malloc(row_a * sizeof(float));
	mul(B, u, Bu, row_a, column_b, 1);

	/* Sum x = Ax + Bu */
	size_t i;
	for (i = 0; i < row_a; i++) {
		x[i] = Ax[i] + Bu[i];
	}

	/* Compute y = Cx */
	mul(C, x, y, row_c, row_a, 1);

	/* Free */
	free(Ax);
	free(Bu);
}

static void error_covariance_propagation(const float A[], const float Q[], float P[], const size_t row_a) {
	/* Make A transpose */
	float* AT = (float*)malloc(row_a * row_a * sizeof(float));
	memcpy(AT, A, row_a * row_a * sizeof(float));
	tran(AT, row_a, row_a);

	/* Compute PAT = P*AT */
	float* PAT = (float*)malloc(row_a * row_a * sizeof(float));
	mul(P, AT, PAT, row_a, row_a, row_a);

	/* Compute APAT = A*PAT */
	float* APAT = (float*)malloc(row_a * row_a * sizeof(float));
	mul(A, PAT, APAT, row_a, row_a, row_a);

	/* Add Q to APAT */
	size_t i;
	for (i = 0; i < row_a * row_a; i++) {
		APAT[i] += Q[i];
	}

	/* Free */
	free(AT);
	free(PAT);
	free(APAT);
}

static void kalman_gain_matrix(const float P[], const float C[], const float R[], float K[], const size_t row_a, const size_t row_c) {
	/* Make C transpose */
	float* CT = (float*)malloc(row_c * row_a * sizeof(float));
	memcpy(CT, C, row_c * row_a * sizeof(float));
	tran(CT, row_c, row_a);

	/* Compute PCT = P*CT */
	float* PCT = (float*)malloc(row_a * row_c * sizeof(float));
	mul(P, CT, PCT, row_a, row_a, row_c);

	/* Compute CPCT = C * PCT */
	float* CPCT = (float*)malloc(row_c * row_c * sizeof(float));
	mul(C, PCT, CPCT, row_c, row_a, row_c);

	/* Add R to CPCT */
	size_t i;
	for (i = 0; i < row_c * row_c; i++) {
		CPCT[i] += R[i];
	}

	/* Turn CPCT inverse */
	inv(CPCT, row_c);

	/* 2024-07-30: I stopped working on this because I took the decision that SR-UKF might be good enough for linear models */
}
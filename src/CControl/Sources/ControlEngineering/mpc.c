/*
 * mpc.c
 *
 *  Created on: 29 Januari 2025
 *      Author: Daniel Mårtensson
 */

#include "controlengineering.h"

/*
 * A[row_a * row_a]
 * B[row_a * column_b]
 * C[row_c * row_a]
 * E[row_a * column_e]
 * Ad[row_a * row_a]
 * Bd[row_a * column_b]
 * Cd[row_c * row_a]
 * Ed[row_a * column_e]
 */
void mpc_discrete_matrices(const float sampleTime, const float A[], const float B[], const float C[], const float E[], float Ad[], float Bd[], float Cd[], float Ed[], const size_t row_a, const size_t column_b, const size_t row_c, const size_t column_e) {
	/* Create augmented B */
	float* Bae = (float*)malloc(row_a * (column_b + column_e) * sizeof(float));
	insert(B, Bae, row_a, column_b, column_b + column_e, 0, 0);
	insert(E, Bae, row_a, column_e, column_b + column_e, 0, column_b);

	/* Copy */
	memcpy(Ad, A, row_a * row_a * sizeof(float));
	memcpy(Cd, C, row_c * row_a * sizeof(float));

	/* Turn Ad and Bae discrete */
	c2d(Ad, Bae, row_a, column_b + column_e, sampleTime);

	/* Then cut Bae into Bd and Ed */
	cut(Bae, column_b + column_e, Bd, 0, row_a - 1, 0, column_b - 1);
	cut(Bae, column_b + column_e, Ed, 0, row_a - 1, column_b, column_b + column_e - 1);

	/* Free */
	free(Bae);
}

/*
 * A[row_a * row_a]
 * C[row_c * row_a]
 * Q[row_a * row_a]
 * R[row_c * row_c]
 * K[row_a * row_c]
 */
void mpc_kalman_gain(const size_t iterations, const float sampleTime, const float A[], const float C[], const float Q[], const float R[], float K[], const size_t row_a, const size_t row_c) {
	lqe(iterations, sampleTime, A, C, Q, R, K, row_a, row_c);
}

/*
 * A[row_a * row_a]
 * C[row_c * row_a]
 * Phi[(N * row_c) * row_a]
 */
void mpc_phi_matrix(float Phi[], const float A[], const float C[], const size_t row_a, const size_t row_c, const size_t N) {
	obsv(Phi, A, C, row_a, row_c, N);
}

/*
 * Gamma[(N * row_c) * (N * column_b)]
 * Phi[(N * row_c) * row_a]
 * B[row_a * columb_b]
 * C[row_c * row_a]
 */
void mpc_gamma_matrix(float Gamma[], const float Phi[], const float B[], const float C[], const size_t row_a, const size_t row_c, const size_t column_b, const size_t N) {
	cab(Gamma, Phi, B, C, row_a, row_c, column_b, N);
}

/*
 * V[N*dim_v]
 * v[dim_v]
 */
void mpc_vector(float V[], const float v[], const size_t dim_v, const size_t N) {
	size_t i, j;
	for (i = 0; i < N; i++) {
		for (j = 0; j < dim_v; j++) {
			V[i * dim_v + j] = v[j];
		}
	}
}

/*
 * QZ[(N * row) * (N * row)]
 * Qz[row * row]
 */
void mpc_QZ_matrix(float QZ[], const float Qz[], const size_t row, const size_t N) {
	memset(QZ, 0, (N * row) * (N * row) * sizeof(float));
	size_t i;
	for (i = 0; i < N; i++) {
		insert(Qz, QZ, row, row, N * row, i * row, i * row);
	}
}

/*
 * S[row * column]
 */
void mpc_S_matrix(float S[], const float s, const size_t row, const size_t column) {
	eye(S, row, column);
	scalar(S, s, row * column);
}

/*
 * HS[(N * row) * (N * row)]
 * S[row * row]
 */
void mpc_HS_matrix(float HS[], const float S[], const size_t row, const size_t N) {
	/* Positive 2*S */
	float* p2S = (float*)malloc(row * row * sizeof(float));
	memcpy(p2S, S, row * row * sizeof(float));
	scalar(p2S, 2.0f, row * row);

	/* Negative -2*S */
	float* m2S = (float*)malloc(row * row * sizeof(float));
	memcpy(m2S, S, row * row * sizeof(float));
	scalar(m2S, -2.0f, row * row);

	/* Negative S */
	float* mS = (float*)malloc(row * row * sizeof(float));
	memcpy(mS, S, row * row * sizeof(float));
	scalar(mS, -1.0f, row * row);

	/* Fill */
	if (N == 1) {
		memset(HS, S, row * row * sizeof(float));
	}
	else {
		memset(HS, 0, N * row * N * row * sizeof(float));
		insert(p2S, HS, row, row, N * row, 0, 0);
		insert(mS, HS, row, row, N * row, row, 0);
		size_t k, k_row;
		for (k = 0; k < N - 2; k++) {
			k_row = k * row;

		}

	}
}
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
 * QZ[(N * row_c) * (N * row_c)]
 * Qz[row_c * row_c]
 */
void mpc_QZ_matrix(float QZ[], const float Qz[], const size_t row_c, const size_t N) {
	memset(QZ, 0, (N * row_c) * (N * row_c) * sizeof(float));
	size_t i;
	for (i = 0; i < N; i++) {
		insert(Qz, QZ, row_c, row_c, N * row_c, i * row_c, i * row_c);
	}
}

/*
 * S[column_b * column_b]
 */
void mpc_S_matrix(float S[], const float s, const size_t column_b) {
	eye(S, column_b, column_b);
	scalar(S, s, column_b * column_b);
}

/*
 * HS[(N * column_b) * (N * column_b)]
 * S[column_b * column_b]
 */
void mpc_HS_matrix(float HS[], const float S[], const size_t column_b, const size_t N) {
	/* Positive 2*S */
	float* p2S = (float*)malloc(column_b * column_b * sizeof(float));
	memcpy(p2S, S, column_b * column_b * sizeof(float));
	scalar(p2S, 2.0f, column_b * column_b);

	/* Negative -2*S */
	float* m2S = (float*)malloc(column_b * column_b * sizeof(float));
	memcpy(m2S, S, column_b * column_b * sizeof(float));
	scalar(m2S, -2.0f, column_b * column_b);

	/* Negative S */
	float* mS = (float*)malloc(column_b * column_b * sizeof(float));
	memcpy(mS, S, column_b * column_b * sizeof(float));
	scalar(mS, -1.0f, column_b * column_b);

	/* Fill */
	if (N == 1) {
		memset(HS, S, column_b * column_b * sizeof(float));
	}
	else {
		memset(HS, 0, N * column_b * N * column_b * sizeof(float));
		insert(p2S, HS, column_b, column_b, N * column_b, 0, 0);
		insert(mS, HS, column_b, column_b, N * column_b, column_b, 0);
		size_t i, i_column_b;
		for (i = 0; i < N - 2; i++) {
			i_column_b = i * column_b;
			insert(mS, HS, column_b, column_b, N * column_b, i_column_b, i_column_b + column_b);
			insert(p2S, HS, column_b, column_b, N * column_b, i_column_b + column_b, i_column_b + column_b);
			insert(m2S, HS, column_b, column_b, N * column_b, i_column_b + 2 * column_b, i_column_b + column_b);
		}
		i = N - 1;
		i_column_b = i * column_b;
		insert(mS, HS, column_b, column_b, N * column_b, i_column_b, i_column_b + column_b);
		insert(S, HS, column_b, column_b, N * column_b, i_column_b + column_b, i_column_b + column_b);
	}

	/* Free */
	free(p2S);
	free(m2S);
	free(mS);
}

/* 
 * H[(N * column_b) * (N * column_b)]
 * Gamma[(N * row_c) * (N * column_b)]
 * QZ[(N * column_b) * (N * column_b)]
 * HS[(N * column_b) * (N * column_b)]
 */
void mpc_H_matrix(float H[], const float Gamma[], const float QZ[], const float HS[], const size_t row_c, const size_t column_b, const size_t N) {
	/* Transpose Gamma */
	float* GammaT = (float*)malloc((N * row_c) * (N * column_b) * sizeof(float));
	memcpy(GammaT, Gamma, (N * row_c) * (N * column_b) * sizeof(float));
	tran(GammaT, N * row_c, N * column_b);

	/* Compute QZGamma = QZ * Gamma */
	float* QZGamma = (float*)malloc(N * column_b * N * column_b * sizeof(float));
	mul(QZ, Gamma, QZGamma, N * column_b, N * column_b, N * column_b);

	/* Compute H = GammaT * QZGamma + HS */
	mul(GammaT, QZGamma, H, N * column_b, N * row_c, N * column_b);
	size_t i;
	for (i = 0; i < N * column_b * N * column_b; i++) {
		H[i] += HS[i];
	}

	/* Free */
	free(GammaT);
	free(QZGamma);
}

/*
 * Mx0[(N * column_b) * row_a] 
 * Gamma[(N * row_c) * (N * column_b)]
 * QZ[(N * column_b) * (N * column_b)]
 * Phi[(N * row_c) * row_a]
 */
void mpc_Mx0_matrix(float Mx0[], const float Gamma[], const float QZ[], const float Phi[], const size_t row_a, const size_t row_c, const size_t column_b, const size_t N) {
	/* Transpose Gamma */
	float* GammaT = (float*)malloc((N * row_c) * (N * column_b) * sizeof(float));
	memcpy(GammaT, Gamma, (N * row_c) * (N * column_b) * sizeof(float));
	tran(GammaT, N * row_c, N * column_b);

	/* Compute QZPhi = QZ * Phi */
	float* QZPhi = (float*)malloc(N * column_b * row_a * sizeof(float));
	mul(QZ, Phi, QZPhi, N * column_b, N * column_b, row_a);

	/* Compute Mx0 = GammaT * QZPhi */
	mul(GammaT, QZPhi, Mx0, N * column_b, N * row_c, row_a);

	/* Free */
	free(GammaT);
	free(QZPhi);
}

/*
 * Mum1[(N * column_b) * column_b]
 * S[column_b * column_b]
 */
void mpc_Mum1_matrix(float Mum1[], const float S[], const size_t column_b, const size_t N) {
	/* Negative S */
	float* mS = (float*)malloc(column_b * column_b * sizeof(float));
	memcpy(mS, S, column_b * column_b * sizeof(float));
	scalar(mS, -1.0f, column_b * column_b);

	/* Insert */
	memset(Mum1, 0, (N * column_b) * column_b * sizeof(float));
	insert(mS, Mum1, column_b, column_b, column_b, 0, 0);

	/* Free */
	free(mS);
}

/*
 * MR[(N * column_b) * (N * column_b)] 
 * Gamma[(N * row_c) * (N * column_b)]
 * QZ[(N * column_b) * (N * column_b)]
 */
void mpc_MR_matrix(float MR[], const float Gamma[], const float QZ[], const size_t row_c, const size_t column_b, const size_t N) {
	/* Transpose -Gamma */
	float* GammaT = (float*)malloc((N * row_c) * (N * column_b) * sizeof(float));
	memcpy(GammaT, Gamma, (N * row_c) * (N * column_b) * sizeof(float));
	tran(GammaT, N * row_c, N * column_b);
	size_t i;
	for (i = 0; i < (N * row_c) * (N * column_b); i++) {
		GammaT[i] = -GammaT[i];
	}

	/* MR = -GammaT * QZ */
	mul(GammaT, QZ, MR, N * column_b, N * row_c, N * column_b);

	/* Free */
	free(GammaT);
}

/*
 * MD[(N * column_b) * (N * column_b)] 
 * Gamma[(N * row_c) * (N * column_b)]
 * QZ[(N * column_b) * (N * column_b)]
 * Gamma[(N * row_c) * (N * column_b)]
 */
void mpc_MD_matrix(float MD[], const float Gamma[], const float Gammad[], const float QZ[], const size_t row_c, const size_t column_b, const size_t N) {
	/* Transpose Gamma */
	float* GammaT = (float*)malloc((N * row_c) * (N * column_b) * sizeof(float));
	memcpy(GammaT, Gamma, (N * row_c) * (N * column_b) * sizeof(float));
	tran(GammaT, N * row_c, N * column_b);

	/* Compute QZGamma = QZ * Gammad */
	float* QZGammad = (float*)malloc(N * column_b * N * column_b * sizeof(float));
	mul(QZ, Gammad, QZGammad, N * column_b, N * column_b, N * column_b);

	/* Compute MD = GammaT * QZGammad */
	mul(GammaT, QZGammad, MD, N * column_b, N * row_c, N * column_b);

	/* Free */
	free(GammaT);
	free(QZGammad);
}

/*
 * Lambda[((N-1)*column_b) * column_b]
 */
void mpc_Lambda_matrix(float Lambda[], const size_t column_b, const size_t N) {
	float* I = (float*)malloc(column_b * column_b * sizeof(float));
	eye(I, column_b, column_b);
	float* T = (float*)malloc(column_b * (2 * column_b) * sizeof(float));
	insert(I, T, column_b, column_b, 2 * column_b, 0, column_b);
	scalar(I, -1.0f, column_b * column_b);
	insert(I, T, column_b, column_b, 2 * column_b, 0, 0);
	size_t i;
	for (i = 0; i < N - 1; i++) {
		insert(T, Lambda, column_b, 2 * column_b, N * column_b, i * column_b, (i + 1) * column_b);
	}

	/* Free */
	free(I);
	free(T);
}

/*
 * barSpsi[(N * column_b * (N * column_b)]
 */
void mpc_barSpsi_matrix(float barSpsi[], const float Spsi, const size_t column_b, const size_t N) {
	eye(barSpsi, N * column_b, N * column_b);
	scalar(barSpsi, Spsi, N * column_b * N * column_b);
}

/*
 * barspsi[N]
 */
void mpc_barspsi_vector(float barspsi[], const float spsi, const size_t N) {
	size_t i;
	for (i = 0; i < N; i++) {
		barspsi[i] = spsi;
	}
}

/*
 * barH[(2 * N * column_b) * (2 * N * column_b)]
 * H[(N * column_b) * (N * column_b)]
 * barSpsi[(N * column_b) * (N * column_b)]
 */
void mpc_barH_matrix(float barH[], const float H[], const float barSpsi[], const size_t column_b, const size_t N) {
	memset(barH, 0, (2 * N * column_b) * (2 * N * column_b) * sizeof(float));
	insert(H, barH, N * column_b, N * column_b, 2 * N * column_b, 0, 0);
	insert(barSpsi, barH, N * column_b, N * column_b, 2 * N * column_b, N * column_b, N * column_b);
}
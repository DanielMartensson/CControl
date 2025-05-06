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
 * K[row_a * row_c]
 */
void mpc_kalman_gain(const size_t iterations, const float sampleTime, const float A[], const float C[], const float qw, const float rv, float K[], const size_t row_a, const size_t row_c) {
	float* Q = (float*)malloc(row_a * row_a * sizeof(float));
	eye(Q, qw, row_a, row_a);
	float* R = (float*)malloc(row_c * row_c * sizeof(float));
	eye(R, rv, row_c, row_c);
	lqe(iterations, sampleTime, A, C, Q, R, K, row_a, row_c);

	/* Free */
	free(Q);
	free(R);
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
 */
void mpc_QZ_matrix(float QZ[], const float qz, const size_t row_c, const size_t N) {
	memset(QZ, 0, (N * row_c) * (N * row_c) * sizeof(float));
	float* Qz = (float*)malloc(row_c * row_c * sizeof(float));
	eye(Qz, qz, row_c, row_c);
	size_t i;
	for (i = 0; i < N; i++) {
		insert(Qz, QZ, row_c, row_c, N * row_c, i * row_c, i * row_c);
	}

	/* Free */
	free(Qz);
}

/*
 * S[column_b * column_b]
 */
void mpc_S_matrix(float S[], const float s, const size_t column_b) {
	eye(S, s, column_b, column_b);
}

/*
 * HS[(N * column_b) * (N * column_b)]
 * S[column_b * column_b]
 */
void mpc_HS_matrix(float HS[], const float S[], const size_t column_b, const size_t N) {
	/* Positive 2*S */
	float* p2S = (float*)malloc(column_b * column_b * sizeof(float));
	eye(p2S, 2.0f * S[0], column_b, column_b);

	/* Negative -2*S */
	float* m2S = (float*)malloc(column_b * column_b * sizeof(float));
	eye(m2S, -2.0f * S[0], column_b, column_b);

	/* Negative S */
	float* mS = (float*)malloc(column_b * column_b * sizeof(float));
	eye(mS, -1.0f * S[0], column_b, column_b);

	/* Fill */
	if (N == 1) {
		memcpy(HS, S, column_b * column_b * sizeof(float));
	}
	else {
		memset(HS, 0, N * column_b * N * column_b * sizeof(float));
		insert(p2S, HS, column_b, column_b, N * column_b, 0, 0);
		insert(mS, HS, column_b, column_b, N * column_b, 0, column_b);
		size_t i, i_column_b;
		for (i = 0; i < N - 2; i++) {
			i_column_b = i * column_b;
			insert(mS, HS, column_b, column_b, N * column_b, i_column_b + column_b, i_column_b);
			insert(p2S, HS, column_b, column_b, N * column_b, i_column_b + column_b, i_column_b + column_b);
			insert(mS, HS, column_b, column_b, N * column_b, i_column_b + column_b, i_column_b + 2 * column_b);
		}
		i_column_b = i * column_b;
		insert(mS, HS, column_b, column_b, N * column_b, i_column_b + column_b, i_column_b);
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
 * QZ[(N * row_c) * (N * row_c)]
 * HS[(N * column_b) * (N * column_b)]
 */
void mpc_H_matrix(float H[], const float Gamma[], const float QZ[], const float HS[], const size_t row_c, const size_t column_b, const size_t N) {
	/* Transpose Gamma */
	float* GammaT = (float*)malloc((N * row_c) * (N * column_b) * sizeof(float));
	memcpy(GammaT, Gamma, (N * row_c) * (N * column_b) * sizeof(float));
	tran(GammaT, N * row_c, N * column_b);

	/* Compute QZGamma = QZ * Gamma */
	float* QZGamma = (float*)malloc(N * row_c * N * column_b * sizeof(float));
	mul(QZ, Gamma, QZGamma, N * row_c, N * row_c, N * column_b);

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
 * QZ[(N * row_c) * (N * row_c)]
 * Phi[(N * row_c) * row_a]
 */
void mpc_Mx0_matrix(float Mx0[], const float Gamma[], const float QZ[], const float Phi[], const size_t row_a, const size_t row_c, const size_t column_b, const size_t N) {
	/* Transpose Gamma */
	float* GammaT = (float*)malloc((N * row_c) * (N * column_b) * sizeof(float));
	memcpy(GammaT, Gamma, (N * row_c) * (N * column_b) * sizeof(float));
	tran(GammaT, N * row_c, N * column_b);

	/* Compute QZPhi = QZ * Phi */
	float* QZPhi = (float*)malloc(N * row_c * row_a * sizeof(float));
	mul(QZ, Phi, QZPhi, N * row_c, N * row_c, row_a);

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
 * MR[(N * column_b) * (N * row_c)]
 * Gamma[(N * row_c) * (N * column_b)]
 * QZ[(N * row_c) * (N * row_c)]
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
	mul(GammaT, QZ, MR, N * column_b, N * row_c, N * row_c);

	/* Free */
	free(GammaT);
}

/*
 * MD[(N * column_b) * (N * column_e)]
 * Gamma[(N * row_c) * (N * column_b)]
 * QZ[(N * row_c) * (N * row_c)]
 * Gammad[(N * row_c) * (N * column_e)]
 */
void mpc_MD_matrix(float MD[], const float Gamma[], const float Gammad[], const float QZ[], const size_t row_c, const size_t column_b, const size_t column_e, const size_t N) {
	/* Transpose Gamma */
	float* GammaT = (float*)malloc((N * row_c) * (N * column_b) * sizeof(float));
	memcpy(GammaT, Gamma, (N * row_c) * (N * column_b) * sizeof(float));
	tran(GammaT, N * row_c, N * column_b);

	/* Compute QZGamma = QZ * Gammad */
	float* QZGammad = (float*)malloc(N * row_c * N * column_e * sizeof(float));
	mul(QZ, Gammad, QZGammad, N * row_c, N * row_c, N * column_e);

	/* Compute MD = GammaT * QZGammad */
	mul(GammaT, QZGammad, MD, N * column_b, N * row_c, N * column_e);

	/* Free */
	free(GammaT);
	free(QZGammad);
}

/*
 * Lambda[((N-1)*column_b) * (N * column_b)]
 */
void mpc_Lambda_matrix(float Lambda[], const size_t column_b, const size_t N) {
	memset(Lambda, 0, ((N - 1) * column_b) * (N * column_b) * sizeof(float));
	float* I = (float*)malloc(column_b * column_b * sizeof(float));
	eye(I, 1.0f, column_b, column_b);
	float* T = (float*)malloc(column_b * (2 * column_b) * sizeof(float));
	insert(I, T, column_b, column_b, 2 * column_b, 0, column_b);
	eye(I, -1.0f, column_b, column_b);
	insert(I, T, column_b, column_b, 2 * column_b, 0, 0);
	insert(T, Lambda, column_b, 2 * column_b, N * column_b, 0, 0);
	size_t i;
	for (i = 1; i < N - 1; i++) {
		insert(T, Lambda, column_b, 2 * column_b, N * column_b, i * column_b, i * column_b);
	}

	/* Free */
	free(I);
	free(T);
}

/*
 * barSpsi[(N * column_b * (N * column_b)]
 */
void mpc_barSpsi_matrix(float barSpsi[], const float Spsi, const size_t column_b, const size_t N) {
	eye(barSpsi, Spsi, N * column_b, N * column_b);
}

/*
 * barspsi[N * column_b]
 */
void mpc_barspsi_vector(float barspsi[], const float spsi, const size_t column_b, const size_t N) {
	size_t i;
	for (i = 0; i < N * column_b; i++) {
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

/*
 * eta[row_c]
 * r[row_c]
 * y[row_c]
 */
void mpc_eta_vector(float eta[], const float r[], const float y[], const float alpha, const bool integral_active, const size_t row_c) {
	if (integral_active) {
		size_t i;
		for (i = 0; i < row_c; i++) {
			const float psi = r[i] - y[i];
			eta[i] = eta[i] + alpha * psi;
		}
	} else {
		/* Reset the integral */
		memset(eta, 0, row_c * sizeof(float));
	}
}

/*
 * eta[row_c]
 */
void mpc_antiwindup_vector(float eta[], const float antiwindup, const size_t row_c) {
	size_t i;
	for (i = 0; i < row_c; i++) {
		if (fabsf(eta[i]) > antiwindup) {
			eta[i] = sign(eta[i]) * antiwindup;
		}
	}
}

/*
 * g[N * column_b]
 * Mx0[(N * column_b) * row_a]
 * MR[(N * column_b) * (N * row_c)]
 * R[N * row_c]
 * eta[row_c]
 * MD(N * column_b) * (N * column_e)
 * D[N * column_b]
 * Mum1[(N * column_b) * column_b]
 * um1[column_b]
 */
void mpc_g_vector(float g[], const float Mx0[], const float x[], const float MR[], const float R[], const float eta[], const float MD[], const float D[], const float Mum1[], const float um1[], const size_t row_a, const size_t row_c, const size_t column_b, const size_t column_e, const size_t N) {
	float* Mx0x = (float*)malloc(N * column_b * sizeof(float));
	mul(Mx0, x, Mx0x, N * column_b, row_a, 1);

	float* Reta = (float*)malloc(N * row_c * sizeof(float));
	size_t i, j;
	for (i = 0; i < N; i++) {
		for (j = 0; j < row_c; j++) {
			Reta[i * row_c + j] = R[i * row_c + j] + eta[j];
		}
	}
	float* MRReta = (float*)malloc(N * column_b * sizeof(float));
	mul(MR, Reta, MRReta, N * column_b, N * row_c, 1);

	float* MDD = (float*)malloc(N * column_b * sizeof(float));
	mul(MD, D, MDD, N * column_b, N * column_e, 1);

	float* Mum1um1 = (float*)malloc(N * column_b * sizeof(float));
	mul(Mum1, um1, Mum1um1, N * column_b, column_b, 1);

	for (i = 0; i < N * column_b; i++) {
		g[i] = Mx0x[i] + MRReta[i] + MDD[i] + Mum1um1[i];
	}

	/* Free */
	free(Mx0x);
	free(Reta);
	free(MRReta);
	free(MDD);
	free(Mum1um1);
}

/*
 * Umin[N * column_b]
 * umin[column_b]
 * deltaumin[column_b]
 * um1[column_b]
 */
void mpc_Umin_vector(float Umin[], const float umin[], const float deltaumin[], const float um1[], const size_t N, const size_t column_b) {
	mpc_vector(Umin, umin, column_b, N);
	size_t i;
	for (i = 0; i < column_b; i++) {
		Umin[i] = vmax(umin[i], deltaumin[i] + um1[i]);
	}
}

/*
 * Umax[N * column_b]
 * umax[column_b]
 * deltaumax[column_b]
 * um1[column_b]
 */
void mpc_Umax_vector(float Umax[], const float umax[], const float deltaumax[], const float um1[], const size_t N, const size_t column_b) {
	mpc_vector(Umax, umax, column_b, N);
	size_t i;
	for (i = 0; i < column_b; i++) {
		Umax[i] = vmin(umax[i], deltaumax[i] + um1[i]);
	}
}

/*
 * barZmin[N * row_c]
 * Zmin[N * row_c]
 * Phi[(N * row_c) * row_a]
 * x0[row_a]
 * Gammad[(N * row_c) * (N * column_e)]
 * D[N * column_e]
 */
void mpc_barZmin_vector(float barZmin[], const float Zmin[], const float Phi[], const float x0[], const float Gammad[], const float D[], const size_t row_a, const size_t row_c, const size_t column_b, const size_t column_e, const size_t N) {
	float* Phix0 = (float*)malloc(N * row_c * sizeof(float));
	mul(Phi, x0, Phix0, N * row_c, row_a, 1);
	float* GammadD = (float*)malloc(N * row_c * sizeof(float));
	mul(Gammad, D, GammadD, N * row_c, N * column_e, 1);
	size_t i;
	for (i = 0; i < N * row_c; i++) {
		barZmin[i] = Zmin[i] - Phix0[i] - GammadD[i];
	}

	/* Free */
	free(Phix0);
	free(GammadD);
}

/*
 * barZmax[N * row_c]
 * Zmax[N * row_c]
 * Phi[(N * row_c) * row_a]
 * x0[row_a]
 * Gammad[(N * row_c) * (N * column_e)]
 * D[N * column_e]
 */
void mpc_barZmax_vector(float barZmax[], const float Zmax[], const float Phi[], const float x0[], const float Gammad[], const float D[], const size_t row_a, const size_t row_c, const size_t column_b, const size_t column_e, const size_t N) {
	float* Phix0 = (float*)malloc(N * row_c * sizeof(float));
	mul(Phi, x0, Phix0, N * row_c, row_a, 1);
	float* GammadD = (float*)malloc(N * row_c * sizeof(float));
	mul(Gammad, D, GammadD, N * row_c, N * column_e, 1);
	size_t i;
	for (i = 0; i < N * row_c; i++) {
		barZmax[i] = Zmax[i] - Phix0[i] - GammadD[i];
	}

	/* Free */
	free(Phix0);
	free(GammadD);
}

/*
 * barg[N * column_b + column_b * N]
 * g[N * column_b]
 * barspsi[N * column_b]
 */
void mpc_barg_vector(float barg[], const float g[], const float barspsi[], const size_t column_b, const size_t N) {
	insert(g, barg, N * column_b, 1, 1, 0, 0);
	insert(barspsi, barg, N * column_b, 1, 1, N * column_b, 0);
}

/*
 * barUmin[N * column_b + N]
 * Umin[N * column_b]
 */
void mpc_barUmin_vector(float barUmin[], const float Umin[], const size_t column_b, const size_t N) {
	memset(barUmin, 0, (N * column_b + N) * sizeof(float));
	insert(Umin, barUmin, N * column_b, 1, 1, 0, 0);
}

/*
 * barUmax[N * column_b + N]
 * Umax[N * column_b]
 */
void mpc_barUmax_vector(float barUmax[], const float Umax[], const size_t column_b, const size_t N) {
	const float infinity = FLT_MAX;
	size_t i;
	for (i = 0; i < N; i++) {
		barUmax[N * column_b + i] = infinity;
	}
	insert(Umax, barUmax, N * column_b, 1, 1, 0, 0);
}

/*
 * bmin[(N - 1) * column_b + N * row_c + N * row_c]
 * deltaUmin[(N - 1) * column_b]
 * barZmin[N * row_c]
 */
void mpc_bmin_vector(float bmin[], const float deltaUmin[], const float barZmin[], const size_t column_b, const size_t row_c, const size_t N) {
	size_t insert_row = 0;
	insert(deltaUmin, bmin, (N - 1) * column_b, 1, 1, insert_row, 0);
	insert_row += (N - 1) * column_b;
	const float infinity = FLT_MAX;
	size_t i;
	for (i = 0; i < N * row_c; i++) {
		bmin[insert_row + i] = -infinity;
	}
	insert_row += i;
	insert(barZmin, bmin, N * row_c, 1, 1, insert_row, 0);
}

/*
 * bmax[(N - 1) * column_b + N * row_c + N * row_c]
 * deltaUmax[(N - 1) * column_b]
 * barZmax[N * row_c]
 */
void mpc_bmax_vector(float bmax[], const float deltaUmax[], const float barZmax[], const size_t column_b, const size_t row_c, const size_t N) {
	size_t insert_row = 0;
	insert(deltaUmax, bmax, (N - 1) * column_b, 1, 1, insert_row, 0);
	insert_row += (N - 1) * column_b;
	insert(barZmax, bmax, N * row_c, 1, 1, insert_row, 0);
	insert_row += N * row_c;
	const float infinity = FLT_MAX;
	size_t i;
	for (i = 0; i < N * row_c; i++) {
		bmax[insert_row + i] = infinity;
	}
}

/*
 * AA[((N-1) * column_b + 2 * N * row_c) * (2 * N * column_b)]
 * Lambda[((N-1)*column_b) * (N * column_b)]
 * Gamma[(N * row_c) * (N * column_b)]
 */
void mpc_AA_matrix(float AA[], const float Lambda[], const float Gamma[], const size_t row_c, const size_t column_b, const size_t N) {
	memset(AA, 0, ((N - 1) * column_b + 2 * N * row_c) * (2 * N * column_b) * sizeof(float));
	insert(Lambda, AA, (N - 1) * column_b, N * column_b, 2 * N * column_b, 0, 0);
	float* I = (float*)malloc((N * row_c) * (N * column_b) * sizeof(float));
	eye(I, -1.0f, N * row_c, N * column_b);
	insert(Gamma, AA, N * row_c, N * column_b, 2 * N * column_b, (N - 1) * column_b, 0);
	insert(I, AA, N * row_c, N * column_b, 2 * N * column_b, (N - 1) * column_b, N * column_b);
	insert(Gamma, AA, N * row_c, N * column_b, 2 * N * column_b, (N - 1) * column_b + N * row_c, 0);
	eye(I, 1.0f, N * row_c, N * column_b);
	insert(I, AA, N * row_c, N * column_b, 2 * N * column_b, (N - 1) * column_b + N * row_c, N * column_b);

	/* Free */
	free(I);
}

/*
 * aqp[(2*((N-1) * column_b + 2 * N * row_c) + 2 * (N * column_b + N)) * (2 * N * column_b)]
 * AA[((N-1) * column_b + 2 * N * row_c) * (2 * N * column_b)]
 */
void mpc_aqp_matrix(float aqp[], const float AA[], const size_t column_b, const size_t row_c, const size_t N) {
	/* First create a row index variable */
	size_t insert_row = 0;

	/* Clear aqp matrix */
	memset(aqp, 0, (2 * ((N - 1) * column_b + 2 * N * row_c) + 2 * (N * column_b + N)) * (2 * N * column_b) * sizeof(float));
	
	/* Insert first I matrix */
	eye(aqp + insert_row * (2 * N * column_b), 1.0f, N * column_b + N, 2 * N * column_b);
	insert_row += N * column_b + N;
	
	/* Insert first AA matrix */
	insert(AA, aqp, (N - 1) * column_b + 2 * N * row_c, 2 * N * column_b, 2 * N * column_b, insert_row, 0);
	insert_row += (N - 1) * column_b + 2 * N * row_c;
	
	/* Insert second -I matrix */
	eye(aqp + insert_row * (2 * N * column_b), -1.0f, N * column_b + N, 2 * N * column_b);
	insert_row += N * column_b + N;
	
	/* Then insert -AA matrix */
	size_t i;
	for (i = 0; i < ((N - 1) * column_b + 2 * N * row_c) * (2 * N * column_b); i++) {
		aqp[insert_row * 2 * N * column_b + i] = -AA[i];
	}
}

/*
 * bqp[2*(N * column_b + N) + 2*((N - 1) * column_b + N * row_c + N * row_c)]
 * barUmin[N * column_b + N]
 * barUmax[N * column_b + N]
 * bmin[(N - 1) * column_b + N * row_c + N * row_c]
 * bmax[(N - 1) * column_b + N * row_c + N * row_c]
 */
void mpc_bqp_vector(float bqp[], const float barUmin[], const float barUmax[], const float bmin[], const float bmax[], const size_t column_b, const size_t row_c, const size_t N) {
	size_t i, row = 0;
	for (i = 0; i < N * column_b + N; i++) {
		bqp[i] = barUmax[i];
	}
	row += i;
	for (i = 0; i < (N - 1) * column_b + N * row_c + N * row_c; i++) {
		bqp[row + i] = bmax[i];
	}
	row += i;
	for (i = 0; i < N * column_b + N; i++) {
		bqp[row + i] = -barUmin[i];
	}
	row += i;
	for (i = 0; i < (N - 1) * column_b + N * row_c + N * row_c; i++) {
		bqp[row + i] = -bmin[i];
	}
}


/*
 * deltaumin[column_b]
 * deltaumax[column_b]
 */
void mpc_set_input_change_constraints(MPC* mpc, const float deltaumin[], const float deltaumax[]) {
	/* Create constraints on the movment - Equation (3.38) */
	const size_t column_b = mpc->column_b;
	const size_t N = mpc->N;
	if (mpc->deltaUmin) {
		free(mpc->deltaUmin);
	}
	if (mpc->deltaUmax) {
		free(mpc->deltaUmax);
	}
	mpc->deltaUmin = (float*)malloc((N - 1) * column_b * sizeof(float));
	mpc->deltaUmax = (float*)malloc((N - 1) * column_b * sizeof(float));
	mpc_vector(mpc->deltaUmin, deltaumin, column_b, N - 1);
	mpc_vector(mpc->deltaUmax, deltaumax, column_b, N - 1);
	memcpy(mpc->deltaumin, deltaumin, mpc->column_b * sizeof(float));
	memcpy(mpc->deltaumax, deltaumax, mpc->column_b * sizeof(float));

	/* Debug
	print(mpc->deltaUmin, (N - 1) * column_b, 1);
	print(mpc->deltaUmax, (N - 1) * column_b, 1); */
}

/*
 * zmin[row_c]
 * zmax[row_c]
 */
void mpc_set_output_constraints(MPC* mpc, const float zmin[], const float zmax[]) {
	/* Create constraints on outputs - Equation (3.43) */
	const size_t row_c = mpc->row_c;
	const size_t N = mpc->N;
	if (mpc->Zmin) {
		free(mpc->Zmin);
	}
	mpc->Zmin = (float*)malloc(N * row_c * sizeof(float));
	if (mpc->Zmax) {
		free(mpc->Zmax);
	}
	mpc->Zmax = (float*)malloc(N * row_c * sizeof(float));
	mpc_vector(mpc->Zmin, zmin, row_c, N);
	mpc_vector(mpc->Zmax, zmax, row_c, N);

	/* Debug
	print(mpc->Zmin, N * row_c, 1);
	print(mpc->Zmax, N * row_c, 1); */
}

/*
 * umin[column_b]
 * umax[column_b]
 */
void mpc_set_input_constraints(MPC* mpc, const float umin[], const float umax[]) {
	/* Create constraints on inputs - Equation (3.40) */
	memcpy(mpc->umin, umin, mpc->column_b * sizeof(float));
	memcpy(mpc->umax, umax, mpc->column_b * sizeof(float));
}

/*
 * A[row_a * row_a]
 * B[row_a * column_b]
 * C[row_c * row_a]
 * E[row_a * column_e]
 */
bool mpc_init(MPC* mpc, const float A[], const float B[], const float C[], const float E[], const float sampleTime_mpc, const float sampleTime_kf, const float qw, const float rv, const float qz, const float s, const float Spsi_spsi, const size_t row_a, const size_t column_b, const size_t row_c, const size_t column_e, const size_t N, const size_t iterations) {
	/* Check if the mpc has been initlized before */
	if (mpc->is_initlized) {
		return false;
	}

	/* Set sizes */
	mpc->row_a = row_a;           /* Dimension of A matrix */
	mpc->column_b = column_b;     /* Columns of B matrix */
	mpc->row_c = row_c;           /* Dimension of C matrix */
	mpc->column_e = column_e;     /* Columns of E matrix */
	mpc->N = N;                   /* Horizon */

	/* Create discrete matrices for MPC - Equation (2.9) */
	mpc->Ad = (float*)malloc(row_a * row_a * sizeof(float));
	mpc->Bd = (float*)malloc(row_a * column_b * sizeof(float));
	mpc->Cd = (float*)malloc(row_c * row_a * sizeof(float));
	mpc->Ed = (float*)malloc(row_a * column_e * sizeof(float));
	mpc_discrete_matrices(sampleTime_mpc, A, B, C, E, mpc->Ad, mpc->Bd, mpc->Cd, mpc->Ed, row_a, column_b, row_c, column_e);

	/* Create discrete matrices for KF - Equation (2.9) */
	mpc->Adkf = (float*)malloc(row_a * row_a * sizeof(float));
	mpc->Bdkf = (float*)malloc(row_a * column_b * sizeof(float));
	mpc->Cdkf = (float*)malloc(row_c * row_a * sizeof(float));
	mpc->Edkf = (float*)malloc(row_a * column_e * sizeof(float));
	mpc_discrete_matrices(sampleTime_kf, A, B, C, E, mpc->Adkf, mpc->Bdkf, mpc->Cdkf, mpc->Edkf, row_a, column_b, row_c, column_e);

	/* Debug
	print(mpc->Ad, row_a, row_a);
	print(mpc->Bd, row_a, column_b);
	print(mpc->Cd, row_c, row_a);
	print(mpc->Ed, row_a, column_e); 
	print(mpc->Adkf, row_a, row_a);
	print(mpc->Bdkf, row_a, column_b);
	print(mpc->Cdkf, row_c, row_a);
	print(mpc->Edkf, row_a, column_e); */

	/* Create the kalman gain matrix K - Here we use Kalman-Bucy (1961) filter instead of Kalman Filter (1960) */
	mpc->K = (float*)malloc(row_a * row_c * sizeof(float));
	mpc_kalman_gain(iterations, sampleTime_kf, mpc->Adkf, mpc->Cdkf, qw, rv, mpc->K, row_a, row_c);

	/* Debug 
	print(mpc->K, row_a, row_c); */

	/* Create the Phi matrix and lower hankel toeplitz Gamma matrix of inputs - Equation (3.6) */
	mpc->Phi = (float*)malloc((N * row_c) * row_a * sizeof(float));
	mpc_phi_matrix(mpc->Phi, mpc->Ad, mpc->Cd, row_a, row_c, N);

	/* Debug
	print(mpc->Phi, N * row_c, row_a); */

	/* Lower triangular toeplitz of extended observability matrix */
	float* Gamma = (float*)malloc((N * row_c) * (N * column_b) * sizeof(float));
	mpc_gamma_matrix(Gamma, mpc->Phi, mpc->Bd, mpc->Cd, row_a, row_c, column_b, N);

	/* Debug
	print(Gamma, N * row_c, N * column_b); */

	/* Create the weigth matrix - Equation (3.10) */
	float* QZ = (float*)malloc((N * row_c) * (N * row_c) * sizeof(float));
	mpc_QZ_matrix(QZ, qz, row_c, N);

	/* Debug
	print(QZ, N * row_c, N * row_c); */

	/* Create the regularization matrix - Equation (3.21) */
	float* S = (float*)malloc(column_b * column_b * sizeof(float));
	mpc_S_matrix(S, s, column_b);
	float* HS = (float*)malloc((N * column_b) * (N * column_b) * sizeof(float));
	mpc_HS_matrix(HS, S, column_b, N);

	/* Debug
	print(S, column_b, column_b);
	print(HS, N * column_b, N * column_b); */

	/* Create the QP solver H matrix - Equation (3.24) */
	float* H = (float*)malloc((N * column_b) * (N * column_b) * sizeof(float));
	mpc_H_matrix(H, Gamma, QZ, HS, row_c, column_b, N);

	/* Free */
	free(HS);

	/* Debug
	print(H, N * column_b, N * column_b); */

	/* Create the lower hankel toeplitz Gamma matrix of disturbance - Equation (3.27) */
	mpc->Gammad = (float*)malloc((N * row_c) * (N * column_e) * sizeof(float));
	mpc_gamma_matrix(mpc->Gammad, mpc->Phi, mpc->Ed, mpc->Cd, row_a, row_c, column_e, N);

	/* Debug
	print(mpc->Gammad, N * row_c, N * column_e); */

	/* Create the QP solver matrix for the gradient - Equation (3.32) */
	mpc->Mx0 = (float*)malloc((N * column_b) * row_a * sizeof(float));
	mpc_Mx0_matrix(mpc->Mx0, Gamma, QZ, mpc->Phi, row_a, row_c, column_b, N);
	mpc->Mum1 = (float*)malloc((N * column_b) * column_b * sizeof(float));
	mpc_Mum1_matrix(mpc->Mum1, S, column_b, N);
	mpc->MR = (float*)malloc((N * column_b) * (N * row_c) * sizeof(float));
	mpc_MR_matrix(mpc->MR, Gamma, QZ, row_c, column_b, N);
	mpc->MD = (float*)malloc((N * column_b) * (N * column_e) * sizeof(float));
	mpc_MD_matrix(mpc->MD, Gamma, mpc->Gammad, QZ, row_c, column_b, column_e, N);

	/* Free */
	free(S);
	free(QZ);

	/* Debug
	print(mpc->Mx0, N * column_b, row_a);
	print(mpc->Mum1, N * column_b, column_b);
	print(mpc->MR, N * column_b, N * row_c);
	print(mpc->MD, N * column_b, N * column_e); */

	/* Create constraints on the movment - Equation (3.38) */
	float* Lambda = (float*)malloc(((N - 1) * column_b) * (N * column_b) * sizeof(float));
	mpc_Lambda_matrix(Lambda, column_b, N);

	/* Debug
	print(Lambda, (N - 1) * column_b, N * column_b); */

	/* Create the slack variables - Equation (3.49) */
	float* barSpsi = (float*)malloc((N * column_b) * (N * column_b) * sizeof(float));
	mpc_barSpsi_matrix(barSpsi, Spsi_spsi, column_b, N);
	mpc->barspsi = (float*)malloc(N * column_b * sizeof(float));
	mpc_barspsi_vector(mpc->barspsi, Spsi_spsi, column_b, N);

	/* Debug
	print(barSpsi, N * column_b, N * column_b);
	print(mpc->barspsi, N, 1); */

	/* Create QP solver matrix - Equation (3.51) */
	mpc->barH = (float*)malloc((2 * N * column_b) * (2 * N * column_b) * sizeof(float));
	mpc_barH_matrix(mpc->barH, H, barSpsi, column_b, N);

	/* Free */
	free(H);
	free(barSpsi);

	/* Debug
	print(mpc->barH, 2 * N * column_b, 2 * N * column_b); */

	/* Create inequality constraints AA - Equation (3.56) */
	mpc->AA = (float*)malloc(((N - 1) * column_b + 2 * N * row_c) * (2 * N * column_b) * sizeof(float));
	mpc_AA_matrix(mpc->AA, Lambda, Gamma, row_c, column_b, N);

	/* Debug 
	print(mpc->AA, (N - 1) * column_b + 2 * N * row_c, (2 * N * column_b)); */

	/* Free */
	free(Gamma);
	free(Lambda);

	/* Create empty arrays */
	mpc->eta = (float*)malloc(row_c * sizeof(float));
	memset(mpc->eta, 0, row_c * sizeof(float));
	mpc->x = (float*)malloc(row_a * sizeof(float));
	memset(mpc->x, 0, row_a * sizeof(float));

	/* Create holders */
	mpc->deltaumin = (float*)malloc(column_b * sizeof(float));
	mpc->deltaumax = (float*)malloc(column_b * sizeof(float));
	mpc->umin = (float*)malloc(column_b * sizeof(float));
	mpc->umax = (float*)malloc(column_b * sizeof(float));

	/* Flag */
	mpc->is_initlized = true;

	/* Return true */
	return true;
}

/*
 * umin[column_b]
 * umax[column_b]
 * zmin[row_c]
 * zmax[row_c]
 * deltaumin[column_b]
 * deltaumax[column_b]
 */
void mpc_set_constraints(MPC* mpc, const float umin[], const float umax[], const float zmin[], const float zmax[], const float deltaumin[], const float deltaumax[], const float alpha, const float antiwindup) {
	mpc_set_input_constraints(mpc, umin, umax);
	mpc_set_output_constraints(mpc, zmin, zmax);
	mpc_set_input_change_constraints(mpc, deltaumin, deltaumax);
	mpc->alpha = alpha;
	mpc->antiwindup = antiwindup;
}

/*
 * u[column_b]
 * r[row_c]
 * y[row_c]
 * d[column_e]
 */
STATUS_CODES mpc_optimize(MPC* mpc, float u[], const float r[], const float y[], const float d[], const bool integral_active) {
	/* Get sizes */
	const size_t row_a = mpc->row_a;
	const size_t column_b = mpc->column_b;
	const size_t row_c = mpc->row_c;
	const size_t column_e = mpc->column_e;
	const size_t N = mpc->N;

	/* Integral action - Equation (3.66) */
	mpc_eta_vector(mpc->eta, r, y, mpc->alpha, integral_active, row_c);

	/* Debug
	print(mpc->eta, row_c, 1); */

	/* Anti-windup */
	mpc_antiwindup_vector(mpc->eta, mpc->antiwindup, row_c);

	/* Debug
	print(mpc->eta, row_c, 1); */

	/* Create reference vector */
	float* R = (float*)malloc(N * row_c * sizeof(float));
	mpc_vector(R, r, row_c, N);

	/* Debug
	print(R, N * row_c, 1); */

	/* Create disturbance vector */
	float* D = (float*)malloc(N * column_e * sizeof(float));
	mpc_vector(D, d, column_e, N);

	/* Debug
	print(D, N * column_e, 1); */

	/* Old u to um1 */
	float* um1 = (float*)malloc(column_b * sizeof(float));
	memcpy(um1, u, column_b * sizeof(float));

	/* Debug
	print(um1, column_b, 1); */

	/*
	 * Create gradient g. Also add the integral eta together with reference vector R for adjust the reference settings - Equation (3.32)
	 * The reason why adjusting the reference R vector is because then the integral action will be optimized inside the QP-solver.
	 */
	float* g = (float*)malloc(N * column_b * sizeof(float));
	mpc_g_vector(g, mpc->Mx0, mpc->x, mpc->MR, R, mpc->eta, mpc->MD, D, mpc->Mum1, um1, row_a, row_c, column_b, column_e, N);

	/* Free */
	free(R);

	/* Debug
	print(g, N * column_b, 1); */

	/* Create constrants on inputs */
	float* Umin = (float*)malloc(N * column_b * sizeof(float));
	float* Umax = (float*)malloc(N * column_b * sizeof(float));
	mpc_Umin_vector(Umin, mpc->umin, mpc->deltaumin, um1, N, column_b);
	mpc_Umax_vector(Umax, mpc->umax, mpc->deltaumax, um1, N, column_b);

	/* Free */
	free(um1);

	/* Create constraints for the output - Equation (3.44) */
	float* barZmin = (float*)malloc(N * row_c * sizeof(float));
	float* barZmax = (float*)malloc(N * row_c * sizeof(float));
	mpc_barZmin_vector(barZmin, mpc->Zmin, mpc->Phi, mpc->x, mpc->Gammad, D, row_a, row_c, column_b, column_e, N);
	mpc_barZmax_vector(barZmax, mpc->Zmax, mpc->Phi, mpc->x, mpc->Gammad, D, row_a, row_c, column_b, column_e, N);

	/* Free */
	free(D);

	/* Debug
	print(mpc->barZmin, N * row_c, 1);
	print(mpc->barZmax, N * row_c, 1); */

	/* Create gradient bar g - Equation (3.51) */
	float* barg = (float*)malloc((N * column_b + column_b * N) * sizeof(float));
	mpc_barg_vector(barg, g, mpc->barspsi, column_b, N);

	/* Free */
	free(g);

	/* Debug
	print(barg, N * column_b + column_b * N, 1); */

	/* Debug
	print(Umin, N * column_b, 1);
	print(Umax, N * column_b, 1); */

	/* Create barUmin and barUmax - Equation (3.52) */
	float* barUmin = (float*)malloc((N * column_b + N) * sizeof(float));
	float* barUmax = (float*)malloc((N * column_b + N) * sizeof(float));
	mpc_barUmin_vector(barUmin, Umin, column_b, N);
	mpc_barUmax_vector(barUmax, Umax, column_b, N);

	/* Free */
	free(Umin);
	free(Umax);

	/* Debug
	print(barUmin, N * column_b + N, 1);
	print(barUmax, N * column_b + N, 1); */

	/* Create bmin and bmax - Equation (3.56) */
	float* bmin = (float*)malloc(((N - 1) * column_b + N * row_c + N * row_c) * sizeof(float));
	float* bmax = (float*)malloc(((N - 1) * column_b + N * row_c + N * row_c) * sizeof(float));
	mpc_bmax_vector(bmax, mpc->deltaUmax, barZmax, column_b, row_c, N);
	mpc_bmin_vector(bmin, mpc->deltaUmin, barZmin, column_b, row_c, N);

	/* Debug
	print(bmin, (N - 1) * column_b + N * row_c + N * row_c, 1);
	print(bmax, (N - 1) * column_b + N * row_c + N * row_c, 1); */

	/* Free */
	free(barZmin);
	free(barZmax);

	/* Create for QP - Equation (3.57) */
	float* aqp = (float*)malloc((2 * ((N - 1) * column_b + 2 * N * row_c) + 2 * (N * column_b + N)) * (2 * N * column_b) * sizeof(float));
	float* bqp = (float*)malloc((2 * (N * column_b + N) + 2 * ((N - 1) * column_b + N * row_c + N * row_c)) * sizeof(float));
	mpc_aqp_matrix(aqp, mpc->AA, column_b, row_c, N);
	mpc_bqp_vector(bqp, barUmin, barUmax, bmin, bmax, column_b, row_c, N);

	/* Free */
	free(bmin);
	free(bmax);
	free(barUmin);
	free(barUmax);

	/* Debug 
	print(aqp, 2 * ((N - 1) * column_b + 2 * N * row_c) + 2 * (N * column_b + N), 2 * N * column_b);
	print(bqp, 2 * (N * column_b + N) + 2 * ((N - 1) * column_b + N * row_c + N * row_c), 1); */

	/* Quadraptic programming output */
	float* U = (float*)malloc(2 * N * column_b * sizeof(float));
	const STATUS_CODES status = quadprogslim(mpc->barH, barg, aqp, bqp, NULL, NULL, U, 2 * ((N - 1) * column_b + 2 * N * row_c) + 2 * (N * column_b + N), 0, 2 * N * column_b, false);

	/* Get amount of output from U */
	memcpy(u, U, column_b * sizeof(float));

	/* Free */
	free(barg);
	free(aqp);
	free(bqp);
	free(U);

	/* Return status */
	return status;
}

/*
 * u[column_b]
 * y[row_c]
 * d[column_e]
 */
void mpc_estimate(MPC* mpc, const float u[], const float y[], const float d[]) {
	/* Get sizes */
	const size_t row_c = mpc->row_c;
	const size_t row_a = mpc->row_a;
	const size_t column_b = mpc->column_b;
	const size_t column_e = mpc->column_e;

	/* Compute model output */
	float* Cdkfx = (float*)malloc(row_c * sizeof(float));
	mul(mpc->Cdkf, mpc->x, Cdkfx, row_c, row_a, 1);

	/* Update error - Equation (3.72) */
	float* e = (float*)malloc(row_c * sizeof(float));
	size_t i;
	for (i = 0; i < row_c; i++) {
		e[i] = y[i] - Cdkfx[i];
	}

	/* Free */
	free(Cdkfx);

	/* Compute kalman */
	float* Ke = (float*)malloc(row_a * sizeof(float));
	mul(mpc->K, e, Ke, row_a, row_c, 1);

	/* Free */
	free(e);

	/* Kalman update - Equation (3.75) */
	for (i = 0; i < row_a; i++) {
		mpc->x[i] = mpc->x[i] + Ke[i];
	}

	/* Free */
	free(Ke);

	/* Compute candidate state x - Equation (3.65) */
	float* Adkfx = (float*)malloc(row_a * sizeof(float));
	mul(mpc->Adkf, mpc->x, Adkfx, row_a, row_a, 1);
	float* Bdkfu = (float*)malloc(row_a * sizeof(float));
	mul(mpc->Bdkf, u, Bdkfu, row_a, column_b, 1);
	float* Edkfd = (float*)malloc(row_a * sizeof(float));
	mul(mpc->Edkf, d, Edkfd, row_a, column_e, 1);
	for (i = 0; i < row_a; i++) {
		mpc->x[i] = Adkfx[i] + Bdkfu[i] + Edkfd[i];
	}

	/* Free */
	free(Adkfx);
	free(Bdkfu);
	free(Edkfd);
}

bool mpc_free(MPC* mpc) {
	/* Only free if it has been initlized */
	if (!mpc->is_initlized) {
		return false;
	}

	/* This follows the MPC struct */
	free(mpc->Ad);
	free(mpc->Bd);
	free(mpc->Cd);
	free(mpc->Ed);
	free(mpc->Adkf);
	free(mpc->Bdkf);
	free(mpc->Cdkf);
	free(mpc->Edkf);
	free(mpc->K);
	free(mpc->Phi);
	free(mpc->Gammad);
	free(mpc->Mx0);
	free(mpc->Mum1);
	free(mpc->MR);
	free(mpc->MD);
	free(mpc->deltaUmin);
	free(mpc->deltaUmax);
	free(mpc->deltaumin);
	free(mpc->deltaumax);
	free(mpc->Zmin);
	free(mpc->Zmax);
	free(mpc->umin);
	free(mpc->umax);
	free(mpc->barspsi);
	free(mpc->barH);
	free(mpc->AA);
	free(mpc->eta);
	free(mpc->x);

	/* Flag */
	mpc->is_initlized = false;

	/* Return true */
	return true;
}

/* GNU Octave code
% Use Model Predictive Control with integral action, quadratic programming and kalman-bucy filter
% This MPC code follows this thesis: https://github.com/DanielMartensson/MataveControl/blob/master/Model%20Predictive%20Control%20for%20an%20artifical%20pancreas%20-%20Matias%20S%C3%B8rensen%20og%20Simon%20Kristiansen.pdf
% Input: sysp(State space model of the plant), sysc(State space model of the controller), N(Horizon number), r(Reference vector),
% umin(Minimum input vector), umax(Maximum input vector), zmin(Minimum output vector), zmax(Maximum output vector),
% deltaumin(Minimum output vector rate of change), deltaumax(Maximum rate of change output vector), antiwindup(Maximum/Minimum value of integral),
% alpha(Integral rate, optional), Ts_mpc(The sample time of MPC, optional), Ts_kf(The sample time of KF, optional), T(End time, optional), x0(Initial state, optional),
% s(Regularization value, optional), Qz(Weight parameter, optional), qw(Disturbance kalman filter tuning, optional),
% rv(Noice kalman filter tuning, optional), Spsi_spsi(Slack variable matrix tuning and slack variable vector tuning, optional),
% d(Disturbance vector e.g other measurements rather than y, optional), E(Disturbance input signal matrix, optional)
% Output: y(Output signal), T(Discrete time vector), X(State vector), U(Output signal)
% Example 1: [Y, T, X, U] = mc.kf_qmpc(sysp, sysc, N, r, umin, umax, zmin, zmax, deltaumin, deltaumax, antiwindup)
% Example 2: [Y, T, X, U] = mc.kf_qmpc(sysp, sysc, N, r, umin, umax, zmin, zmax, deltaumin, deltaumax, antiwindup, alpha)
% Example 3: [Y, T, X, U] = mc.kf_qmpc(sysp, sysc, N, r, umin, umax, zmin, zmax, deltaumin, deltaumax, antiwindup, alpha, Ts_mpc)
% Example 4: [Y, T, X, U] = mc.kf_qmpc(sysp, sysc, N, r, umin, umax, zmin, zmax, deltaumin, deltaumax, antiwindup, alpha, Ts_mpc, Ts_kf)
% Example 5: [Y, T, X, U] = mc.kf_qmpc(sysp, sysc, N, r, umin, umax, zmin, zmax, deltaumin, deltaumax, antiwindup, alpha, Ts_mpc, Ts_kf, T)
% Example 6: [Y, T, X, U] = mc.kf_qmpc(sysp, sysc, N, r, umin, umax, zmin, zmax, deltaumin, deltaumax, antiwindup, alpha, Ts_mpc, Ts_kf, T, x0)
% Example 7: [Y, T, X, U] = mc.kf_qmpc(sysp, sysc, N, r, umin, umax, zmin, zmax, deltaumin, deltaumax, antiwindup, alpha, Ts_mpc, Ts_kf, T, x0, s)
% Example 8: [Y, T, X, U] = mc.kf_qmpc(sysp, sysc, N, r, umin, umax, zmin, zmax, deltaumin, deltaumax, antiwindup, alpha, Ts_mpc, Ts_kf, T, x0, s, Qz)
% Example 9: [Y, T, X, U] = mc.kf_qmpc(sysp, sysc, N, r, umin, umax, zmin, zmax, deltaumin, deltaumax, antiwindup, alpha, Ts_mpc, Ts_kf, T, x0, s, Qz, qw)
% Example 10: [Y, T, X, U] = mc.kf_qmpc(sysp, sysc, N, r, umin, umax, zmin, zmax, deltaumin, deltaumax, antiwindup, alpha, Ts_mpc, Ts_kf, T, x0, s, Qz, qw, rv)
% Example 11: [Y, T, X, U] = mc.kf_qmpc(sysp, sysc, N, r, umin, umax, zmin, zmax, deltaumin, deltaumax, antiwindup, alpha, Ts_mpc, Ts_kf, T, x0, s, Qz, qw, rv, Spsi_spsi)
% Example 12: [Y, T, X, U] = mc.kf_qmpc(sysp, sysc, N, r, umin, umax, zmin, zmax, deltaumin, deltaumax, antiwindup, alpha, Ts_mpc, Ts_kf, T, x0, s, Qz, qw, rv, Spsi_spsi, d)
% Example 13: [Y, T, X, U] = mc.kf_qmpc(sysp, sysc, N, r, umin, umax, zmin, zmax, deltaumin, deltaumax, antiwindup, alpha, Ts_mpc, Ts_kf, T, x0, s, Qz, qw, rv, Spsi_spsi, d, E)
% Author: Daniel Mårtensson 2025 Januari 20
% Update: Add separte sample time for both MPC and KF, 2025-03-19

function [Y, T, X, U] = kf_qmpc(varargin)
  % Check if there is any input
  if(isempty(varargin))
	error('Missing inputs')
  end

  % Get system plant model
  if(length(varargin) >= 1)
	sysp = varargin{1};
  else
	error('Missing system plant model');
  end

  % Get system controller model
  if(length(varargin) >= 2)
	sysc = varargin{2};
  else
	error('Missing system controller model');
  end

  % Get horizon
  if(length(varargin) >= 3)
	N = varargin{3};
  else
	error('Missing horizon');
  end

  % Get reference
  if(length(varargin) >= 4)
	r = varargin{4};
  else
	error('Missing reference');
  end

  % Get umin
  if(length(varargin) >= 5)
	umin = varargin{5};
  else
	error('Missing umin');
  end

  % Get umax
  if(length(varargin) >= 6)
	umax = varargin{6};
  else
	error('Missing umax');
  end

  % Get zmin
  if(length(varargin) >= 7)
	zmin = varargin{7};
  else
	error('Missing zmin');
  end

  % Get zmax
  if(length(varargin) >= 8)
	zmax = varargin{8};
  else
	error('Missing zmax');
  end

  % Get udmin
  if(length(varargin) >= 9)
	deltaumin = varargin{9};
  else
	error('Missing deltaumin');
  end

  % Get udmax
  if(length(varargin) >= 10)
	deltaumax = varargin{10};
  else
	error('Missing deltaumax');
  end

  % Get anti-windup
  if(length(varargin) >= 11)
	antiwindup = varargin{11};
  else
	error('Missing anti-windup');
  end

  % Get integral rate
  if(length(varargin) >= 12)
	alpha = varargin{12};
  else
	alpha = 0.01;
  end

  % Get sample time for MPC
  if(length(varargin) >= 13)
	Ts_mpc = varargin{13};
  else
	Ts_mpc = 1;
  end

  % Get sample time for KF
  if(length(varargin) >= 14)
	Ts_kf = varargin{14};
  else
	Ts_kf = 1;
  end

  % Get time
  if(length(varargin) >= 15)
	T = varargin{15};
  else
	T = 10;
  end

  % Get initial state x
  if(length(varargin) >= 16)
	x = varargin{16};
  else
	x = 0;
  end

  % Get regularization parameter s
  if(length(varargin) >= 17)
	s = varargin{17};
  else
	s = 1;
  end

  % Get weight parameter Qz
  if(length(varargin) >= 18)
	Qz = varargin{18};
  else
	Qz = 1;
  end

  % Get kalman disturbance qw
  if(length(varargin) >= 19)
	qw = varargin{19};
  else
	qw = 1;
  end

  % Get kalman noise rv
  if(length(varargin) >= 20)
	rv = varargin{20};
  else
	rv = 1;
  end

  % Get slack parameter Spsi_spsi
  if(length(varargin) >= 21)
	Spsi_spsi = varargin{21};
  else
	Spsi_spsi = 1;
  end

  % Get disturbance
  if(length(varargin) >= 22)
	d = varargin{22};
  else
	d = 0;
  end

  % Get disturbance matrix E
  if(length(varargin) >= 23)
	E = varargin{23};
	Ep = E;
  else
	E = 0;
	Ep = E;
  end

  % Get model type
  typep = sysp.type;
  typec = sysp.type;

  % Check if there is a TF or SS model
  if(and(strcmp(typep, 'SS' ), strcmp(typec, 'SS' )))
	% Get A, B, C, D matrecies from the plant model
	Ap = sysp.A;
	Bp = sysp.B;
	Cp = sysp.C;
	Dp = sysp.D;
	delayp = sysp.delay;

	% Get A, B, C, D matrecies from the controller model
	A = sysc.A;
	B = sysc.B;
	C = sysc.C;
	D = sysc.D;
	delayc = sysc.delay;

	% This is the infinity value for single precision
	infinity = realmax('float');

	% Get all the sizes
	nx = size(A, 1); % Total number of states from system matrix A[nx * nx]
	nu = size(B, 2); % Total number of inputs from control matrix B[nx * nu]
	nz = size(C, 1); % Total number of outputs from observation matrix C[nz * nx]
	nd = size(E, 2); % Total number of disturbances from disturbance matrix E[nx * nd]

	% The disturbance matrix E need to have the same dimension as system matrix A
	if(size(E, 1) ~= nx)
	  E = zeros(nx, nd);
	end

	% Create the discrete matrices - Equation (2.9)
	[Ad, Bd, Cd, Ed] = DiscreteMatrices(A, B, C, E, Ts_mpc);
	[Adkf, Bdkf, Cdkf, Edkf] = DiscreteMatrices(A, B, C, E, Ts_kf);
	[Adp, Bdp, Cdp, Edp] = DiscreteMatrices(Ap, Bp, Cp, Ep, Ts_kf);

	% Create the kalman gain matrix K - Here we use Kalman-Bucy (1961) filter instead of Kalman Filter (1960).
	syse = mc.ss(delayc, Adkf, Bdkf, Cdkf);
	syse.sampleTime = Ts_kf;
	Qw = qw * eye(nx);
	Rv = rv * eye(nz);
	[K] = mc.lqe(syse, Qw, Rv); % This is the same as running the MATLAB command dare for computing kalman-bucy gain K

	% Create the Phi matrix and lower hankel toeplitz Gamma matrix of inputs - Equation (3.6)
	Phi = PhiMat(Ad, Cd, N);
	Gamma = GammaMat(Ad, Bd, Cd, N);

	% Create reference vector - Equation (3.8)
	R = RVec(r, N);

	% Create the weigth matrix - Equation (3.10)
	QZ = QZMat(Qz, N, nz);

	% Create the regularization matrix - Equation (3.21)
	S = SMat(s, nu);
	HS = HSMat(S, N, nu);

	% Create the QP solver H matrix - Equation (3.24)
	H = HMat(Gamma, QZ, HS);

	% Create the lower hankel toeplitz Gamma matrix of disturbance and its disturbance vector - Equation (3.27)
	Gammad = GammaMat(Ad, Ed, Cd, N);
	D = DVec(d, N);

	% Create the QP solver matrix for the gradient - Equation (3.32)
	Mx0 = Mx0Mat(Gamma, QZ, Phi);
	Mum1 = Mum1Mat(N, nu, S);
	MR = MRMat(Gamma, QZ);
	MD = MDMat(Gamma, QZ, Gammad);

	% Create constraints on the movment - Equation (3.38)
	deltaUmin = deltaUminVec(deltaumin, N);
	deltaUmax = deltaUmaxVec(deltaumax, N);
	Lambda = LambdaMat(N, nu);

	% Create constraints on outputs - Equation (3.43)
	Zmin = ZminVec(zmin, N);
	Zmax = ZmaxVec(zmax, N);

	% Create the slack variables - Equation (3.49)
	Spsi = Spsi_spsi;
	spsi = Spsi_spsi;
	barSpsi = barSpsiMat(Spsi, N, nu);
	barspsi = barspsiVec(spsi, N, nu);

	% Create QP solver matrix - Equation (3.51)
	barH = barHMat(H, barSpsi, N, nu);

	% Create inequality constraints AA - Equation (3.56)
	AA = AAMat(Lambda, Gamma, N, nu, nz);

	% Create time vector
	t = 0:Ts_kf:T;
	L = length(t);

	% Create outputs for the simulation
	u = zeros(nu, 1);
	y = zeros(nz, 1);
	um1 = zeros(nu, 1);
	eta = zeros(nz, 1);

	% Create measurement noise
	v = rv * randn(nz, L);

	% The plant state must have the same initial state as the controller state x
	xp = x;

	% Get the disturbance
	disturbance = D(1:nd);

	% Create the output vectors
	X = zeros(nx, L);
	U = zeros(nu, L);
	Y = zeros(nz, L);

	% Check if it's MATLAB or Octave
	isOctave = exist('OCTAVE_VERSION', 'builtin') ~= 0;

	% Simulate the MPC
	for k = 1:L
	  % Return state, input and output
	  X(:,k) = x;
	  U(:,k) = u;
	  Y(:,k) = y;

	  % Give the old u to um1
	  um1 = u;

	  % Integral action - Equation (3.66)
	  psi = r - y;
	  eta = eta + alpha*psi;

	  % Limits for the integral eta
	  if(abs(eta) > antiwindup)
		eta = sign(eta)*antiwindup;
	  end

	  % Create gradient g. Also add the integral eta together with reference vector R for adjust the reference settings - Equation (3.32)
	  % The reason why adjusting the reference R vector is because then the integral action will be optimized inside the QP-solver.
	  Ri = repmat(eta, N, 1);
	  g = gVec(Mx0, x, MR, R + Ri, MD, D, Mum1, um1);

	  % Create constraints on inputs - Equation (3.40)
	  Umin = UminVec(umin, deltaumin, um1, N, nu);
	  Umax = UmaxVec(umax, deltaumax, um1, N, nu);

	  % Create constraints for the output - Equation (3.44)
	  barZmin = barZminVec(Zmin, Phi, x, Gammad, D);
	  barZmax = barZmaxVec(Zmax, Phi, x, Gammad, D);

	  % Create gradient bar g - Equation (3.51)
	  barg = bargVec(g, barspsi);

	  % Create barUmin and barUmax - Equation (3.52)
	  barUmin = barUminVec(Umin, N);
	  barUmax = barUmaxVec(Umax, infinity, N);
	  UI = eye(N * nu + N, 2 * N * nu);

	  % Create bmin and bmax - Equation (3.56)
	  bmin = bminVec(deltaUmin, barZmin, infinity, N, nz);
	  bmax = bmaxVec(deltaUmax, barZmax, infinity, N, nz);

	  % Create for QP - Equation (3.57)
	  % barUmin <= I*U <= barUmax
	  % bmin <= AA*U <= bmax
	  aqp = [UI; AA; -UI; -AA];
	  bqp = [barUmax; bmax; -barUmin; -bmin];

	  % Quadratic programming for propotional action for u
	  if(isOctave == 1)
		[output, ~, e] = qp ([], barH, barg, [], [], [], [], [], aqp, bqp);
		if(e.info == 3)
		  error('Quadratic programming QP could not optimize input signals. Try increase the horizion N number.');
		end
	  else
		[output, solution] = mc.quadprog(barH, barg, aqp, bqp); % Used for MATLAB users
		if(solution == false)
		  error('Quadratic programming quadprog could not optimize input signals. Try to decrease the horizion N number or remove/change lambda regularization. Perhaps increase the slack variable.');
		end
	  end

	  % Set the input signal
	  u = output(1:nu);

	  % Compute outputs - Equation (3.67)
	  y = Cdp*xp + v(:, k);

	  % Compute plant model with the optimized u - Equation (3.65)
	  xp = Adp*xp + Bdp*u + Edp*disturbance;

	  % Update error - Equation (3.72)
	  e = y - Cdkf*x;

	  % Kalman update - Equation (3.75)
	  x = x + K*e;

	  % Compute candidate state x - Equation (3.65)
	  x = Adkf*x + Bdkf*u + Edkf*disturbance;
	end

	%Cange t and y vector and u so the plot look like it is discrete - Important!
	for(i = 1:2:length(Y)*2)
	  leftPart = Y(:,1:i);
	  rightPart = Y(:,(i+1):end);
	  Y = [leftPart Y(:,i) rightPart];
	end

	for(i = 1:2:length(t)*2)
	  leftPart = t(1:i);
	  rightPart = t((i+1):end);
	  t = [leftPart t(i) rightPart];
	end

	for(i = 1:2:length(U)*2)
	  leftPart = U(:,1:i);
	  rightPart = U(:,(i+1):end);
	  U = [leftPart U(:,i) rightPart];
	end

	for(i = 1:2:length(X)*2)
	  leftPart = X(:,1:i);
	  rightPart = X(:,(i+1):end);
	  X = [leftPart X(:,i) rightPart];
	end

	% Just remove the first one
	T = t(:,2:length(t));
	% And the last one
	Y = Y(:,1:(length(Y)-1));
	% And for U and X too
	U = U(:,1:(length(U)-1));
	X = X(:,1:(length(X)-1));
	% Now we have two vectors which look like a discrete signal

	% Plot - How many subplots?
	for i = 1:size(C,1)
	  subplot(size(C,1),1,i)
	  plot(T, Y(i,:));
	  ylabel(strcat('y', num2str(i)));
	  if (Ts_kf > 0)
		xlabel(strcat(num2str(Ts_kf), ' time unit/sample'));
	  else
		xlabel('Time units');
	  end
	  grid on
	end
	title('Model Predictive Control With Integral Action And Quadratic Programming')
  else
	% TF to SS
	if(~strcmp(typep, 'SS' ))
	  sysp = mc.tf2ss(sysp, 'OCF');
	end
	if(~strcmp(typec, 'SS' ))
	  sysc = mc.tf2ss(sysc, 'OCF');
	end
	[Y, T, X, U] = kf_qmpc(sysp, sysc, N, r, umin, umax, zmin, zmax, deltaumin, deltaumax, antiwindup, lambda, Ts_mpc, Ts_kf, T, x0, s, Qz, qw, rv, Spsi_spsi, d, E);
  end
end

function Phi = PhiMat(A, C, N)
  % Create the special Observabillity matrix
  Phi = [];
  for i = 1:N
	Phi = vertcat(Phi, C*A^i);
  end

end

function Gammad = GammadMat(A, B, E, N)
  Gammad = GammaMat(A, B, E, N);
end

function Gamma = GammaMat(A, B, C, N)
  % Create the lower triangular toeplitz matrix
  Gamma = [];
  for i = 1:N
	Gamma = horzcat(Gamma, vertcat(zeros((i-1)*size(C*A*B, 1), size(C*A*B, 2)),cabMat(A, B, C, N-i+1)));
  end
end

function CAB = cabMat(A, B, C, N)
  % Create the column for the Gamma matrix
  CAB = [];
  for i = 0:N-1
	CAB = vertcat(CAB, C*A^i*B);
  end
end

function Mx0 = Mx0Mat(Gamma, QZ, Phi)
  Mx0 = Gamma' * QZ * Phi;
end

function MR = MRMat(Gamma, QZ)
  MR = -Gamma' * QZ;
end

function MD = MDMat(Gamma, QZ, Gammad)
  MD = Gamma' * QZ * Gammad;
end

function Mum1 = Mum1Mat(N, nu, S)
  Mum1 = -[S; zeros((N - 1) * nu, nu)];
end

function QZ = QZMat(Qz, N, nz)
  QZ = zeros(N * nz, N * nz);
  kz = 0;
  for k = 1:N
	QZ(kz + 1:kz + nz, kz + 1:kz + nz) = Qz;
	kz = kz + nz;
  end
end

function Lambda = LambdaMat(N, nu)
  Lambda = zeros((N - 1) * nu, N * nu);
  T = [-eye(nu, nu) eye(nu, nu)];
  for k = 1:N - 1
	Lambda((k - 1) * nu + 1:k * nu, (k - 1) * nu + 1:(k + 1) * nu) = T;
  end
end

function H = HMat(Gamma, QZ, HS)
  H = Gamma' * QZ * Gamma + HS;
end

function S = SMat(s, nu)
  S = s * eye(nu);
end

function barH = barHMat(H, barSpsi, N, nu)
  z = zeros(nu * N, nu * N);
  barH = [H z; z barSpsi];
end

function barSpsi = barSpsiMat(Spsi, N, nu)
  barSpsi = Spsi * eye(nu * N);
end

function AA = AAMat(Lambda, Gamma, N, nu, nz)
  AA = [Lambda zeros((N-1)*nu, nu*N); Gamma -eye(nz*N, nu*N); Gamma eye(nz*N, nu*N)];
end

function HS = HSMat(S, N, nu)
  HS = zeros(N * nu, N * nu);
  if N == 1
	HS = S;
  else
	k = 0;
	HS(1:nu, 1:nu) = 2 * S;
	HS(1 + nu:nu + nu, 1:nu) = -S;

	for k = 1:N - 2
	  ku = k * nu;
	  HS(ku-nu+1:ku,ku+1:ku+nu) = -S;
	  HS(ku+1:ku+nu,ku+1:ku+nu) =2*S;
	  HS(ku+nu+1:ku+2*nu,ku+1:ku+nu) = -S;
	end

	k = N - 1;
	ku = k * nu;
	HS(ku-nu+1:ku,ku+1:ku+nu) = -S;
	HS(ku+1:ku+nu,ku+1:ku+nu) = S;
  end
end

function barspsi = barspsiVec(spsi, N, nu)
  barspsi = spsi * ones(N*nu, 1);
end

function barg = bargVec(g, barspsi)
  barg = [g; barspsi];
end

function g = gVec(Mx0, x0, MR, R, MD, D, Mum1, um1)
  g = Mx0 * x0 + MR * R + MD * D + Mum1 * um1;
end

function R = RVec(r, N)
  R = repmat(r, N, 1);
end

function D = DVec(d, N)
  D = repmat(d, N, 1);
end

function Umin = UminVec(umin, deltaumin, um1, N, nu);
  Umin = repmat(umin, N, 1);
  Umin(1:nu) = max(umin, deltaumin + um1);
end

function Umax = UmaxVec(umax, deltaumax, um1, N, nu);
  Umax = repmat(umax, N, 1);
  Umax(1:nu) = min(umax, deltaumax + um1);
end

function deltaUmin = deltaUminVec(deltaumin, N)
  deltaUmin = repmat(deltaumin, N-1, 1);
end

function deltaUmax = deltaUmaxVec(deltaumax, N)
  deltaUmax = repmat(deltaumax, N-1, 1);
end

function Zmax = ZmaxVec(zmax, N)
  Zmax = repmat(zmax, N, 1);
end

function Zmin = ZminVec(zmin, N)
  Zmin = repmat(zmin, N, 1);
end

function barZmin = barZminVec(Zmin, Phi, x0, Gammad, D)
  barZmin = Zmin - Phi * x0 - Gammad * D;
end

function barZmax = barZmaxVec(Zmax, Phi, x0, Gammad, D)
  barZmax = Zmax - Phi * x0 - Gammad * D;
end

function barUmin = barUminVec(Umin, N)
  barUmin = [Umin; zeros(N, 1)];
end

function barUmax = barUmaxVec(Umax, infinity, N)
  barUmax = [Umax; infinity * ones(N, 1)];
end

function bmin = bminVec(deltaUmin, barZmin, infinity, N, nz)
  bmin = [deltaUmin; -infinity * ones(N*nz, 1); barZmin];
end

function bmax = bmaxVec(deltaUmax, barZmax, infinity, N, nz)
  bmax = [deltaUmax; barZmax; infinity * ones(N*nz, 1)];
end

function [Ad, Bd, Cd, Ed] = DiscreteMatrices(A, B, C, E, Ts)
  nx = size(A, 1);
  nu = size(B, 2);
  nd = size(E, 2);
  M1 = [A B E; zeros(nu + nd, nx + nu + nd)];
  M2 = expm(M1 * Ts);
  Ad = M2(1:nx, 1:nx);
  Bd = M2(1:nx, nx + 1:nx + nu);
  Ed = M2(1:nx, nx + nu + 1:nx + nu + nd);
  Cd = C;
end
*/
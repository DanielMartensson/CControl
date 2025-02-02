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
	eye(S, s, column_b, column_b);
}

/*
 * HS[(N * column_b) * (N * column_b)]
 * S[column_b * column_b]
 */
void mpc_HS_matrix(float HS[], const float S[], const size_t column_b, const size_t N) {
	/* Positive 2*S */
	float* p2S = (float*)malloc(column_b * column_b * sizeof(float));
	eye(p2S, 2.0f*S[0], column_b, column_b);

	/* Negative -2*S */
	float* m2S = (float*)malloc(column_b * column_b * sizeof(float));
	eye(m2S, -2.0f*S[0], column_b, column_b);

	/* Negative S */
	float* mS = (float*)malloc(column_b * column_b * sizeof(float));
	eye(mS, -1.0f*S[0], column_b, column_b);

	/* Fill */
	if (N == 1) {
		memset(HS, S, column_b * column_b * sizeof(float));
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
 * barspsi[N]
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
void mpc_eta_vector(float eta[], const float r[], const float y[], const float lambda, const size_t row_c) {
	size_t i;
	float* psi = (float*)malloc(row_c * sizeof(float));
	for (i = 0; i < row_c; i++) {
		psi[i] = r[i] - y[i];
		eta[i] = eta[i] + lambda * psi[i];
	}

	/* Free */
	free(psi);
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
 * x[row_a]
 * Ad[row_a * row_a]
 * Bd[row_a * column_b]
 * Ed[row_a * column_e]
 * u[column_b]
 * d[column_e]
 */
void mpc_stateupdate_vector(float x[], const float Ad[], const float Bd[], const float Ed[], const float u[], const float d[], const size_t row_a, const size_t column_b, const size_t column_e) {
	float* Adx = (float*)malloc(row_a * sizeof(float));
	mul(Ad, x, Adx, row_a, row_a, 1);
	float* Bdu = (float*)malloc(row_a * sizeof(float));
	mul(Bd, u, Bdu, row_a, column_b, 1);
	float* Edd = (float*)malloc(row_a * sizeof(float));
	mul(Ed, d, Edd, row_a, column_e, 1);
	size_t i;
	for(i = 0; i < row_a; i++){
		x[i] = Adx[i] + Bdu[i] + Edd[i];
	}

	/* Free */
	free(Adx);
	free(Bdu);
	free(Edd);
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
	for (i = 0; i <  N * row_c; i++) {
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
	float* UI = (float*)malloc((N * column_b + N) * (2 * N * column_b) * sizeof(float));
	eye(UI, 1.0f, N * column_b + N, 2 * N * column_b);
	size_t insert_row = 0;
	insert(UI, aqp, N * column_b + N, 2 * N * column_b, 2 * N * column_b, insert_row, 0);
	insert_row += N * column_b + N;
	insert(AA, aqp, (N - 1) * column_b + 2 * N * row_c, 2 * N * column_b, 2 * N * column_b, insert_row, 0);
	insert_row += (N - 1) * column_b + 2 * N * row_c;
	eye(UI, -1.0f, N * column_b + N, 2 * N * column_b);
	insert(UI, aqp, N * column_b + N, 2 * N * column_b, 2 * N * column_b, insert_row, 0);
	insert_row += N * column_b + N;
	size_t i;
	for (i = 0; i < ((N - 1) * column_b + 2 * N * row_c) * (2 * N * column_b); i++) {
		aqp[insert_row * 2 * N * column_b + i] = -AA[i];
	}

	/* Free */
	free(UI);
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
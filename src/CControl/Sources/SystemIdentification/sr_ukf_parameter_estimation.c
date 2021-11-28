/*
 * sr_ukf_parameter_estimation.c
 *
 *  Created on: 27 nov. 2021
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

static void create_weights(float Wc[], float Wm[], float alpha, float beta, float kappa, uint8_t L);
static void scale_Sw_with_lambda_rls_factor(float Sw[], float lambda_rls, uint8_t L);
static void create_sigma_point_matrix(float W[], float w[], float Sw[], float alpha, float kappa, uint8_t L);
static void multiply_sigma_point_matrix_to_weights(float dhat[], float D[], float Wm[], uint8_t L);
static void create_state_estimation_error_covariance_matrix(float Sd[], float Wc[], float D[], float dhat[], float Re[], uint8_t L);
static void create_state_cross_covariance_matrix(float Pwd[], float Wc[], float W[], float D[], float what[], float dhat[], uint8_t L);
static void update_state_covarariance_matrix_and_state_estimation_vector(float Sw[], float what[], float dhat[], float d[], float Sd[], float Pwd[], uint8_t L);


/*
 * Square Root Unscented Kalman Filter For Parameter Estimation§§
 * L = Number of states, or sensors in practice.§§§
 * kappa = 0 for state estimation, 3 - L for parameter estimation
 * beta = used to incorporate prior knowledge of the distribution of x (for Gaussian distributions, beta = 2 is optimal)
 * alpha = determines the spread of the sigma points around xhat and alpha is usually set to 0.0001 <= alpha <= 1
 * Sw[L * L] = Parameter estimate error covariance
 * lambda_rls = Scalar factor 0 <= lambda_rls <= 1. A good number is close to 1 like 0.995
 * G(D[L * (2 * L + 1)], x[L], W[L * (2 * L + 1)], uint8_t L) = Model
 * x[L] = State vector
 * Re[L * L] = Measurement noise covariance matrix
 * what[L] = Estimated parameter (our input)
 * d[L] = Measurement parameter (our output)
 */
void sr_ukf_parameter_estimation(float d[], float what[], float Re[], float Rv[], float x[], void (*G)(float[], float[], float[], uint8_t), float lambda_rls, float Sw[], float alpha, float beta, float kappa, uint8_t L){
	/* Create the size N */
	uint8_t N = 2 * L + 1;

	/* Predict: Create the weights */
	float Wc[N];
	float Wm[N];
	create_weights(Wc, Wm, alpha, beta, kappa, L);

	/* Predict: Scale Sw with lambda_rls */
	scale_Sw_with_lambda_rls_factor(Sw, lambda_rls, L);

	/* Predict: Create sigma point matrix for G function  */
	float W[L * N];
	create_sigma_point_matrix(W, what, Sw, alpha, kappa, L);

	/* Predict: Compute the model F */
	float D[L * N];
	G(D, x, W, L);

	/* Predict: Multiply sigma points to weights for dhat */
	float dhat[L];
	multiply_sigma_point_matrix_to_weights(dhat, D, Wm, L);

	/* Update: Create measurement covariance matrix */
	float Sd[L * L];
	create_state_estimation_error_covariance_matrix(Sd, Wc, D, dhat, Re, L);

	/* Update: Create parameter covariance matrix */
	float Pwd[L * L];
	create_state_cross_covariance_matrix(Pwd, Wc, W, D, what, dhat, L);

	/* Update: Perform parameter update and covariance update */
	update_state_covarariance_matrix_and_state_estimation_vector(Sw, what, dhat, d, Sd, Pwd, L);
}

static void create_weights(float Wc[], float Wm[], float alpha, float beta, float kappa, uint8_t L){
	/* Create the size N */
	uint8_t N = 2 * L + 1;

	/* Compute lambda and gamma parameters */
	float lambda = alpha * alpha * (L + kappa) - L;

	/* Insert at first index */
	Wc[0] = lambda/(L + lambda) + 1 - alpha * alpha + beta;
	Wm[0] = lambda/(L + lambda);

	/* The rest of the indexes are the same */
	for(uint8_t i = 1; i < N; i++){
		Wc[i] = 1/(2 * (L + lambda));
		Wm[i] = Wc[i];
	}
}

static void scale_Sw_with_lambda_rls_factor(float Sw[], float lambda_rls, uint8_t L){
	/* Create the size M */
	uint8_t M = 2 * L;

	/* Apply scalar factor to Sw */
	for(uint8_t i = 0; i < M; i++)
		Sw[i] *= 1/sqrtf(lambda_rls);
}

static void create_sigma_point_matrix(float W[], float w[], float Sw[], float alpha, float kappa, uint8_t L){
	/* Create the size N */
	uint8_t N = 2 * L + 1;

	/* Compute lambda and gamma parameters */
	float lambda = alpha * alpha * (L + kappa) - L;
	float gamma = sqrtf(L + lambda);

	/* Insert first column in W */
	for(uint8_t i = 0; i < L; i++)
		W[i * N] = w[i];

	/* Insert in to the middle of the columns - Positive */
	for(uint8_t j = 1; j < L + 1; j++)
		for(uint8_t i = 0; i < L; i++)
			W[i * N + j] = w[i] + gamma * Sw[i * L + j - 1];

	/* Insert in the rest of the columns - Negative */
	for(uint8_t j = L + 1; j < N; j++)
		for(uint8_t i = 0; i < L; i++)
			W[i * N + j] = w[i] - gamma * Sw[i * L + j - 1 - L];

}

static void multiply_sigma_point_matrix_to_weights(float dhat[], float D[], float Wm[], uint8_t L){
	/* Create the size N */
	uint8_t N = 2 * L + 1;

	/* Clear dhat */
	memset(dhat, 0, L * sizeof(float));

	/* Multiply x = W*X */
	for(uint8_t j = 0; j < N; j++)
		for(uint8_t i = 0; i < L; i++)
			dhat[i] += Wm[i] * D[i * N + j];
}

static void create_state_estimation_error_covariance_matrix(float Sd[], float Wc[], float D[], float dhat[], float Re[], uint8_t L){
	/* Create the size N, M and K */
	uint8_t N = 2 * L + 1;
	uint8_t M = 3 * L;
	uint8_t K = 2 * L;

	/* Square root parameter of index 1 */
	float weight1 = sqrtf(Wc[1]);
	float weight0 = sqrtf(Wc[0]);

	/* Create [Q, R_] = qr(A') */
	float AT[L * M];
	float Q[M * M];
	float R_[M * L];
	uint8_t j = 0;
	for(; j < K; j++)
		for(uint8_t i = 0; i < L; i++)
			AT[i*M + j] = weight1 * (D[i * N + j + 1] - dhat[i]);
	for(; j < M; j++)
		for(uint8_t i = 0; i < L; i++)
			AT[i*M + j] = Re[i * L + j - K];

	/* We need to do transpose on A according to the SR-UKF paper */
	tran(AT, L, M);

	/* Solve [Q, R_] = qr(A') */
	qr(AT, Q, R_, M, L);

	/* Get the upper triangular of R_ according to the SR-UKF paper */
	memcpy(Sd, R_, L * L * sizeof(float));

	/* Perform cholesky update on Sd */
	float b[L];
	for(uint8_t i = 0; i < L; i++)
		b[i] = D[i * N] - dhat[i];
	bool rank_one_update = weight0 < 0 ? false : true;
	cholupdate(Sd, b, L, rank_one_update);
}

static void create_state_cross_covariance_matrix(float Pwd[], float Wc[], float W[], float D[], float what[], float dhat[], uint8_t L){
	/* Create the size N and K */
	uint8_t N = 2 * L + 1;
	uint8_t K = 2 * L;

	/* clear Pwd */
	memset(Pwd, 0, K * sizeof(float));

	for(uint8_t j = 0; j < N; j++){
		/* Create the vectors and the matrix */
		float a[L];
		float b[L];
		float c[K];

		/* Fill the vectors */
		for(uint8_t i = 0; i < L; i++){
			a[j] = W[i * N + j] - what[i];
			b[j] = D[i * N + j] - dhat[i];
		}

		/* Compute the matrix - We don't need to take transpose on b in this C code because vector^T = vector in C*/
		mul(a, b, c, L, 1, L);

		/* Do the sum */
		for(uint8_t k = 0; k < K; k++)
			Pwd[k] += Wc[j] * c[k];
	}
}

static void update_state_covarariance_matrix_and_state_estimation_vector(float Sw[], float what[], float dhat[], float d[], float Sd[], float Pwd[], uint8_t L){
	/* Transpose of Syd*/
	float SdT[L * L];
	memcpy(SdT, Sd, L * L * sizeof(float));
	tran(SdT, L, L);

	/* Multiply Sd and Sd' to Sd'Sd */
	float SdTSd[L * L];
	mul(SdT, Sd, SdTSd, L, L, L);

	/* Compute kalman gain K from Sd'Sd * K = Pwd */
	float K[L * L];
	for(uint8_t j = 0; j < L; j++){
		/* Fill b vector */
		float b[L];
		for(uint8_t i = 0; i < L; i++)
			b[i] = SdTSd[i * L + j];

		/* Compute x */
		float x[L];
		linsolve_lup(Pwd, x, b, L);

		/* Fill K matrix */
		for(uint8_t i = 0; i < L; i++)
			K[i * L + j] = b[i];
	}

	/* Compute what = what + K*(d - dhat) */
	float ddhat[L];
	float Kd[L];
	for(uint8_t i = 0; i < L; i++)
		ddhat[i] = d[i] - dhat[i];
	mul(K, ddhat, Kd, L, L, 1);
	for(uint8_t i = 0; i < L; i++)
		what[i] = what[i] + Kd[i];

	/* Compute U = K*Sd */
	float U[L];
	mul(K, Sd, U, L, 1, L);

	/* Compute Sw = cholupdate(Sw, U, -1) */
	cholupdate(Sw, U, L, false);
}

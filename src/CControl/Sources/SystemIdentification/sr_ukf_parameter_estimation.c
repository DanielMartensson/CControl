/*
 * sr_ukf_parameter_estimation.c
 *
 *  Created on: 27 nov. 2021
 *      Author: Daniel Mårtensson
 */

#include "systemidentification.h"

static void create_weights(float Wc[], float Wm[], const float alpha, const float beta, const float kappa, const size_t L);
static void scale_Sw_with_lambda_rls_factor(float Sw[], const float lambda_rls, const size_t L);
static void create_sigma_point_matrix(float W[], const float what[], const float Sw[], const float alpha, const float kappa, const size_t L);
static void compute_transistion_function(float D[], const float W[], const float x[], void (*G)(float[], const float[], const float[]), const size_t L);
static void multiply_sigma_point_matrix_to_weights(float dhat[], const float D[], const float Wm[], const size_t L);
static void create_state_estimation_error_covariance_matrix(float Sd[], const float Wc[], const float D[], const float dhat[], const float Re[], const size_t L);
static void create_state_cross_covariance_matrix(float Pwd[], const float Wc[], float W[], float D[], const float what[], const float dhat[], const size_t L);
static void update_state_covarariance_matrix_and_state_estimation_vector(float Sw[], float what[], float dhat[], const float d[], const float Sd[], const float Pwd[], const size_t L);

/*
 * Square Root Unscented Kalman Filter For Parameter Estimation (A better version than regular UKF)
 * L = Number of states, or sensors in practice.
 * beta = used to incorporate prior knowledge of the distribution of x (for Gaussian distributions, beta = 2 is optimal)
 * alpha = determines the spread of the sigma points around what and alpha is usually set to 0.01 <= alpha <= 1
 * Sw[L * L] = Parameter estimate error covariance
 * lambda_rls = Scalar factor 0 <= lambda_rls <= 1. A good number is close to 1 like 0.995
 * G(float dw[L], float x[L], float w[L]) = Transition function with unknown parameters
 * x[L] = State vector
 * w[L] = Parameter vector
 * Re[L * L] = Measurement noise covariance matrix
 * what[L] = Estimated parameter (our input)
 * d[L] = Measurement parameter (our output)
 */
void sr_ukf_parameter_estimation(const float d[], float what[], const float Re[], const float x[], void (*G)(float[], const float[], const float[]), const float lambda_rls, float Sw[], const float alpha, const float beta, const size_t L){
	/* Create the size N */
	const size_t N = 2 * L + 1;

	/* Predict: Create the weights */
	float* Wc = (float*)malloc(N * sizeof(float));
	float* Wm = (float*)malloc(N * sizeof(float));
	const float kappa = 3.0f - L; /* kappa is 3 - L for parameter estimation */
	create_weights(Wc, Wm, alpha, beta, kappa, L);

	/* Predict: Scale Sw with lambda_rls */
	scale_Sw_with_lambda_rls_factor(Sw, lambda_rls, L);

	/* Predict: Create sigma point matrix for G function  */
	float* W = (float*)malloc(L * N * sizeof(float));
	create_sigma_point_matrix(W, what, Sw, alpha, kappa, L);

	/* Predict: Compute the model G */
	float* D = (float*)malloc(L * N * sizeof(float));
	compute_transistion_function(D, W, x, G, L);

	/* Predict: Multiply sigma points to weights for dhat */
	float* dhat = (float*)malloc(L * sizeof(float));
	multiply_sigma_point_matrix_to_weights(dhat, D, Wm, L);

	/* Update: Create measurement covariance matrix */
	float *Sd = (float*)malloc(L * L * sizeof(float));
	create_state_estimation_error_covariance_matrix(Sd, Wc, D, dhat, Re, L);

	/* Update: Create parameter covariance matrix */
	float* Pwd = (float*)malloc(L * L * sizeof(float));
	create_state_cross_covariance_matrix(Pwd, Wc, W, D, what, dhat, L);

	/* Update: Perform parameter update and covariance update */
	update_state_covarariance_matrix_and_state_estimation_vector(Sw, what, dhat, d, Sd, Pwd, L);

	/* Free */
	free(Wc);
	free(Wm);
	free(W);
	free(D);
	free(dhat);
	free(Sd);
	free(Pwd);
}

static void create_weights(float Wc[], float Wm[], const float alpha, const float beta, const float kappa, const size_t L){
	/* Decleration */
	size_t i;
	
	/* Create the size N */
	const size_t N = 2 * L + 1;

	/* Compute lambda and gamma parameters */
	const float lambda = alpha * alpha * (L + kappa) - L;

	/* Insert at first index */
	Wm[0] = lambda/(L + lambda);
	Wc[0] = Wm[0] + 1 - alpha * alpha + beta;

	/* The rest of the indexes are the same */
	for(i = 1; i < N; i++){
		Wc[i] = 0.5f / (L + lambda);
		Wm[i] = Wc[i];
	}
}

static void scale_Sw_with_lambda_rls_factor(float Sw[], const float lambda_rls, const size_t L){
	/* Decleration */
	size_t i;
	
	/* Create the size M */
	const size_t M = 2 * L;

	/* Apply scalar factor to Sw */
	for (i = 0; i < M; i++) {
		Sw[i] *= 1.0f / sqrtf(lambda_rls);
	}
}

static void create_sigma_point_matrix(float W[], const float what[], const float Sw[], const float alpha, const float kappa, const size_t L){
	/* Decleration */
	size_t i, j;
	
	/* Create the size N and K */
	const size_t N = 2 * L + 1;
	const size_t K = L + 1;

	/* Compute lambda and gamma parameters */
	const float lambda = alpha * alpha * (L + kappa) - L;
	const float gamma = sqrtf(L + lambda);

	/* Insert first column in W */
	for (i = 0; i < L; i++) {
		W[i * N] = what[i];
	}

	/* Insert in to the middle of the columns - Positive */
	for (j = 1; j < K; j++) {
		for (i = 0; i < L; i++) {
			W[i * N + j] = what[i] + gamma * Sw[i * L + j - 1];
		}
	}

	/* Insert in the rest of the columns - Negative */
	for (j = K; j < N; j++) {
		for (i = 0; i < L; i++) {
			W[i * N + j] = what[i] - gamma * Sw[i * L + j - K];
		}
	}

}

static void compute_transistion_function(float D[], const float W[], const float x[], void (*G)(float[], const float[], const float[]), const size_t L){
	/* Decleration */
	size_t i, j;
	
	/* Create the size N */
	const size_t N = 2 * L + 1;

	/* Create the derivative state and state vector */
	float* dw = (float*)malloc(L * sizeof(float));
	float* w = (float*)malloc(L * sizeof(float));

	/* Call the F transition function with W matrix */
	for(j = 0; j < N; j++){
		/* Fill the state vector w with every row from W */
		for (i = 0; i < L; i++) {
			w[i] = W[i * N + j];
		}

		/* Call the transition function */
		G(dw, x, w);

		/* Get dw into D */
		for (i = 0; i < L; i++) {
			D[i * N + j] = dw[i];
		}
	}

	free(dw);
	free(w);
}

static void multiply_sigma_point_matrix_to_weights(float dhat[], const float D[], const float Wm[], const size_t L){
	/* Decleration */
	size_t i, j;
	
	/* Create the size N */
	const size_t N = 2 * L + 1;

	/* Clear dhat */
	memset(dhat, 0, L * sizeof(float));

	/* Multiply dhat = Wm*D */
	for (j = 0; j < N; j++) {
		for (i = 0; i < L; i++) {
			dhat[i] += Wm[j] * D[i * N + j];
		}
	}

}

static void create_state_estimation_error_covariance_matrix(float Sd[], const float Wc[], const float D[], const float dhat[], const float Re[], const size_t L){
	/* Decleration */
	size_t i, j;
	
	/* Create the size N, M and K */
	const size_t N = 2 * L + 1;
	const size_t M = 2 * L + L;
	const size_t K = 2 * L;

	/* Square root parameter of index 1 */
	const float weight1 = sqrtf(fabsf(Wc[1]));

	/* Create [Q, R_] = qr(A') */
	float* AT = (float*)malloc(L * M * sizeof(float));
	float* Q = (float*)malloc(M * M * sizeof(float));
	float* R = (float*)malloc(M * L * sizeof(float));
	for(j = 0; j < K; j++){
		for(i = 0; i < L; i++){
			AT[i*M + j] = weight1 * (D[i * N + j+1] - dhat[i]);
		}
	}
	for (j = K; j < M; j++) {
		for (i = 0; i < L; i++) {
			AT[i * M + j] = sqrtf(Re[i * L + j - K]);
		}
	}

	/* We need to do transpose on A according to the SR-UKF paper */
	tran(AT, L, M);

	/* Solve [Q, R] = qr(A') but we only need R matrix */
	qr(AT, Q, R, M, L, true);

	/* Get the upper triangular of R according to the SR-UKF paper */
	memcpy(Sd, R, L * L * sizeof(float));

	/* Perform cholesky update on Sd */
	float* b = (float*)malloc(L * sizeof(float));
	for (i = 0; i < L; i++) {
		b[i] = D[i * N] - dhat[i];
	}
	const bool rank_one_update = Wc[0] < 0.0f ? false : true;
	cholupdate(Sd, b, L, rank_one_update);

	/* Free */
	free(AT);
	free(Q);
	free(R);
	free(b);
}

static void create_state_cross_covariance_matrix(float Pwd[], const float Wc[], float W[], float D[], const float what[], const float dhat[], const size_t L){
	/* Decleration */
	size_t i, j;
	
	/* Create the size N and K */
	const size_t N = 2 * L + 1;
	const size_t K = 2 * L;

	/* clear P */
	memset(Pwd, 0, K * sizeof(float));

	/* Subtract the matrices */
	for(j = 0; j < N; j++){
		for(i = 0; i < L; i++){
			W[i * N + j] -= what[i];
			D[i * N + j] -= dhat[i];
		}
	}

	/* Create diagonal matrix */
	float* diagonal_W = (float*)malloc(N * N * sizeof(float));
	memset(diagonal_W, 0, N*N*sizeof(float));
	for (i = 0; i < N; i++) {
		diagonal_W[i * N + i] = Wc[i];
	}

	/* Do Pwd = W*diagonal_W*D' */
	tran(D, L, N);
	float* diagonal_WD = (float*)malloc(N * L * sizeof(float));
	mul(diagonal_W, D, diagonal_WD, N, N, L);
	mul(W, diagonal_WD, Pwd, L, N, L);

	/* Free */
	free(diagonal_W);
	free(diagonal_WD);
}

/* Sw, what, dhat, d, Sd, Pwd, L */
static void update_state_covarariance_matrix_and_state_estimation_vector(float Sw[], float what[], float dhat[], const float d[], const float Sd[], const float Pwd[], const size_t L){
	/* Decleration */
	size_t i, j;
	
	/* Transpose of Sd */
	float* SdT = (float*)malloc(L * L * sizeof(float));
	memcpy(SdT, Sd, L * L * sizeof(float));
	tran(SdT, L, L);

	/* Multiply Sd and Sd' to Sd'Sd */
	float* SdTSd = (float*)malloc(L * L * sizeof(float));
	mul(SdT, Sd, SdTSd, L, L, L);

	/* Take inverse of Sd'Sd - Inverse is using LUP-decomposition */
	inv(SdTSd, L);

	/* Compute kalman gain K from Sd'Sd * K = Pwd => K = Pwd * inv(SdTSd) */
	float* K = (float*)malloc(L * L * sizeof(float));
	mul(Pwd, SdTSd, K, L, L, L);

	/* Compute what = what + K*(d - dhat) */
	float* ddhat = (float*)malloc(L * sizeof(float));
	float* Kd = (float*)malloc(L * sizeof(float));
	for (i = 0; i < L; i++) {
		ddhat[i] = d[i] - dhat[i];
	}
	mul(K, ddhat, Kd, L, L, 1);
	for (i = 0; i < L; i++) {
		what[i] = what[i] + Kd[i];
	}

	/* Compute U = K*Sd */
	float* U = (float*)malloc(L * L * sizeof(float));
	mul(K, Sd, U, L, L, L);

	/* Compute Sw = cholupdate(Sw, Uk, -1) because Uk is a vector and U is a matrix */
	float *Uk = (float*)malloc(L * sizeof(float));
	for(j = 0; j < L; j++){
		for (i = 0; i < L; i++) {
			Uk[i] = U[i * L + j];
		}
		cholupdate(Sw, Uk, L, false);
	}

	/* Free */
	free(SdT);
	free(SdTSd);
	free(K);
	free(ddhat);
	free(Kd);
	free(U);
	free(Uk);
}

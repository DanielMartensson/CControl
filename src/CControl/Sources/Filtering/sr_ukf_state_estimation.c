/*
 * sr_ukf_state_estimation.c
 *
 *  Created on: Nov 22, 2021
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

static void create_weights(float Wc[], float Wm[], float alpha, float beta, float kappa, uint8_t L);
static void create_sigma_point_matrix(float X[], float x[], float S[], float alpha, float kappa, uint8_t L);
static void multiply_sigma_point_matrix_to_weights(float x[], float X[], float W[], uint8_t L);
static void create_state_estimation_error_covariance_matrix(float S[], float W[], float X[], float x[], float R[], uint8_t L);
static void H(float Y[], float X[], uint8_t L);
static void create_state_cross_covariance_matrix(float P[], float W[], float X[], float Y[], float x[], float y[], uint8_t L);
static void update_state_covarariance_matrix_and_state_estimation_vector(float S[], float xhat[], float yhat[], float y[], float Sy[], float Pxy[], uint8_t L);

/*
 * Square Root Unscented Kalman Filter For State Estimation (A better version than regular UKF)
 * L = Number of states, or sensors in practice.
 * kappa = 0 for state estimation, 3 - L for parameter estimation
 * beta = used to incorporate prior knowledge of the distribution of x (for Gaussian distributions, beta = 2 is optimal)
 * alpha = determines the spread of the sigma points around xhat and alpha is usually set to 0.0001 <= alpha <= 1
 * S[L * L] = State estimate error covariance
 * F(float Xstar[L * (2 * L + 1)], float X[L * (2 * L + 1)], float u[L], uint8_t L) = Transition function
 * u[L] = Input signal
 * Rv[L * L] = Process noise covariance matrix
 * Rn[L * L] = Measurement noise covariance matrix
 * xhat[L] = Estimated state (our input)
 * y[L] = Measurement state (our output)
 */
void sr_ukf_state_estimation(float y[], float xhat[], float Rn[], float Rv[], float u[], void (*F)(float[], float[], float[], uint8_t), float S[], float alpha, float beta, float kappa, uint8_t L){
	/* Create the size N */
	uint8_t N = 2 * L + 1;

	/* Predict: Create the weights */
	float Wc[N];
	float Wm[N];
	create_weights(Wc, Wm, alpha, beta, kappa, L);

	/* Predict: Create sigma point matrix for F function  */
	float X[L * N];
	create_sigma_point_matrix(X, xhat, S, alpha, kappa, L);

	/* Predict: Compute the transition function F */
	float Xstar[L * N];
	F(Xstar, X, u, L);

	/* Predict: Multiply sigma points to weights for xhat */
	multiply_sigma_point_matrix_to_weights(xhat, Xstar, Wc, L);

	/* Predict: Create state estimate error covariance  */
	create_state_estimation_error_covariance_matrix(S, Wc, Xstar, xhat, Rv, L);

	/* Predict: Create sigma point matrix for H function. This is the updated version of SR-UKF paper. The old SR-UKF paper don't have this */
	float Y[L * N];
	create_sigma_point_matrix(Y, xhat, S, alpha, kappa, L);

	/* Predict: Compute the observability function H */
	H(Y, X, L);

	/* Predict: Multiply sigma points to weights for yhat_ */
	float yhat[L];
	multiply_sigma_point_matrix_to_weights(yhat, Y, Wm, L);

	/* Update: Create measurement covariance matrix */
	float Sy[L * L];
	create_state_estimation_error_covariance_matrix(Sy, Wc, Y, yhat, Rn, L);

	/* Update: Create state covariance matrix */
	float Pxy[L * L];
	create_state_cross_covariance_matrix(Pxy, Wc, X, Y, xhat, yhat, L);

	/* Update: Perform state update and covariance update */
	update_state_covarariance_matrix_and_state_estimation_vector(S, xhat, yhat, y, Sy, Pxy, L);
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

static void create_sigma_point_matrix(float X[], float x[], float S[], float alpha, float kappa, uint8_t L){
	/* Create the size N */
	uint8_t N = 2 * L + 1;

	/* Compute lambda and gamma parameters */
	float lambda = alpha * alpha * (L + kappa) - L;
	float gamma = sqrtf(L + lambda);

	/* Insert first column in X */
	for(uint8_t i = 0; i < L; i++)
		X[i * N] = x[i];

	/* Insert in to the middle of the columns - Positive */
	for(uint8_t j = 1; j < L + 1; j++)
		for(uint8_t i = 0; i < L; i++)
			X[i * N + j] = x[i] + gamma * S[i * L + j - 1];

	/* Insert in the rest of the columns - Negative */
	for(uint8_t j = L + 1; j < N; j++)
		for(uint8_t i = 0; i < L; i++)
			X[i * N + j] = x[i] - gamma * S[i * L + j - 1 - L];

}

static void multiply_sigma_point_matrix_to_weights(float x[], float X[], float W[], uint8_t L){
	/* Create the size N */
	uint8_t N = 2 * L + 1;

	/* Clear x */
	memset(x, 0, L * sizeof(float));

	/* Multiply x = W*X */
	for(uint8_t j = 0; j < N; j++)
		for(uint8_t i = 0; i < L; i++)
			x[i] += W[i] * X[i * N + j];
}

static void create_state_estimation_error_covariance_matrix(float S[], float W[], float X[], float x[], float R[], uint8_t L){
	/* Create the size N, M and K */
	uint8_t N = 2 * L + 1;
	uint8_t M = 3 * L;
	uint8_t K = 2 * L;

	/* Square root parameter of index 1 */
	float weight1 = sqrtf(W[1]);
	float weight0 = sqrtf(W[0]);

	/* Create [Q, R_] = qr(A') */
	float AT[L * M];
	float Q[M * M];
	float R_[M * L];
	uint8_t j = 0;
	for(; j < K; j++)
		for(uint8_t i = 0; i < L; i++)
			AT[i*M + j] = weight1 * (X[i * N + j + 1] - x[i]);
	for(; j < M; j++)
		for(uint8_t i = 0; i < L; i++)
			AT[i*M + j] = R[i * L + j - K];

	/* We need to do transpose on A according to the SR-UKF paper */
	tran(AT, L, M);

	/* Solve [Q, R_] = qr(A') */
	qr(AT, Q, R_, M, L);

	/* Get the upper triangular of R_ according to the SR-UKF paper */
	memcpy(S, R_, L * L * sizeof(float));

	/* Perform cholesky update on S */
	float b[L];
	for(uint8_t i = 0; i < L; i++)
		b[i] = X[i * N] - x[i];
	bool rank_one_update = weight0 < 0 ? false : true;
	cholupdate(S, b, L, rank_one_update);
}

static void H(float Y[], float X[], uint8_t L){
	/* Create the size N */
	uint8_t N = 2 * L + 1;

	/* Compute Y = I*X where I is identity matrix */
	memcpy(Y, X, L * N * sizeof(float));
}

static void create_state_cross_covariance_matrix(float P[], float W[], float X[], float Y[], float x[], float y[], uint8_t L){
	/* Create the size N and K */
	uint8_t N = 2 * L + 1;
	uint8_t K = 2 * L;

	/* clear P */
	memset(P, 0, K * sizeof(float));

	for(uint8_t j = 0; j < N; j++){
		/* Create the vectors and the matrix */
		float a[L];
		float b[L];
		float c[K];

		/* Fill the vectors */
		for(uint8_t i = 0; i < L; i++){
			a[j] = X[i * N + j] - x[i];
			b[j] = Y[i * N + j] - y[i];
		}

		/* Compute the matrix - We don't need to take transpose on b in this C code because vector^T = vector in C*/
		mul(a, b, c, L, 1, L);

		/* Do the sum */
		for(uint8_t k = 0; k < K; k++)
			P[k] += W[j] * c[k];
	}
}

static void update_state_covarariance_matrix_and_state_estimation_vector(float S[], float xhat[], float yhat[], float y[], float Sy[], float Pxy[], uint8_t L){
	/* Transpose of Sy */
	float SyT[L * L];
	memcpy(SyT, Sy, L * L * sizeof(float));
	tran(SyT, L, L);

	/* Multiply Sy and Sy' to Sy'Sy */
	float SyTSy[L * L];
	mul(SyT, Sy, SyTSy, L, L, L);

	/* Compute kalman gain K from Sy'Sy * K = Pxy */
	float K[L * L];
	for(uint8_t j = 0; j < L; j++){
		/* Fill b vector */
		float b[L];
		for(uint8_t i = 0; i < L; i++)
			b[i] = SyTSy[i * L + j];

		/* Compute x */
		float x[L];
		linsolve_lup(Pxy, x, b, L);

		/* Fill K matrix */
		for(uint8_t i = 0; i < L; i++)
			K[i * L + j] = b[i];
	}

	/* Compute xhat = xhat + K*(y - yhat) */
	float yyhat[L];
	float Ky[L];
	for(uint8_t i = 0; i < L; i++)
		yyhat[i] = y[i] - yhat[i];
	mul(K, yyhat, Ky, L, L, 1);
	for(uint8_t i = 0; i < L; i++)
		xhat[i] = xhat[i] + Ky[i];

	/* Compute U = K*Sy */
	float U[L];
	mul(K, Sy, U, L, 1, L);

	/* Compute S = cholupdate(S, U, -1) */
	cholupdate(S, U, L, false);
}
/*
 * sr_ukf_state_estimation.c
 *
 *  Created on: Nov 22, 2021
 *      Author: Daniel Mårtensson
 */

#include "signalprocessing.h"

static void create_weights(float Wc[], float Wm[], float alpha, float beta, float kappa, size_t L);
static void create_sigma_point_matrix(float X[], float x[], float S[], float alpha, float kappa, size_t L);
static void compute_transistion_function(float Xstar[], float X[], float u[], void (*F)(float[], float[], float[]), size_t L);
static void multiply_sigma_point_matrix_to_weights(float x[], float X[], float W[], size_t L);
static void create_state_estimation_error_covariance_matrix(float S[], float W[], float X[], float x[], float R[], size_t L);
static void H(float Y[], float X[], size_t L);
static void create_state_cross_covariance_matrix(float P[], float W[], float X[], float Y[], float x[], float y[], size_t L);
static void update_state_covarariance_matrix_and_state_estimation_vector(float S[], float xhat[], float yhat[], float y[], float Sy[], float Pxy[], size_t L);

/*
 * Square Root Unscented Kalman Filter For State Estimation (A better version than regular UKF)
 * L = Number of states, or sensors in practice.
 * beta = used to incorporate prior knowledge of the distribution of x (for Gaussian distributions, beta = 2 is optimal)
 * alpha = determines the spread of the sigma points around xhat and alpha is usually set to 0.01 <= alpha <= 1
 * S[L * L] = State estimate error covariance
 * F(float dx[L], float x[L], float u[L]) = Transition function
 * x[L] = State vector
 * u[L] = Input signal
 * Rv[L * L] = Process noise covariance matrix
 * Rn[L * L] = Measurement noise covariance matrix
 * xhat[L] = Estimated state (our input)
 * y[L] = Measurement state (our output)
 */
void sr_ukf_state_estimation(float y[], float xhat[], float Rn[], float Rv[], float u[], void (*F)(float[], float[], float[]), float S[], float alpha, float beta, size_t L){
	/* Create the size N */
	size_t N = 2 * L + 1;

	/* Predict: Create the weights */
	float *Wc = (float*)malloc(N * sizeof(float));
	float *Wm = (float*)malloc(N * sizeof(float));
	float kappa = 0.0f; /* kappa is 0 for state estimation */
	create_weights(Wc, Wm, alpha, beta, kappa, L);

	/* Predict: Create sigma point matrix for F function  */
	float *X = (float*)malloc(L * N * sizeof(float));
	create_sigma_point_matrix(X, xhat, S, alpha, kappa, L);

	/* Predict: Compute the transition function F */
	float *Xstar = (float*)malloc(L * N * sizeof(float));
	compute_transistion_function(Xstar, X, u, F, L);

	/* Predict: Multiply sigma points to weights for xhat */
	multiply_sigma_point_matrix_to_weights(xhat, Xstar, Wm, L);

	/* Predict: Create state estimate error covariance  */
	create_state_estimation_error_covariance_matrix(S, Wc, Xstar, xhat, Rv, L);

	/* Predict: Create sigma point matrix for H function. This is the updated version of SR-UKF paper. The old SR-UKF paper don't have this */
	create_sigma_point_matrix(X, xhat, S, alpha, kappa, L);

	/* Predict: Compute the observability function H */
	float *Y = (float*)malloc(L * N * sizeof(float));
	H(Y, X, L);

	/* Predict: Multiply sigma points to weights for yhat */
	float *yhat = (float*)malloc(L * sizeof(float));
	multiply_sigma_point_matrix_to_weights(yhat, Y, Wm, L);

	/* Update: Create measurement covariance matrix */
	float *Sy = (float*)malloc(L * L * sizeof(float));
	create_state_estimation_error_covariance_matrix(Sy, Wc, Y, yhat, Rn, L);

	/* Update: Create state covariance matrix */
	float *Pxy = (float*)malloc(L * L * sizeof(float));
	create_state_cross_covariance_matrix(Pxy, Wc, X, Y, xhat, yhat, L);

	/* Update: Perform state update and covariance update */
	update_state_covarariance_matrix_and_state_estimation_vector(S, xhat, yhat, y, Sy, Pxy, L);

	/* Free */
	free(Wc);
	free(Wm);
	free(X);
	free(Xstar);
	free(Y);
	free(yhat);
	free(Sy);
	free(Pxy);
}

static void create_weights(float Wc[], float Wm[], float alpha, float beta, float kappa, size_t L){
	/* Create the size N */
	size_t N = 2 * L + 1;

	/* Compute lambda and gamma parameters */
	float lambda = alpha * alpha * (L + kappa) - L;

	/* Insert at first index */
	Wm[0] = lambda/(L + lambda);
	Wc[0] = Wm[0] + 1 - alpha * alpha + beta;

	/* The rest of the indexes are the same */
	size_t i;
	for(i = 1; i < N; i++){
		Wc[i] = 0.5f / (L + lambda);
		Wm[i] = Wc[i];
	}
}

static void create_sigma_point_matrix(float X[], float x[], float S[], float alpha, float kappa, size_t L){
	/* Decleration */
	size_t i, j;

	/* Create the size N and K */
	size_t N = 2 * L + 1;
	size_t K = L + 1;

	/* Compute lambda and gamma parameters */
	float lambda = alpha * alpha * (L + kappa) - L;
	float gamma = sqrtf(L + lambda);

	/* Insert first column in X */
	for (i = 0; i < L; i++) {
		X[i * N] = x[i];
	}

	/* Insert in to the middle of the columns - Positive */
	for (j = 1; j < K; j++) {
		for (i = 0; i < L; i++) {
			X[i * N + j] = x[i] + gamma * S[i * L + j - 1];
		}
	}

	/* Insert in the rest of the columns - Negative */
	for (j = K; j < N; j++) {
		for (i = 0; i < L; i++) {
			X[i * N + j] = x[i] - gamma * S[i * L + j - K];
		}
	}

}

static void compute_transistion_function(float Xstar[], float X[], float u[], void (*F)(float[], float[], float[]), size_t L){
	/* Decleration */
	size_t i, j;
	
	/* Create the size N */
	size_t N = 2 * L + 1;

	/* Create the derivative state and state vector */
	float *dx = (float*)malloc(L * sizeof(float));
	float *x = (float*)malloc(L * sizeof(float));

	/* Call the F transition function with X matrix */
	for(j = 0; j < N; j++){
		/* Fill the state vector x with every row from X */
		for (i = 0; i < L; i++) {
			x[i] = X[i * N + j];
		}

		/* Call the transition function */
		F(dx, x, u);

		/* Get dx into Xstar */
		for (i = 0; i < L; i++) {
			Xstar[i * N + j] = dx[i];
		}
	}

	/* Free */
	free(dx);
	free(x);
}

static void multiply_sigma_point_matrix_to_weights(float x[], float X[], float W[], size_t L){
	/* Decleration */
	size_t i, j;
	
	/* Create the size N */
	size_t N = 2 * L + 1;

	/* Clear x */
	memset(x, 0, L * sizeof(float));

	/* Multiply x = W*X */
	for (j = 0; j < N; j++) {
		for (i = 0; i < L; i++) {
			x[i] += W[j] * X[i * N + j];
		}
	}

}

static void create_state_estimation_error_covariance_matrix(float S[], float W[], float X[], float x[], float R[], size_t L){
	/* Decleration */
	size_t i, j;
	
	/* Create the size N, M and K */
	size_t N = 2 * L + 1;
	size_t M = 2 * L + L;
	size_t K = 2 * L;

	/* Square root parameter of index 1 */
	float weight1 = sqrtf(fabsf(W[1]));

	/* Create [Q, R_] = qr(A') */
	float *AT = (float*)malloc(L * M * sizeof(float));
	float *Q = (float*)malloc(M * M * sizeof(float));
	float *R_ = (float*)malloc(M * L * sizeof(float));
	for(j = 0; j < K; j++){
		for(i = 0; i < L; i++){
			AT[i*M + j] = weight1 * (X[i * N + j+1] - x[i]);
		}
	}
	for (j = K; j < M; j++) {
		for (i = 0; i < L; i++) {
			AT[i * M + j] = sqrtf(R[i * L + j - K]);
		}
	}

	/* We need to do transpose on A according to the SR-UKF paper */
	tran(AT, L, M);

	/* Solve [Q, R_] = qr(A') but we only need R_ matrix */
	qr(AT, Q, R_, M, L, true);

	/* Get the upper triangular of R_ according to the SR-UKF paper */
	memcpy(S, R_, L * L * sizeof(float));

	/* Perform cholesky update on S */
	float *b = (float*)malloc(L * sizeof(float));
	for (i = 0; i < L; i++) {
		b[i] = X[i * N] - x[i];
	}
	bool rank_one_update = W[0] < 0.0f ? false : true;
	cholupdate(S, b, L, rank_one_update);

	/* Free */
	free(AT);
	free(Q);
	free(R_);
	free(b);
}

static void H(float Y[], float X[], size_t L){
	/* Create the size N */
	size_t N = 2 * L + 1;

	/* Compute Y = I*X where I is identity matrix */
	memcpy(Y, X, L * N * sizeof(float));
}

static void create_state_cross_covariance_matrix(float P[], float W[], float X[], float Y[], float x[], float y[], size_t L){
	/* Decleration */
	size_t i, j;
	
	/* Create the size N and K */
	size_t N = 2 * L + 1;
	size_t K = 2 * L;

	/* clear P */
	memset(P, 0, K * sizeof(float));

	/* Subtract the matrices */
	for(j = 0; j < N; j++){
		for(i = 0; i < L; i++){
			X[i * N + j] -= x[i];
			Y[i * N + j] -= y[i];
		}
	}

	/* Create diagonal matrix */
	float *diagonal_W = (float*)malloc(N * N * sizeof(float));
	memset(diagonal_W, 0, N*N*sizeof(float));
	for (i = 0; i < N; i++) {
		diagonal_W[i * N + i] = W[i];
	}

	/* Do P = X*diagonal_W*Y' */
	tran(Y, L, N);
	float *diagonal_WY = (float*)malloc(N * L * sizeof(float));
	mul(diagonal_W, Y, diagonal_WY, N, N, L);
	mul(X, diagonal_WY, P, L, N, L);

	/* Free */
	free(diagonal_W);
	free(diagonal_WY);
}

static void update_state_covarariance_matrix_and_state_estimation_vector(float S[], float xhat[], float yhat[], float y[], float Sy[], float Pxy[], size_t L){
	/* Decleration */
	size_t i, j;
	
	/* Transpose of Sy */
	float *SyT = (float*)malloc(L * L * sizeof(float));
	memcpy(SyT, Sy, L * L * sizeof(float));
	tran(SyT, L, L);

	/* Multiply Sy and Sy' to Sy'Sy */
	float *SyTSy = (float*)malloc(L * L * sizeof(float));
	mul(SyT, Sy, SyTSy, L, L, L);

	/* Take inverse of Sy'Sy - Inverse is using LUP-decomposition */
	inv(SyTSy, L);

	/* Compute kalman gain K from Sy'Sy * K = Pxy => K = Pxy * inv(SyTSy) */
	float *K = (float*)malloc(L * L * sizeof(float));
	mul(Pxy, SyTSy, K, L, L, L);

	/* Compute xhat = xhat + K*(y - yhat) */
	float *yyhat = (float*)malloc(L * sizeof(float));
	float *Ky = (float*)malloc(L * sizeof(float));
	for (i = 0; i < L; i++) {
		yyhat[i] = y[i] - yhat[i];
	}
	mul(K, yyhat, Ky, L, L, 1);
	for (i = 0; i < L; i++) {
		xhat[i] = xhat[i] + Ky[i];
	}

	/* Compute U = K*Sy */
	float *U = (float*)malloc(L * L * sizeof(float));
	mul(K, Sy, U, L, L, L);

	/* Compute S = cholupdate(S, Uk, -1) because Uk is a vector and U is a matrix */
	float *Uk = (float*)malloc(L * sizeof(float));
	for(j = 0; j < L; j++){
		for (i = 0; i < L; i++) {
			Uk[i] = U[i * L + j];
		}
		cholupdate(S, Uk, L, false);
	}

	/* Free */
	free(SyT);
	free(SyTSy);
	free(K);
	free(yyhat);
	free(Ky);
	free(U);
	free(Uk);
}

/*
 * ukf.c
 *
 *  Created on: 9 juni 2021
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

static void ukf_create_weights(float *Wa, float *Wc, float a, float b, float k, uint8_t L);
static void ukf_compute_sigma_points(float *s, float *x, float *P, float a, float k, uint8_t L);
static void ukf_multiply_weights(float *x, float *xi, float *W, uint8_t L);
static void ukf_estimate_covariance(float *P, float *xi, float *x, float *W, float *O, uint8_t L);
static void ukf_estimate_cross_covariance(float* Csz, float* s, float* xhat, float* z, float* zhat, float* Wc, uint8_t L);
static void ukf_create_kalman_K(float *K, float *Shat, float *Csz, uint8_t L);
static void ukf_state_update(float *K, float *Shat, float *P, float *xhat, float *zk, float *zhat, uint8_t L);

/*
 * Unscented Kalman Filter
 * xhat[L] = Estimated state vector
 * zk[L] = Real world measurement vector
 * u[L] = Input signal
 * P[L*L] = Covariance matrix
 * Q[L*L] = Disturbance covariance matrix
 * R[L*L] = Noise covaraiance matrix
 * a = Alpha number, a small number, but not very small due to float presition
 * k = Kappa number, usually zero.
 * b = Beta number, 2 = optimal for gaussian distribution
 * L = Number of states, or sensors in practice.
 * This follows wikipedia article: https://en.wikipedia.org/wiki/Kalman_filter#Unscented_Kalman_filter
 */
void ukf(float* xhat, float* zk, float* u, float* P, float* Q, float* R, float a, float k, float b,  uint8_t L, void (*ukf_transition)(float*, float*, float*, uint8_t)) {
	// Column
	uint8_t N = 2 * L + 1;

	// Init - Create the weights
	float Wa[N];
	float Wc[N];
	ukf_create_weights(Wa, Wc, a, b, k, L);

	// PREDICT: Step 0 - Predict the state and state estimation error covariance at the next time step
	float s[L*N];
	ukf_compute_sigma_points(s, xhat, P, a, k, L);

	// PREDICT: Step 1 - Run our transition function
	float x[L*N];
	ukf_transition(x, s, u, L);

	// PREDICT: Step 2 - Combine the predicted states to obtain the predicted states
	ukf_multiply_weights(xhat, x, Wa, L);

	// PREDICT: Step 3 - Compute the covariance of the predicted state
	ukf_estimate_covariance(P, x, xhat, Wc, Q, L);

	// UPDATE: Step 1 - Use the nonlinear measurement function to compute the predicted measurements for each of the sigma points.
	float z[L*N];
	memcpy(z, s, L * N * sizeof(float)); // Here we assume that the observation function z = h(s, u) = s

	// UPDATE: Step 2 - Combine the predicted measurements to obtain the predicted measurement
	float zhat[L];
	ukf_multiply_weights(zhat, z, Wa, L);

	// UPDATE: Step 3 - Estimate the covariance of the predicted measurement
	float Shat[L*L];
	ukf_estimate_covariance(Shat, z, zhat, Wc, R, L);

	// UPDATE: Step 4 - Estimate the cross-covariance
	float Csz[L*L];
	ukf_estimate_cross_covariance(Csz, s, xhat, z, zhat, Wc, L);

	// UPDATE: Step 5 - Find kalman K matrix
	float K[L*L];
	ukf_create_kalman_K(K, Shat, Csz, L);

	// UPDATE: Step 6 - Obtain the estimated state and state estimation error covariance
	ukf_state_update(K, Shat, P, xhat, zk, zhat, L);
}

/*
 * Create weight vector
 * Wa[2*L + 1] = Weight vector for covariance sigma points
 * Wc[2*L + 1] = Weight vector for covariance matrices
 * a = A small number
 * b = Knowledge about the gaussian distirbution. b = 2 (Optimal for gaussian distribution)
 * k = Usually set to 0
 * L = how many states we have
 */
static void ukf_create_weights(float *Wa, float *Wc, float a, float b, float k, uint8_t L) {
	uint8_t N = 2 * L + 1;
	for (uint8_t i = 0; i < N; i++){
		if (i == 0){
			*Wa = (a*a*k-L)/(a*a*k);
			*Wc = *Wa + 1 - a*a + b;
		}else{
			*(Wa + i) = 1/(2*a*a*k);
			*(Wc + i) = *(Wa + i);
		}
	}
}

/*
 * Multiply and sum the weights w with vector
 * x[L] = The output vector
 * xi[L*(2*L+1)] = Vector of sigma points
 * W[2*L+1] = Weight vector
 * L = Dimension of the vectors
 */
static void ukf_multiply_weights(float *x, float *xi, float *W, uint8_t L) {
	uint8_t N = 2 * L + 1;
	memset(x, 0, L * sizeof(float));
	for (uint8_t j = 0; j < N; j++)
		for (uint8_t i = 0; i < L; i++)
			*(x + i) += *(W + j) * *(xi + i * N + j);
}

/*
 * Compute the sigma points
 * s[L*(2*L + 1)] = Sigma point matrix
 * x[L] = Estimated vector
 * P[L*L] = Covariance matrix
 * a = Alpha factor
 * k = Kappa factor
 * L = Dimension of the vectors
 */
static void ukf_compute_sigma_points(float *s, float *x, float *P, float a, float k, uint8_t L) {
	uint8_t N = 2 * L + 1;
	uint8_t compensate_column = L + 1;

	// Compute A = a*sqrt(k)*chol(P)
	float c = a*sqrtf(k);
	float A[L * L];
	chol(P, A, L);
	for(uint8_t i = 0; i < L * L; i++)
		*(A + i) = c * *(A + i); // A = c*A
	for (uint8_t j = 0; j < N; j++)
		if (j == 0)
			for (uint8_t i = 0; i < L; i++)
				*(s + i * N + j) = *(x + i); 										// First sigma vector will become as the previous estimated state
		else if (j >= 1 && j <= L)
			for (uint8_t i = 0; i < L; i++)
				*(s + i * N + j) = *(x + i) + *(A + i * L + j - 1); 				// We take the j:th column of A from 0..L-1 where j >= 1
		else
			for (uint8_t i = 0; i < L; i++)
				*(s + i * N + j) = *(x + i) - *(A + i * L + j - compensate_column); // Same here. A have not the same amount of columns as s
}

/*
 * Estimate the covariance
 * P[L*L] = Covariance matrix
 * xi[L*(2*L + 1)] = Sigma point matrix
 * x[L] = Estimated vector
 * W[2*L + 1] = Weight vector
 * O[L*L] = Tuning matrix
 * L = Dimension of the vectors
 */
static void ukf_estimate_covariance(float *P, float *xi, float *x, float *W, float *O, uint8_t L) {
	uint8_t N = 2 * L + 1;
	float diff[L];
	float diffT[L];
	float diff_diffT[L * L];
	memset(P, 0, L * L * sizeof(float));
	for (uint8_t j = 0; j < N; j++) {
		for (uint8_t i = 0; i < L; i++)
			*(diff + i) = *(xi + i * N + j) - *(x + i); // Create the difference vector xi - x
		memcpy(diffT, diff, L * sizeof(float)); // We need to transpose difference vector as well
		mul(diff, diffT, diff_diffT, L, 1, L); // Create the matrix diff_diffT
		for (uint8_t i = 0; i < L * L; i++)
			*(P + i) += *(W + j) * *(diff_diffT + i); // P = sum(W*diff*diffT)
	}
	for (uint8_t i = 0; i < L * L; i++)
		*(P + i) += *(O + i); // Add the O matrix, which can be R or Q matrix
}

/*
 * Estimate the cross covariance
 * Csz[M*M] = Cross covariance matrix
 * s[L*(2*L + 1)] = Sigma point matrix
 * xhat[L] = Estimated vector
 * z[L*(2*L + 1)] = Sigma point matrix
 * zhat[L] = Estimated vector
 * Wc = Weight vector
 * L = Dimension of the vectors
 */
static void ukf_estimate_cross_covariance(float* Csz, float* s, float* xhat, float* z, float* zhat, float* Wc, uint8_t L) {
	uint8_t N = 2 * L + 1;
	float diffx[L];
	float diffyT[L];
	float diffx_diffyT[L * L];
	memset(Csz, 0, L * L * sizeof(float));
	for (uint8_t j = 1; j < N; j++) {
		for (uint8_t i = 0; i < L; i++) {
			*(diffx + i) = *(s + i * N + j) - *(xhat + i);  // Create the difference vector s - xhat
			*(diffyT + i) = *(z + i * N + j) - *(zhat + i); // Create the difference vector z - zhat
		}
		mul(diffx, diffyT, diffx_diffyT, L, 1, L); // Create the matrix diffx_diffyT
		for (uint8_t i = 0; i < L * L; i++)
			*(Csz + i) += *(Wc + j) * *(diffx_diffyT + i); // Sum P = sum(Wc*diffx*diffyT)
	}
}

/*
 * Create kalman K matrix
 * K[L*L] = Kalman gain matrix
 * Shat[L*L] = Covariance matrix
 * Csz[L*L] = Cross Covaraince matrix
 * L = Dimension of the vectors
 */
static void ukf_create_kalman_K(float *K, float *Shat, float *Csz, uint8_t L) {
	float b[L];
	float x[L];
	for (uint8_t j = 0; j < L; j++) {
		for (uint8_t i = 0; i < L; i++)
			*(b + i) = *(Csz + i * L + j); // For every column
		linsolve_chol(Shat, x, b, L); // Cholesky decomposition because Shat is symmetric and positive definite
		for (uint8_t i = 0; i < L; i++)
			*(K + i * L + j) = *(x + i); // Add column to the kalman K matrix
	}
}

/*
 * State update
 * K[L*L] = Kalman gain matrix
 * Shat[L*L] = Covariance matrix
 * P[L*L] = Covariance matrix
 * xhat[L] = Estimated vector
 * zk[L] = Output measurement vector
 * zhat[L] = Estimated vector
 * L = Dimension of the vectors
 */
static void ukf_state_update(float *K, float *Shat, float *P, float *xhat, float *zk, float *zhat, uint8_t L) {
	// K_zdiff = K*(zk - zhat)
	float zdiff[L];
	for (uint8_t i = 0; i < L; i++)
		*(zdiff + i) = *(zk + i) - *(zhat + i);
	float K_zdiff[L];
	mul(K, zdiff, K_zdiff, L, L, 1);

	// Final state update: xhat = xhat + K_zdiff
	for (uint8_t i = 0; i < L; i++)
		*(xhat + i) = *(xhat + i) + *(K_zdiff + i);

	// Solve P = P - K*Shat*K'
	// Transpose of K -> K'
	float KT[L * L];
	memcpy(KT, K, L * L * sizeof(float));
	tran(KT, L, L);

	// Multiply Shat times KT
	float ShatKT[L * L];
	mul(Shat, KT, ShatKT, L, L, L);

	// Multiply K times PyKT, we borrow matrix KT but we call it KShatKT
	mul(K, ShatKT, KT, L, L, L);

	// Update P = P - KShatKT
	for (uint8_t i = 0; i < L * L; i++)
		*(P + i) -= *(KT + i); // KT == KShatKT
}

/*
 * 	GNU Octave code:
 *
function [xhat, P] = ukf(xhat, zk, P, Q, R, a, k, b, L)  
  % Init - Create the weights
  [Wa, Wc] = ukf_create_weights(a, b, k, L);
    
  % PREDICT: Step 0 - Predict the state and state estimation error covariance at the next time step
  s = ukf_compute_sigma_points(xhat, P, a, k, L);

  % PREDICT: Step 1 - Run our transition function
  x = ukf_transition(s, L);
 
  % PREDICT: Step 2 - Combine the predicted states to obtain the predicted states
  xhat = ukf_multiply_weights(x, Wa, L);

  % PREDICT: Step 3 - Compute the covariance of the predicted state
  P = ukf_estimate_covariance(x, xhat, Wc, Q, L);
  
  % UPDATE: Step 1 - Use the nonlinear measurement function to compute the predicted measurements for each of the sigma points.
  z = s; % Here we assume that the observation function z = h(s, u) = s
  
  % UPDATE: Step 2 - Combine the predicted measurements to obtain the predicted measurement
  zhat = ukf_multiply_weights(z, Wa, L);
  
  % UPDATE: Step 3 - Estimate the covariance of the predicted measurement
  Shat = ukf_estimate_covariance(z, zhat, Wc, R, L);
  
  % UPDATE: Step 4 - Estimate the cross-covariance between xhat and zhat. 
  Csz = ukf_estimate_cross_covariance(s, xhat, z, zhat, Wc, L);
  
  % UPDATE: Step 5 - Find kalman K matrix
  K = ukf_create_kalman_K(Shat, Csz, L);
  
  % UPDATE: Step 6 - Obtain the estimated state and state estimation error covariance
  [xhat, P] = ukf_state_update(K, Shat, P, xhat, zk, zhat, L);
  
end

function [Wa, Wc] = ukf_create_weights(a, b, k, L)
  N = 2 * L + 1;
  Wa = zeros(1, N);
  Wc = zeros(1, N);
  for i = 1:N
    if(i == 1)
      Wa(i) = (a*a*k-L)/(a*a*k);
      Wc(i) = Wa(i) + 1 - a*a + b;
    else
      Wa(i) = 1/(2*a*a*k);
      Wc(i) = Wa(i);
    end
  end
end

function [s] = ukf_compute_sigma_points(x, P, a, k, L)
  N = 2 * L + 1;
	compensate = L + 1;
  s = zeros(L, N);
  A = a*sqrt(k)*chol(P);
  for j = 1:N
    if(j == 1)
      s(:, j) = x;
    elseif(and(j >= 2, j <= L + 1))
      s(:, j) = x + A(:, j - 1);
    else 
      s(:, j) = x - A(:, j - compensate);
    end
  end
end

function x = ukf_multiply_weights(xi, W, L)
  N = 2 * L + 1;
  x = zeros(L, 1);
  for i = 1:N
    x = x + W(i)*xi(:, i);
  end
end

function P = ukf_estimate_covariance(xi, x, W, O, L)
  N = 2 * L + 1;
  P = zeros(L, L);
  for i = 1:N
    P = P + W(i)*(xi(:, i) - x)*(xi(:, i) - x)';
  end
  P = P + O;
end

function Csz = ukf_estimate_cross_covariance(s, xhat, z, zhat, Wc, L)
	N = 2 * L + 1;
  Csz = zeros(L, L);
  for i = 1:N
    Csz = Csz + Wc(i)*(s(:, i) - xhat)*(z(:, i) - zhat)';
  end
end

function K = ukf_create_kalman_K(Shat, Csz, L)
  K = zeros(L, L);
  for i = 1:L
    % Solve Ax = b with Cholesky
    A = chol(Shat, 'lower');
    y = linsolve(A, Csz(:, i));
    K(:, i) = linsolve(A', y);
  end
end

function [xhat, P] = ukf_state_update(K, Shat, P, xhat, zk, zhat, L)
  xhat = xhat + K*(zk - zhat);
  P = P - K*Shat*K';
end

function x = ukf_transition(s, L)
  N = 2 * L + 1;
  x = zeros(L, 1);
  for i = 1:N
    x(i) = std(s(:, i))*randn + mean(s(:, i)); % std*random_variable + average = Gaussian distribution
  end
end
 */

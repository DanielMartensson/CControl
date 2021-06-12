/*
 * ukf.c
 *
 *  Created on: 9 juni 2021
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

static void ukf_create_weights(float *Wc, float *WM, float a, float b, float k, uint8_t M);
static void ukf_multiply_weights(float *x, float *xi, float *W, uint8_t M);
static void ukf_compute_sigma_points(float *xi, float *x, float *P, float a, float k, uint8_t M);
static void ukf_estimate_covariance(float *P, float *xi, float *x, float *W, float *O, uint8_t M);
static void ukf_estimate_cross_covariance(float *P, float *xi, float *x, float *yi, float *y, float a, float k, uint8_t M);
static void ukf_create_kalman_K(float *K, float *Py, float *Pxy, uint8_t M);
static void ukf_state_update(float *K, float *Py, float *P, float *xhat, float *y, float *yhat, uint8_t M);
static void ukf_transition(void (*transistion_function)(float*, float*, float*), float *xhati, float *u, uint8_t M);

/*
 * Do Unscented Kalman Filter
 * xhat[M] = Previous estimated state, which will result as the current estimated state
 * y[M] = Measured output
 * u[M] = Input signal
 * P[M*M] = Covariance matrix x
 * Q[M*M] = Process noise matrix
 * R[M*M] = Measurement noise matrix
 * transistion_function = first order ordinary differential equation system with three array arguments
 * a = Alpha scalar. Positive but small. This controls the sigma points.
 * k = Kappa scalar. Usually set to 0
 * b = Beta scalar. Incorporates prior knowledge of the distribution of the state. 2 = Gaussian
 * M = Number of states e.g the dimension of the state vector and size of the matrix
 * This ukf.c file follows https://se.mathworks.com/help/control/ug/extended-and-unscented-kalman-filter-algorithms-for-online-state-estimation.html
 */
void ukf(float *xhat, float *y, float *u, float *P, float *Q, float *R, void (*transistion_function)(float*, float*, float*), float a, float k, float b, uint8_t M) {
	// Initial parameters
	uint8_t column = 2 * M + 1; // Only for sigma points and weights
	uint8_t row = M;

	// Step 0 - Create the weights
	float Wc[column];
	float WM[column];
	ukf_create_weights(Wc, WM, a, b, k, row);

	// UPDATE: Step 1 - Compute sigma points
	float xhati[row * column];
	ukf_compute_sigma_points(xhati, xhat, P, a, k, row);

	// UPDATE: Step 2 - Use the nonlinear measurement function to compute the predicted measurements for each of the sigma points.
	float yhati[row * column];
	memcpy(yhati, xhati, row * column * sizeof(float)); // Here we assume that the observation function y = h(x, u) = x

	// UPDATE: Step 3 - Combine the predicted measurements to obtain the predicted measurement
	float yhat[row];
	ukf_multiply_weights(yhat, yhati, WM, row);

	// UPDATE: Step 4 - Estimate the covariance of the predicted measurement
	float Py[row * row];
	ukf_estimate_covariance(Py, yhati, yhat, Wc, R, row);

	// UPDATE: Step 5 - Estimate the cross-covariance between xhat and yhat. Here i begins at 1 because xhati(0) - xhat(0) = 0
	float Pxy[row * row];
	ukf_estimate_cross_covariance(Pxy, xhati, xhat, yhati, yhat, a, k, M);

	// UPDATE: Step 6 - Find kalman K matrix
	float K[row * row];
	ukf_create_kalman_K(K, Py, Pxy, M);

	// UPDATE: Step 7 - Obtain the estimated state and state estimation error covariance at time step
	ukf_state_update(K, Py, P, xhat, y, yhat, M);

	// PREDICT: Step 0 - Predict the state and state estimation error covariance at the next time step
	ukf_compute_sigma_points(xhati, xhat, P, a, k, M);

	// PREDICT: Step 1 - Use the nonlinear state transition function to compute the predicted states for each of the sigma points.
	ukf_transition(transistion_function, xhati, u, M);

	// PREDICT: Step 2 - Combine the predicted states to obtain the predicted states at time
	ukf_multiply_weights(xhat, xhati, WM, M);

	// PREDICT: Step 3 - Compute the covariance of the predicted state
	ukf_estimate_covariance(P, xhati, xhat, Wc, Q, M);

}

/*
 * Create weight vector
 * WM[2*M + 1] = Weight vector for covariance sigma points
 * Wc[2*M + 1] = Weight vector for covariance matrices
 */
static void ukf_create_weights(float *Wc, float *WM, float a, float b, float k, uint8_t M) {
	uint8_t column = 2 * M + 1;
	for (uint8_t i = 0; i < column; i++){
		if (i == 0){
			*Wc = (2 - a * a + b) - M / (a * a * (M + k));
			*WM = 1 - M / (a * a * (M + k));
		}else{
			*(Wc + i) = 1 / (2 * a * a * (M + k));
			*(WM + i) = 1 / (2 * a * a * (M + k));
		}
	}
}

/*
 * Multiply and sum the weights w with vector
 * x[M] = The output vector
 * xi[M*(2*M+1)] = Vector of sigma points
 * W[2*M+1] = Weight vector
 * M = Dimension of the vectors
 */
static void ukf_multiply_weights(float *x, float *xi, float *W, uint8_t M) {
	uint8_t column = 2 * M + 1;
	uint8_t row = M;
	memset(x, 0, row * sizeof(float));
	for (uint8_t j = 0; j < column; j++)
		for (uint8_t i = 0; i < row; i++)
			*(x + i) += *(W + j) * *(xi + i * column + j);
}

/*
 * Compute the sigma points
 * xi[M*(2*M + 1)] = Sigma point matrix
 * x[M] = Estimated vector
 * P[M*M] = Covariance matrix
 * a = Alpha factor
 * k = Kappa factor
 * M = Dimension of the vectors
 */
static void ukf_compute_sigma_points(float *xi, float *x, float *P, float a, float k, uint8_t M) {
	uint8_t column = 2 * M + 1;
	uint8_t compensate_column = 2 * M - 1;
	uint8_t row = M;
	float c = a * a * (M + k);

	/*
	 * According to the paper "A New Extension of the Kalman Filter to Nonlinear Systems"
	 * by Simon J. Julier and Jeffrey K. Uhlmann, they used L = chol(c*P) as "square root",
	 * instead of computing the square root of c*P. Accuring to them, cholesky decomposition
	 * was a numerically efficient and a stable method.
	 */
	float A[M * M];
	float L[M * M];
	memcpy(A, P, row * row * sizeof(float));
	for(uint8_t i = 0; i < row * row; i++)
		*(A + i) = c * *(A + i); // A = c*A
	chol(A, L, row);

	for (uint8_t j = 0; j < column; j++)
		if (j == 0)
			for (uint8_t i = 0; i < row; i++)
				*(xi + i * column + j) = *(x + i); // First sigma vector will become as the previous estimated state
		else if (j >= 1 && j <= row)
			for (uint8_t i = 0; i < row; i++)
				*(xi + i * column + j) = *(x + i) + *(L + i * row + j - 1); // We take the j:th column of P from 0..M-1 where j >= 1
		else
			for (uint8_t i = 0; i < row; i++)
				*(xi + i * column + j) = *(x + i) - *(L + i * row + j - compensate_column); // Same here. P have not the same amount of columns as xi
}

/*
 * Estimate the covariance
 * P[M*M] = Covariance matrix
 * xi[M*(2*M + 1)] = Sigma point matrix
 * x[M] = Estimated vector
 * W[2*M + 1] = Weight vector
 * O[M*M] = Tuning matrix
 * M = Dimension of the vectors
 */
static void ukf_estimate_covariance(float *P, float *xi, float *x, float *W, float *O, uint8_t M) {
	uint8_t column = 2 * M + 1;
	uint8_t row = M;
	float diff[row];
	float diffT[row];
	float diff_diffT[row * row];
	memset(P, 0, row * row * sizeof(float));
	for (uint8_t j = 0; j < column; j++) {
		for (uint8_t i = 0; i < row; i++)
			*(diff + i) = *(xi + i * column + j) - *(x + i); // Create the differense vector
		memcpy(diffT, diff, row * sizeof(float)); // We need to transpose difference vector as well
		mul(diff, diffT, diff_diffT, row, 1, row); // Create the matrix diff_diffT
		for (uint8_t i = 0; i < row * row; i++)
			*(P + i) += *(W + j) * *(diff_diffT + i); // Sum P = sum(W*diff*diffT)
	}
	for (uint8_t i = 0; i < row * row; i++)
		*(P + i) += *(O + i);
}

/*
 * Estimate the cross covariance
 * P[M*M] = Covariance matrix
 * xi[M*(2*M + 1)] = Sigma point matrix
 * x[M] = Estimated vector
 * yi[M*(2*M + 1)] = Sigma point matrix
 * y[M] = Estimated vector
 * a = Alpha
 * k = kappa
 * M = Dimension of the vectors
 */
static void ukf_estimate_cross_covariance(float *P, float *xi, float *x, float *yi, float *y, float a, float k, uint8_t M) {
	uint8_t column = 2 * M + 1;
	uint8_t row = M;
	float c = 1 / (2 * a * a * (M + k));
	float diffx[row];
	float diffyT[row];
	float diffx_diffyT[row * row];
	memset(P, 0, row * row * sizeof(float));
	for (uint8_t j = 1; j < column; j++) {
		for (uint8_t i = 0; i < row; i++) {
			*(diffx + i) = *(xi + i * column + j) - *(x + i); // Create the difference vector
			*(diffyT + i) = *(yi + i * column + j) - *(y + i);
		}
		mul(diffx, diffyT, diffx_diffyT, row, 1, row); // Create the matrix diffx_diffyT
		for (uint8_t i = 0; i < row * row; i++)
			*(P + i) += *(diffx_diffyT + i); // Sum P = sum(diffx*diffyT)
	}
	for (uint8_t i = 0; i < row * row; i++)
		*(P + i) *= c; // Multiply with the weight
}

/*
 * Create kalman K matrix
 * K[M*M] = Kalman gain matrix
 * Py[M*M] = Covariance matrix
 * Pxy[M*M] = Cross Covaraince matrix
 * M = Dimension of the vectors
 */
static void ukf_create_kalman_K(float *K, float *Py, float *Pxy, uint8_t M) {
	uint8_t column = M;
	uint8_t row = M;
	float b[row];
	float x[row];
	for (uint8_t j = 0; j < column; j++) {
		for (uint8_t i = 0; i < row; i++)
			*(b + i) = *(Pxy + i * row + j); // For every column
		linsolve_chol(Py, x, b, row); // Cholesky decomposition because Py is symmetric and positive definite
		for (uint8_t i = 0; i < row; i++)
			*(K + i * row + j) = *(x + i); // Add column to the kalman K matrix
	}
}

/*
 * State update
 * K[M*M] = Kalman gain matrix
 * Py[M*M] = Covariance matrix
 * P[M*M] = Covariance matrix
 * xhat[M] = Estimated vector
 * y[M] = Output measurement vector
 * yhat[M] = Estimated vector
 * M = Dimension of the vectors
 */
static void ukf_state_update(float *K, float *Py, float *P, float *xhat, float *y, float *yhat, uint8_t M) {
	uint8_t row = M;

	// K_ydiff = K*(y - yhat)
	float ydiff[row];
	for (uint8_t i = 0; i < row; i++)
		*(ydiff + i) = *(y + i) - *(yhat + i);
	float K_ydiff[row];
	mul(K, ydiff, K_ydiff, row, row, 1);

	// Final state update: xhat = xhat + K_ydiff
	for (uint8_t i = 0; i < row; i++)
		*(xhat + i) = *(xhat + i) + *(K_ydiff + i);

	// Solve P = P - K*Py*K'
	// Transpose of K -> K'
	float KT[row * row];
	memcpy(KT, K, row * row * sizeof(float));
	tran(KT, row, row);

	// Multiply Py times KT
	float PyKT[row * row];
	mul(Py, KT, PyKT, row, row, row);

	// Multiply K times PyKT, we borrow matrix KT but we call it KPyKT
	mul(K, PyKT, KT, row, row, row);

	// Update P = P - KPyKT
	for (uint8_t i = 0; i < row * row; i++)
		*(P + i) -= *(KT + i); // KT == KPyKT
}

/*
 * Apply with transition function dx = f(x, u)
 * transistion_function = Nonlinear system of equations
 * xhati[M*(2*M + 1)] = Sigma point matrix
 * u[M] = Input vector
 * M = Dimension of vectors
 */
static void ukf_transition(void (*transistion_function)(float*, float*, float*), float *xhati, float *u, uint8_t M) {
	uint8_t column = 2 * M + 1;
	uint8_t row = M;
	float dx[row];
	float x[row];
	for (uint8_t j = 0; j < column; j++) {
		for (uint8_t i = 0; i < row; i++)
			*(x + i) = *(xhati + i * column + j); // Extract one column from sigma point xhati to x vector
		transistion_function(dx, x, u); // Simulate and get dx
		for (uint8_t i = 0; i < row; i++)
			*(xhati + i * column + j) = *(dx + i); // Replace the same column with new values of dx
	}
}

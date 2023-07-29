/*
 * kernel.c
 *
 *  Created on: 27 Juli. 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

static float linear_kernel(float x[], float y[], size_t N) {
	return dot(x, y, N);
}

static float rbf_kernel(float x[], float y[], size_t N, float gamma) {
	float* diff = (float*)malloc(N * sizeof(float));
	size_t i;
	for (i = 0; i < N; i++) {
		diff[i] = x[i] - y[i];
	}
	float a = norm(diff, 1, N, NORM_METHOD_L2);
	free(diff);
	return expf(-gamma * a * a);
}

static float polynomial_kernel(float x[], float y[], size_t N, float degree) {
	return powf(dot(x, y, N) + 1.0f, degree);
}

static float sigmoid_kernel(float x[], float y[], size_t N, float alpha, float beta) {
	return tanhf(alpha * dot(x, y, N) + beta);
}

static float gaussian_kernel(float x[], float y[], size_t N, float sigma) {
	float* diff = (float*)malloc(N * sizeof(float));
	size_t i;
	for (i = 0; i < N; i++) {
		diff[i] = x[i] - y[i];
	}
	float a = norm(diff, 1, N, NORM_METHOD_L2);
	free(diff);
	return expf(-a * a / (2 * sigma * sigma));
}

static float exponential_kernel(float x[], float y[], size_t N, float sigma) {
	float* diff = (float*)malloc(N * sizeof(float));
	size_t i;
	for (i = 0; i < N; i++) {
		diff[i] = x[i] - y[i];
	}
	float a = norm(diff, 1, N, NORM_METHOD_L2);
	free(diff);
	return expf(-a / (2 * sigma * sigma));
}

/*
 * This should be used with e.g before PCA or LDA for better classification/separation of classes-
 * This algorithm applies a kernel onto matrix K, e.g giving K another higher dimension
 * X[m*n]
 * K[m*m]
 * KERNEL_METHOD_LINEAR -> kernel_parameters can be NULL
 * KERNEL_METHOD_RBF -> kernel_parameters[gamma]
 * KERNEL_METHOD_POLY -> kernel_parameters[degree]
 * KERNEL_METHOD_SIGMOID -> kernel_parameters[alpha, beta]
 * KERNEL_METHOD_GAUSSIAN -> kernel_parameters[sigma]
 * KERNEL_METHOD_EXPONENTIAL -> kernel_parameters[sigma]
 */
void kernel(float X[], float K[], size_t row, size_t column, float kernel_parameters[], KERNEL_METHOD kernel_method) {
	size_t i, j;
	switch (kernel_method) {
	case KERNEL_METHOD_LINEAR:
		for (i = 0; i < row; i++) {
			for (j = 0; j < row; j++) {
				K[j] = linear_kernel(&X[i * column], &X[j * column], column);
			}
			K += row;
		}
		break;
	case KERNEL_METHOD_RBF:
		for (i = 0; i < row; i++) {
			for (j = 0; j < row; j++) {
				K[j] = rbf_kernel(&X[i * column], &X[j * column], column, kernel_parameters[0]);
			}
			K += row;
		}
		break;
	case KERNEL_METHOD_POLY:
		for (i = 0; i < row; i++) {
			for (j = 0; j < row; j++) {
				K[j] = polynomial_kernel(&X[i * column], &X[j * column], column, kernel_parameters[0]);
			}
			K += row;
		}
		break;
	case KERNEL_METHOD_SIGMOID:
		for (i = 0; i < row; i++) {
			for (j = 0; j < row; j++) {
				K[j] = sigmoid_kernel(&X[i * column], &X[j * column], column, kernel_parameters[0], kernel_parameters[1]); 
			}
			K += row;
		}
		break;
	case KERNEL_METHOD_GAUSSIAN:
		for (i = 0; i < row; i++) {
			for (j = 0; j < row; j++) {
				K[j] = gaussian_kernel(&X[i * column], &X[j * column], column, kernel_parameters[0]);
			}
			K += row;
		}
		break;
	case KERNEL_METHOD_EXPONENTIAL:
		for (i = 0; i < row; i++) {
			for (j = 0; j < row; j++) {
				K[j] = exponential_kernel(&X[i * column], &X[j * column], column, kernel_parameters[0]);
			}
			K += row;
		}
		break;
	}
}

/* GNU Octave code:
	  % Create kernel
	  function kernel_test
	  X = [1.0, 2.0, 3.0,
		   4.0, 5.0, 6.0,
		   7.0, 8.0, 9.0,
		  10.0, 11.0, 12.0];
	  K = create_kernel(X, 'linear', [0])
	  K = create_kernel(X, 'rbf', [0.1])
	  K = create_kernel(X, 'polynomial', [1])
	  K = create_kernel(X, 'sigmoid', [0.5 1.5])
	  K = create_kernel(X, 'gaussian', [343.23])
	  K = create_kernel(X, 'exponential', [10.23])

	end

	function K = create_kernel(X, kernel_type, kernel_parameters)
	  % Get the rows of X
	  m = size(X, 1);

	  % Create empty kernel
	  K = zeros(m, m);

	  % Select kernel type
	  switch kernel_type
		case 'gaussian'
		sigma = kernel_parameters(1);
		for i = 1:m
			for j = 1:m
				K(i, j) = exp(-norm(X(i, :) - X(j, :))^2 / (2 * sigma^2));
			end
		end

		case 'exponential'
		sigma = kernel_parameters(1);
		for i = 1:m
			for j = 1:m
				 K(i, j) = exp(-norm(X(i, :) - X(j, :)) / (2 * sigma^2));
			end
		end

		case 'polynomial'
		degree = kernel_parameters(1);
		for i = 1:m
			for j = 1:m
				K(i, j) = (dot(X(i, :), X(j, :)) + 1)^degree;
			end
		end

		case 'linear'
		K = X * X';

		case 'sigmoid'
		alpha = kernel_parameters(1);
		beta = kernel_parameters(2);
		K = tanh(alpha * X' * X + beta);

		case 'rbf'
		gamma = kernel_parameters(1);
		for i = 1:m
			for j = 1:m
				K(i, j) = exp(-gamma * norm(X(i, :) - X(j, :))^2);
			end
		end

		otherwise
		  error('Kernel type does not exist!');
		end
	end
*/
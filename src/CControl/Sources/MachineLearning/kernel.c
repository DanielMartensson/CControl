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
	float diff, sum_sq_diff = 0.0f;
	size_t i;
	for (i = 0; i < N; i++) {
		diff = x[i] - y[i];
		sum_sq_diff += diff * diff;
	}
	return exp(-gamma * sum_sq_diff);
}

static float polynomial_kernel(float x[], float y[], size_t N, int degree, float constant) {
	return pow(dot(x, y, N) + constant, degree);
}

static float sigmoid_kernel(float x[], float y[], size_t N, float a, float c) {
	return tanhf(a * dot(x, y, N) + c);
}

/*
 * This should be used with e.g before PCA or LDA for better classification/separation of classes-
 * This algorithm applies a kernel onto matrix K, e.g giving K another higher dimension
 * A[m*n]
 * K[m*m]
 * If KERNEL_METHOD_LINEAR is used, then parameters[] can be NULL
 * If KERNEL_METHOD_RBF is used, then parameters[0] = gamma
 * If KERNEL_METHOD_POLY is used, then parameters[0] = degree (integer), parameters[1] = constant
 * If KERNEL_METHOD_SIGMOID is used, then parameters[0] = a, parameters[1] = c
 */
void kernel(float A[], float K[], size_t row, size_t column, float parameters[], KERNEL_METHOD kernel_method) {
	size_t i, j;
	switch (kernel_method) {
	case KERNEL_METHOD_LINEAR:
		for (i = 0; i < row; i++) {
			for (j = 0; j < row; j++) {
				K[j] = linear_kernel(&A[i * column], &A[j * column], column);
			}
			K += row;
		}
		break;
	case KERNEL_METHOD_RBF:
		for (i = 0; i < row; i++) {
			for (j = 0; j < row; j++) {
				K[j] = rbf_kernel(&A[i * column], &A[j * column], column, parameters[0]); /* parameters[0] = gamma */
			}
			K += row;
		}
		break;
	case KERNEL_METHOD_POLY:
		for (i = 0; i < row; i++) {
			for (j = 0; j < row; j++) {
				K[j] = polynomial_kernel(&A[i * column], &A[j * column], column, (int)parameters[0], parameters[1]); /* parameters[0] = degree, parameters[1] = constant */
			}
			K += row;
		}
		break;
	case KERNEL_METHOD_SIGMOID:
		for (i = 0; i < row; i++) {
			for (j = 0; j < row; j++) {
				K[j] = sigmoid_kernel(&A[i * column], &A[j * column], column, parameters[0], parameters[1]); /* parameters[0] = a, parameters[1] = c */
			}
			K += row;
		}
		break;
	}
}
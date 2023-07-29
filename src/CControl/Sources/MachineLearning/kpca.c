/*
 * kpca.c
 *
 *  Created on: 28 Juli 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

 /*
  * Kernel Principal Component Analysis
  * X[m*n]
  * W[m*c]
  * P[c*m]
  * K[m*m]
  * mu[m]
  */
void kpca(float X[], float W[], float P[], float K[], float mu[], size_t c, size_t row, size_t column, float kernel_parameters[], KERNEL_METHOD kernel_method) {
	/* Create kernel matrix K */
	kernel(X, K, row, column, kernel_parameters, kernel_method);

	/* Do PCA on K instead of X */
	pca(K, W, P, mu, c, row, row);
}
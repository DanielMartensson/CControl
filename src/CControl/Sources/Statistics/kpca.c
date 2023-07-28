/*
 * kpca.c
 *
 *  Created on: 28 Juli 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

void kpca(float X[], float W[], float P[], size_t components, size_t row, size_t column, float parameters[], KERNEL_METHOD kernel_method) {
	/* Create kernel matrix K */
	float* K = (float*)malloc(row * row * sizeof(float));
	kernel(X, K, row, column, parameters, kernel_method);

	/* Do PCA on K instead of X */
	pca(K, W, P, components, row, row);


}
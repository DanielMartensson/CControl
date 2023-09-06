/*
 * sobel.c
 *
 *  Created on: 6 september 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/*
 * Compute sobel filter
 * X[m*n] - Image
 * G[m*n] - Gradients
 * O[m*n] - Orientations
 */
void sobel(float X[], float G[], float O[], size_t row, size_t column) {
	/* Create kernel matrix */
	const kernel_x[9] = { -1.0f, 0.0f, 1.0f,
						  -2.0f, 0.0f, 2.0f,
						  -1.0f, 0.0f, 1.0f };
	const kernel_y[9] = { -1.0f, -2.0f, -1.0f,
						   0.0f,  0.0f,  0.0f,
		                   1.0f,  2.0f,  1.0f };

	/* Do conv2 */
	const size_t total_size = row * column;
	float* Gx = (float*)malloc(total_size * sizeof(float));
	float* Gy = (float*)malloc(total_size * sizeof(float));
	conv2(X, Gx, row, column, kernel_x, 3);
	conv2(X, Gy, row, column, kernel_y, 3);

	/* 
	 * Do L2-norm G = sqrt(Gx^2 + Gy^2) 
	 * Do O = atan2(Gy, Gx) 
	 */
	size_t i;
	for (i = 0; i < total_size; i++) {
		G[i] = sqrtf(Gx[i] * Gx[i] + Gy[i] * Gy[i]);
		O[i] = atan2f(Gy[i], Gx[i]);
	}

	/* Free */
	free(Gx);
	free(Gy);
}
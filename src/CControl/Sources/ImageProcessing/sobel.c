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
 * G[m*n] - Gradients with L2-norm
 * O[m*n] - Orientations in radians
 */
void sobel(float X[], float G[], float O[], size_t row, size_t column, bool only_compute_G) {
	/* Create kernel matrix */
	const float kernel_x[9] = { -1.0f, 0.0f, 1.0f,
								-2.0f, 0.0f, 2.0f,
								-1.0f, 0.0f, 1.0f };
	const float kernel_y[9] = { -1.0f, -2.0f, -1.0f,
								 0.0f,  0.0f,  0.0f,
								 1.0f,  2.0f,  1.0f };

	/* Do conv2 */
	const size_t total_size = row * column;
	float* Gx = (float*)malloc(total_size * sizeof(float));
	float* Gy = (float*)malloc(total_size * sizeof(float));
	conv2fft(X, Gx, row, column, kernel_x, 3);
	conv2fft(X, Gy, row, column, kernel_y, 3);

	/* 
	 * Find gradients G = sqrt(Gx^2 + Gy^2) 
	 * Find orientations O = atan2(Gy, Gx) 
	 */
	size_t i;
	if (only_compute_G) {
		for (i = 0; i < total_size; i++) {
			G[i] = sqrtf(Gx[i] * Gx[i] + Gy[i] * Gy[i]);
		}
	}
	else {
		for (i = 0; i < total_size; i++) {
			G[i] = sqrtf(Gx[i] * Gx[i] + Gy[i] * Gy[i]);
			O[i] = atan2f(Gy[i], Gx[i]);
		}
	}

	/* Free */
	free(Gx);
	free(Gy);
}
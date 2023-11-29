/*
 * harris.c
 *
 *  Created on: 28 November 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/* 
 * Harris corner detection
 * X[m*n] - Image
 * H[m*n] - Harris score, highest peaks describe the coordinates of a corner
 */
void harris(const float X[], float H[], const float sigma, const uint8_t edge, const size_t row, const size_t column){
	/* Sobel */
	const size_t row_column = row * column;
	float* Ix = (float*)malloc(row_column * sizeof(float));
	float* Iy = (float*)malloc(row_column * sizeof(float));
	float* Ixy = (float*)malloc(row_column * sizeof(float));
	sobel(X, Ix, Iy, row, column, SOBEL_METHOD_GRADIENT_X_Y);

	/* Get gaussian matrix kernel */
	size_t K_size;
	float* K = fspecial(row, column, sigma, &K_size, FSPECIAL_TYPE_GAUSSIAN);

	/* Raise by two */
	size_t i;
	for (i = 0; i < row_column; i++) {
		Ixy[i] = Ix[i] * Iy[i];
		Ix[i] = Ix[i] * Ix[i];
		Iy[i] = Iy[i] * Iy[i];
	}

	/* Do Gaussian blurring */
	float* Ix2 = (float*)malloc(row_column * sizeof(float));
	float* Iy2 = (float*)malloc(row_column * sizeof(float));
	float* Ixy2 = (float*)malloc(row_column * sizeof(float));
	conv2(Ix, K, Ix2, row, column, K_size, K_size, CONV2_SHAPE_SAME);
	conv2(Iy, K, Iy2, row, column, K_size, K_size, CONV2_SHAPE_SAME);
	conv2(Ixy, K, Ixy2, row, column, K_size, K_size, CONV2_SHAPE_SAME);

	/* Free */
	free(Ix);
	free(Iy);
	free(Ixy);
	free(K);

	/* Find Harris score */
	const float k = 0.04f;
	for (i = 0; i < row_column; i++) {
		H[i] = Ix2[i] * Iy2[i] - Ixy2[i] * Ixy2[i] - k * (Ix2[i] + Iy2[i]) * (Ix2[i] + Iy2[i]);
	}

	/* Normalize */
	size_t index;
	float value = fabsf(amax(H, &index, row_column));
	for (i = 0; i < row_column; i++) {
		H[i] = 10000.0f / value * H[i];
	}

	/* Free */
	free(Ix2);
	free(Iy2);
	free(Ixy2);

	/* Set edges to zero */
	size_t j;
	const size_t row_minus_edge = row - edge;
	const size_t column_minus_edge = column - edge;
	for (i = 0; i < row; i++) {
		for (j = 0; j < column; j++) {
			if (i < edge || i >= row_minus_edge || j < edge || j >= column_minus_edge) {
				H[i * column + j] = 0.0f;
			}
		}
	}
}
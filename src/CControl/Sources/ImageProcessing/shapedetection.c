/*
 * shapedetection.c
 *
 *  Created on: 6 Januari 2024
 *      Author: Daniel Mårtensson
 */

#include "imageprocessing.h"

/*
 * Find specific landmarks from an image 
 * X[m*n]
 */
FAST_XY* shapedetection(const uint8_t X[], int* N, const uint8_t fast_threshold, const uint8_t sobel_threshold, const FAST_METHOD fast_method, const size_t row, const size_t column) {
	/* Turn uint8_t X to float X */
	float* X_float = uint2float(X, row, column);

	/* Do sobel */
	float* G = (float*)malloc(row * column * sizeof(float));
	sobel(X_float, G, NULL, row, column, SOBEL_METHOD_GRADIENT);

	/* Free */
	free(X_float);

	/* Create G -> G_uint8 */
	uint8_t* G_uint8 = float2uint(G, row, column);

	/* Free */
	free(G);

	/* Threshold */
	size_t i;
	const size_t row_column = row * column;
	for (i = 0; i < row_column; i++) {
		if (G_uint8[i] < sobel_threshold) {
			G_uint8[i] = 0;
		}
	}

	/* Do feature detection */
	FAST_XY* xy = featuredetection(G_uint8, N, fast_threshold, fast_method, row, column);

	/* Free */
	free(G_uint8);

	/* Return */
	return xy;
}
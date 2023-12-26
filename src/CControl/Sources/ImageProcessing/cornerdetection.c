/*
 * cornerdetection.c
 *
 *  Created on: 8 December 2023
 *      Author: Daniel M�rtensson
 */

#include "../../Headers/functions.h"

/*
 * Combine Harris corner detection with FAST feature detection
 * X[m*n]
 * N is the desired corners
 */
FAST_XY* cornerdetection(const uint8_t X[], const float sigma, int* N, const uint8_t fast_threshold, const FAST_METHOD fast_method, const size_t row, const size_t column) {
	/* Do FAST feature detection */
	int num_features;
	FAST_XY* xy = featuredetection(X, &num_features, fast_threshold, fast_method, row, column);

	/* uint8_t X to float */
	size_t i;
	const size_t row_column = row * column;
	float* X_float = (float*)malloc(row_column * sizeof(float));
	for (i = 0; i < row_column; i++) {
		X_float[i] = (float)X[i];
	}

	/* Do Harris corner detection */
	float* H = (float*)malloc(row_column * sizeof(float));
	harris(X_float, H, sigma, 0, row, column);

	/* Collect corners and its coordinates */
	float* corners = (float*)malloc(num_features * sizeof(float));
	int* x = (int*)malloc(num_features * sizeof(int));
	int* y = (int*)malloc(num_features * sizeof(int));
	for (i = 0; i < num_features; i++) {
		corners[i] = H[xy[i].y * column + xy[i].x];
		x[i] = xy[i].x;
		y[i] = xy[i].y;
	}

	/* Free */
	free(xy);
	free(X_float);
	free(H);
	
	/* Sort */
	size_t* index = (size_t*)malloc(num_features * sizeof(size_t));
	sort(corners, index, 1, num_features, SORT_MODE_COLUMN_DIRECTION_DESCEND);

	/* Select the N corners */
	if (num_features < *N) {
		*N = num_features;
	}
	xy = (FAST_XY*)malloc(*N * sizeof(FAST_XY));
	for (i = 0; i < *N; i++) {
		xy[i].x = x[index[i]];
		xy[i].y = y[index[i]];
	}

	/* Free */
	free(x);
	free(y);
	free(corners);
	free(index);

	/* Return corners */
	return xy;
}

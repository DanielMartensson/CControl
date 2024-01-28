/*
 * cornerdetection.c
 *
 *  Created on: 8 December 2023
 *      Author: Daniel Mårtensson
 */

#include "computervision.h"

/*
 * Combine Harris corner detection with FAST feature detection
 * X[m*n]
 * N is the desired corners
 */
COORDINATE_XY* cornerdetection(const uint8_t X[], const float sigma, int* N, const uint8_t fast_threshold, const FAST_METHOD fast_method, const size_t row, const size_t column) {
	/* Do FAST feature detection */
	int num_features;
	COORDINATE_XY* xy = featuredetection(X, &num_features, fast_threshold, fast_method, row, column);

	/* uint8_t X to float */
	float* X_float = uint2float(X, row, column);

	/* Do Harris corner detection */
	float* H = (float*)malloc(row * column * sizeof(float));
	harris(X_float, H, sigma, 0, row, column);

	/* Free */
	free(X_float);

	/* Collect corners and its coordinates */
	float* corners = (float*)malloc(num_features * sizeof(float));
	int* x = (int*)malloc(num_features * sizeof(int));
	int* y = (int*)malloc(num_features * sizeof(int));
	size_t i;
	for (i = 0; i < num_features; i++) {
		corners[i] = H[xy[i].y * column + xy[i].x];
		x[i] = xy[i].x;
		y[i] = xy[i].y;
	}

	/* Free */
	free(xy);
	free(H);
	
	/* Sort */
	size_t* index = (size_t*)malloc(num_features * sizeof(size_t));
	sort(corners, index, 1, num_features, SORT_MODE_COLUMN_DIRECTION_DESCEND);

	/* Select the N corners */
	if (num_features < *N) {
		*N = num_features;
	}
	xy = (COORDINATE_XY*)malloc(*N * sizeof(COORDINATE_XY));
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

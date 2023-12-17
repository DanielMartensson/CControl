/*
 * fsm.c
 *
 *  Created on: 8 december 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

 /*
  * FAST Shape Model
  * X[m*n]
  */
void fsm(const uint8_t X[], const float Y[], const int* N, const float sigma, const uint8_t fast_threshold, const FAST_METHOD fast_method, const size_t row, const size_t column) {
	/* Do corner detection with FAST and Harris */
	FAST_XY* xy = cornerdetection(X, sigma, N, fast_threshold, fast_method, row, column);

	/* Check if N is zero */
	if (*N == 0) {
		free(xy);
		return;
	}

	/* Copy over xy to xy_float as a matrix */
	const size_t N2 = *N * 2;
	float* xy_float = (float*)malloc(N2 * sizeof(float));
	size_t i = 0, j = 0;
	while (i < N2) {
		xy_float[i] = (float)xy[j].x;
		i++;
		xy_float[i] = (float)xy[j].y;
		i++;
		j++;
	}

	/* Free */
	free(xy);

	/* Do DBSCAN */
	size_t* idx = (size_t*)malloc(*N * sizeof(size_t));
	//dbscan(xy_float, idx, epsilon, min_pts, *N, 2);

	/* Find how many clusters there are */
	i = 0;
	size_t max_index = idx[0];
	for (i = 1; i < *N; i++) {
		if (idx[i] > max_index) {
			max_index = idx[i];
		}
	}


}

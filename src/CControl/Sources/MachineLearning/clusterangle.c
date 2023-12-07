/*
 * clusterangle.c
 *
 *  Created on: 5 december 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

 /*
  * Compute the angle of a cluster
  * X[m*n]
  */
float clusterangle(const float X[], const float Y[], const size_t row_x, const size_t row_y, const size_t column) {
	/* Find all the distances D(i, j) between X(i, :) and Y(j, :) */
	float* D = (float*)malloc(row_x * row_y * sizeof(float));
	float* D0 = D;
	pdist2(Y, X, D, row_x, column, row_y, PDIST2_METRIC_EUCLIDEAN);

	/* Find the minimal values */
	size_t* min_indices = (size_t*)malloc(row_x * row_y * sizeof(size_t));
	size_t i, min_index;
	for (i = 0; i < row_x; i++) {
		/* Find the minimum value at D(i, :)
		 * This is the shortest distance for the closest points between X and Y
		 * MATLAB: [~, min_indices] = min(D, [], 2);
		 */
		amin(D, &min_index, row_y);
		min_indices[i] = min_index;

		/* Next row */
		D += row_y;
	}

	/* This computes the angle between clostest points between X and Y */
	float* angles_closest_points = (float*)malloc(row_x * sizeof(float));
	for (i = 0; i < row_x; i++) {
		angles_closest_points[i] = atan2f(Y[min_indices[i] * column + 1] - X[i * column + 1], Y[min_indices[i] * column] - X[i * column]);
	}

	/* Find the average of D */
	float value = mean(angles_closest_points, row_x);

	/* Free */
	free(D0);
	free(angles_closest_points);

	/* Return average */
	return value;
}
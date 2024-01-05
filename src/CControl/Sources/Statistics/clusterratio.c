/*
 * clusterratio.c
 *
 *  Created on: 5 december 2023
 *      Author: Daniel Mårtensson
 */

#include "statistics.h"

/* 
 * Compute the cluster ratio between the X and Y matrix
 * X[m*n]
 * Y[m*n]
 */
float clusterratio(const float X[], const float Y[], const size_t row_x, const size_t row_y, const size_t column) {
	/* Copy */
	float* X_copy = (float*)malloc(row_x * column * sizeof(float));
	float* Y_copy = (float*)malloc(row_y * column * sizeof(float));
	memcpy(X_copy, X, row_x * column * sizeof(float));
	memcpy(Y_copy, Y, row_y * column * sizeof(float));

	/* Center */
	float* mu = (float*)malloc(column * sizeof(float));
	center(X_copy, mu, row_x, column);
	center(Y_copy, mu, row_y, column);

	/* Compute scale factor */
	float scale_factor = 1.0f - clusterdistance(X_copy, Y_copy, row_x, row_y, column);

	/* Free */
	free(X_copy);
	free(Y_copy);
	free(mu);

	/* Return scale factor */
	return scale_factor;
}
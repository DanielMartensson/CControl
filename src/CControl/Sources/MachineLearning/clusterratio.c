/*
 * clusterratio.c
 *
 *  Created on: 5 december 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/* 
 * Compute the cluster ratio between the X and Y matrix
 * X[m*n]
 * Y[m*n]
 */
float clusterratio(const float X[], const float Y[], const size_t row, const size_t column) {
	/* Copy */
	const size_t total_bytes = row * column * sizeof(float);
	float* X_copy = (float*)malloc(total_bytes);
	float* Y_copy = (float*)malloc(total_bytes);
	memcpy(X_copy, X, total_bytes);
	memcpy(Y_copy, Y, total_bytes);

	/* Center */
	float* mu = (float*)malloc(column * sizeof(float));
	center(X_copy, mu, row, column);
	center(Y_copy, mu, row, column);

	/* Compute scale factor */
	float scale_factor = norm(X_copy, row, column, NORM_METHOD_FROBENIUS) / norm(Y_copy, row, column, NORM_METHOD_FROBENIUS);

	/* Free */
	free(X_copy);
	free(Y_copy);
	free(mu);

	/* Return scale factor */
	return scale_factor;
}
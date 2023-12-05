/*
 * clustersize.c
 *
 *  Created on: 5 december 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/* 
 * Compute the cluster size 
 * X[m*n]
 */
float clustersize(const float X[], const size_t row, const size_t column) {
	/* Find centroid of X */
	float* C = (float*)malloc(column * sizeof(float));
	centroid(X, C, row, column);

	/* Find all the distances D(1, j) between X(1, :) and Y(j, :) */
	float* D = (float*)malloc(row * sizeof(float));
	pdist2(C, X, D, 1, column, row, PDIST2_METRIC_EUCLIDEAN);

	/* Find the average of D */
	float value = mean(D, row);

	/* Free */
	free(C);
	free(D);

	/* Return average */
	return value;
}
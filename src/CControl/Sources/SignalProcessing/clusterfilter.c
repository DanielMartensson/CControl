/*
 * clusterfilter.c
 *
 *  Created on: 18 augusti 2023
 *      Author: Daniel Mårtensson
 */

#include "signalprocessing.h"

/*
 * Remove outliers from a custer
 * What this does is that ouliers will be replaced with average rows insde A
 * X[m*n]
 * epsilon = Raduis of the clusters
 * min_pts = Minimum points of a valid cluster
 */
size_t clusterfilter(const float X[], const size_t row, const size_t column, const float epsilon, const uint8_t min_pts){
	/* Create the new indexes */
	size_t* idx = (size_t*)malloc(row * sizeof(size_t));
	dbscan(X, idx, epsilon, min_pts, row, column);

	/* The rule is simple. Any index that holds 0 is noise and should be removed */
	size_t i;
	float* X0 = X;
	const size_t bytes_column = column * sizeof(float);
	for (i = 0; i < row; i++) {
		/* Check if it's noise */
		if (idx[i] == 0) {
			/* Yes - Remove it */
			memset(X, 0, bytes_column);
		}

		/* Next row */
		X += column;
	}

	/* Compute the mean of A */
	float* average = (float*)malloc(bytes_column);
	size_t j, count = 0;
	for (j = 0; j < column; j++) {
		/* Reset */
		average[j] = 0.0f;

		/* Go back to index 0 */
		X = X0;

		/* Sum the columns */
		count = 0;
		for (i = 0; i < row; i++) {
			if (idx[i] != 0) {
				average[j] += X[j];
				count++;
			}

			/* Next row */
			X += column;
		}

		/* Divide */
		if (count > 0) {
			average[j] /= (float)count;
		}
	}
	
	/* Replace all non index zero with average */
	X = X0;
	for (i = 0; i < row; i++) {
		if (idx[i] == 0) {
			memcpy(X, average, bytes_column);
		}

		/* Next row */
		X += column;
	}

	/* Free */
	free(idx);
	free(average);

	/* Return how many outliers it was */
	return row - count;
}
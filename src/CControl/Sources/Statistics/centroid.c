/*
 * centroid.c
 *
 *  Created on: 5 december 2023
 *      Author: Daniel Mårtensson
 */

#include "statistics.h"

/*
 * Compute the centroid of a cluster matrix X
 * X[m*n]
 * C[n]
 */
void centroid(const float X[], float C[], const size_t row, const size_t column) {
	size_t i, j;
	float* x = (float*)malloc(row * sizeof(float));
	const size_t stop_row = row - 1;
	for (i = 0; i < row; i++) {
		for (j = 0; j < column; j++) {
			cut(X, column, x, 0, stop_row, j, j);
			C[j] = mean(x, row);
		}
	}

	/* Free */
	free(x);
}
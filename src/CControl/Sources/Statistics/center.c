/*
 * center.c
 *
 *  Created on: 23 Juli. 2023
 *      Author: Daniel Mårtensson
 */

#include "statistics.h"

/*
 * Center the data matrix X by using X = X - mean(X)
 * X[m*n] - Input and output
 * mu[n]
 */
void center(float X[], float mu[], size_t row, size_t column) {
	size_t i, j;
	float* X0 = X;
	float average;
	/* Column-Major */
	tran(X, row, column);
	for (i = 0; i < column; i++) {
		/* Average data mu[i] = mean(X) */
		average = mean(X, row);

		/* Center the data X = X - mu */
		for (j = 0; j < row; j++) {
			X[j] -= average;
		}

		/* Save the average */
		mu[i] = average;

		/* Jump to next row */
		X += row;
	}
	X = X0;

	/* Turn it back to Row-Major */
	tran(X, column, row);
}
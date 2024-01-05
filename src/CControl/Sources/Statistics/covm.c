/*
 * covm.c
 *
 *  Created on: 1 april 2023
 *      Author: Daniel Mårtensson
 */

#include "statistics.h"

/*
 * Compute covariance of a matrix X and return covariance Y
 * X[m*n]
 * Y[n*n]
 */
void covm(float X[], float Y[], size_t row, size_t column) {
	/* Turn X into transpose because it's easier to handle the rows */
	tran(X, row, column);

	/* Compute the diagonal variance of X */
	size_t i, j, k;
	float* X0 = X;
	float* Y0 = Y;
	for (i = 0; i < column; i++) {
		Y[i] = var(X, row);
		X += row;
		Y += column;
	}

	/* Reset index */
	X = X0;
	Y = Y0;
	
	/* Compute variances of each row of X */
	uint32_t position;
	for (j = 0; j < column - 1; j++) {
		k = 0;
		for (i = j + 1; i < column; i++) {
			position = i * column + j;
			k++;
			Y[position] = cov(X, &X[row * k], row);
		
			/* Symmetric */
			Y[j * column + i] = Y[position];	
		}
		X += row;
	}
	/* Reset index */
	X = X0;

	/* Turn back X */
	tran(X, column, row);
}
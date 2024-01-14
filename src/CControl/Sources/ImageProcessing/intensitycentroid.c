/*
 * orientation.c
 *
 *  Created on: 3 December 2023
 *      Author: Daniel Mårtensson
 */

#include "imageprocessing.h"

/*
 * Get the orientation of the matrix by using Intensity Centroid
 * Use this if you want to find the principal rotation in a whole image, not parts of an image
 * X[m*n]
 */
float intensitycentroid(const float X[], const size_t radius, const size_t x, const size_t y, const size_t row, const size_t column) {
	/* Create variables */
	size_t i, j;
	float distance, pixel;
	float coordinate[2];

	/* Compute the square limits */
	const size_t start_row = y - radius;
	const size_t stop_row = y + radius;
	const size_t start_column = x - radius;
	const size_t stop_column = x + radius;

	/* Angle parameters */
	float x_sum = 0.0f;
	float y_sum = 0.0f;

	/* Iterate */
	for (i = start_row; i <= stop_row; i++) {
		for (j = start_column; j <= stop_column; j++) {
			/* Compute the distance by giving the row and column coordinates for L2-norm */
			coordinate[0] = (float)y - i;
			coordinate[1] = (float)x - j;
			distance = norm(coordinate, 2, 1, NORM_METHOD_L2);

			/* Check if distance is equal or less */
			if (distance <= radius) {
				pixel = X[i * column + j];
				y_sum += pixel * coordinate[0];
				x_sum += pixel * coordinate[1];
			}
		}
	}

	/* Compute the orientation */
	return atan2f(y_sum, x_sum);
}
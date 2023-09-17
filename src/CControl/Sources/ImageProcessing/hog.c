/*
 * hog.c
 *
 *  Created on: 6 september 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/*
 * Turn an image X into Histogram of Oriented Gradients
 * X[m*n]
 */
void hog(float X[], size_t row, size_t column, HOG_BINS number_of_bins, HOG_CELLS cell_size) {
	/* Compute gradients and orientations */
	const size_t total_size_bytes = row * column * sizeof(float);
	float* G = (float*)malloc(total_size_bytes);
	float* O = (float*)malloc(total_size_bytes);
	sobel(X, G, O, row, column);

	/* Create bins */
	float* bins = (float*)malloc(number_of_bins * sizeof(float));

}
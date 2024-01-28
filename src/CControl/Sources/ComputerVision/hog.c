/*
 * hog.c
 *
 *  Created on: 9 Januari 2024
 *      Author: Daniel Mårtensson
 */

#include "computervision.h"

/*
 * Histogram of Oriented Gradients (HOG)
 * X[m*n]
 * Y[
 */
static float* hog(const float X[], const size_t block_size, const HOG_BINS hog_bins, const size_t row, const size_t column, size_t* descriptors_row, size_t* descriptors_column, uint8_t* histogram_size) {
	/* Do sobel and find orientations */
	float* G = (float*)malloc(row * column * sizeof(float));
	float* O = (float*)malloc(row * column * sizeof(float));
	sobel(X, G, O, row, column, SOBEL_METHOD_GRADIENT_ORIENTATION);

	/* Turn O to unsigned degrees from radians */
	size_t i;
	const size_t row_column = row * column;
	for (i = 0; i < row_column; i++) {
		O[i] = fabsf(rad2deg(O[i]));
	}

	/* Create hog binaries */
	float angle_step;
	switch (hog_bins) {
	case HOG_BINS_9:
		/* 180/9 is 20 */
		angle_step = 20.0f;
		*histogram_size = 9;
		break;
	case HOG_BINS_10:
		angle_step = 18.0f;
		*histogram_size = 10;
		break;
	case HOG_BINS_12:
		angle_step = 15.0f;
		*histogram_size = 12;
		break;
	case HOG_BINS_15:
		angle_step = 12.0f;
		*histogram_size = 15;
		break;
	case HOG_BINS_18:
		angle_step = 10.0f;
		*histogram_size = 18;
		break;
	case HOG_BINS_20:
		angle_step = 9.0f;
		*histogram_size = 20;
		break;
	default:
		angle_step = 20.0f;
		*histogram_size = 9;
		break;
	}

	/* Clear the histogram */
	const size_t histogram_byte_size = (*histogram_size) * sizeof(float);
	float* histogram = (float*)malloc(histogram_byte_size);

	/* Create O_part and G_part */
	float* G_part = (float*)malloc(block_size * block_size * sizeof(float));
	float* O_part = (float*)malloc(block_size * block_size * sizeof(float));

	/* Create constants */
	const size_t blocks_blocks = block_size * block_size;
	const size_t row_minus_block_size = row - block_size;
	const size_t column_minus_block_size = column - block_size;
	const uint8_t histogram_size_minus_1 = (*histogram_size) - 1;

	/* Create variables */
	size_t j, k;
	uint8_t index, first, second;
	float ratio, modulo;

	/* Create the descriptors */
	*descriptors_row = row_minus_block_size / block_size;
	*descriptors_column = (*histogram_size) * (column_minus_block_size / block_size);
	float* descritptors = (float*)malloc((*descriptors_row) * (*descriptors_column) * sizeof(float));

	/* Create the descriptor */
	for (i = 0; i < row_minus_block_size; i += block_size) {
		for (j = 0; j < column_minus_block_size; j += block_size) {
			/* Cut out a part from gradients and orientation */
			cut(G, column, G_part, i, i + block_size - 1, j, j + block_size - 1);
			cut(O, column, O_part, i, i + block_size - 1, j, j + block_size - 1);

			/* Fill the histogram */
			memset(histogram, 0, histogram_byte_size);
			for (k = 0; k < blocks_blocks; k++) {
				/* Compute the ratio beween the angle and angle step */
				ratio = O_part[k] / angle_step;

				/* Compute the index for the histogram */
				index = (uint8_t)floorf(ratio);

				/* Compute first and second */
				if (index < histogram_size_minus_1) {
					first = index;
					second = index + 1;
				}
				else {
					first = histogram_size_minus_1;
					second = 0;
				}

				/* Compute modulo */
				modulo = fmodf(ratio, 1.0f);

				/* Insert */
				histogram[first] += G_part[k] * (1.0f - modulo);
				histogram[second] += G_part[k] * modulo;
			}

			/* Save */
			memcpy(descritptors, histogram, histogram_byte_size);

			/* Jump */
			descritptors += (*histogram_size);
		}
	}


	/* Free */
	free(O);
	free(G);
	free(G_part);
	free(O_part);
	free(histogram);

	/* Return the descriptors */
	return descritptors;
}
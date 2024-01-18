/*
 * landmarkdetection_collect.c
 *
 *  Created on: 9 Januari 2024
 *      Author: Daniel Mårtensson
 */

#include "imageprocessing.h"

/* 
 * Analyse landmarks
 * X[m*n]
 * x and y are coordinates of the pixel position
 */
uint32_t landmarkdetection_analyze(const uint8_t X[], const size_t row, size_t column, const int x, const int y) {
	/* Inital map */
	static const uint8_t map[81] = { 0, 0, 0, 1, 1, 1, 0, 0, 0,
									 0, 0, 1, 0, 0, 0, 1, 0, 0,
									 0, 1, 0, 0, 0, 0, 0, 1, 0,
									 1, 0, 0, 0, 0, 0, 0, 0, 1,
									 1, 0, 0, 0, 1, 0, 0, 0, 1,
									 1, 0, 0, 0, 0, 0, 0, 0, 1,
									 0, 1, 0, 0, 0, 0, 0, 1, 0,
									 0, 0, 1, 0, 0, 0, 1, 0, 0,
									 0, 0, 0, 1, 1, 1, 0, 0, 0,
	};

	/* Get the descriptor */
	uint32_t descriptor = 0;
	if (x >= 5 && y >= 5 && x <= column - 5 && y <= row - 5) {
		
	}

	/* Return the descriptor */
	return descriptor;
}

/*
 * Find landmarks
 * X[m*n]
 * descriptors[total_descriptors] - Need to contains binary 32-bit descriptor from Local Binary Pattern
 */
FAST_XY* landmarkdetection_collect(const uint8_t X[], int* N, const uint8_t descriptor_threshold, const uint32_t descriptors[], const uint8_t total_descriptors, const size_t row, const size_t column) {
	/* Collect all landmarks */
	FAST_XY* landmarks = NULL;
	int x, y, i, j;

	// Reset
	*N = 0;

	/* Interate the image */
	for (y = 5; y < row - 5; y++) {
		for (x = 5; x < column - 5; x++) {
			/* Find the descriptor from Local Binary Pattern */
			uint32_t descriptor = landmarkdetection_analyze(X, row, column, x, y);

			/* Classify the descriptor by rotating it and compare with hamming distance */
			uint8_t least_ones = 32;
			for (i = 0; i < total_descriptors; i++) {
				for (j = 0; j < 32; j++) {
					/* Compare with hamming distance by doing XOR */
					uint8_t counted_ones = popcount(descriptor ^ descriptors[i]);
					if (counted_ones < least_ones) {
						least_ones = counted_ones;
						if (least_ones == 0) {
							/* Best fit */
							i = total_descriptors;
							break;
						}
					}

					/* Rotate the descriptor one step to the right */
					descriptor = (descriptor >> 1) | (descriptor << 31);
				}
			}
			
			/* Threshold for saving the landmark  */
			if (least_ones <= descriptor_threshold) {
				landmarks = (FAST_XY*)realloc(landmarks, ((*N) + 1) * sizeof(FAST_XY));
				landmarks[*N].y = y;
				landmarks[*N].x = x;
				(*N)++;
			}
		}
	}

	/* Return landmarks */
	return landmarks;
}

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
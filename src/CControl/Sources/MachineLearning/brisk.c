/*
 * brisk.c
 *
 *  Created on: 7 oktober 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/*
 * Binary Robust Invariant Scalable Keypoints
 * X[m*n]
 * sigma1 - Gaussian blurr for the background
 * sigma2 - Gaussian blurr for whole image (easier to find descriptors if the image is blurry)
 * threshold_sobel - Make corners and edges more visible
 * threshold_fast - Threshold for the FAST algorithm
 * fast_method - Which type of FAST methods should be used
 * descriptor8 - Array output
 * descriptor16 - Array output
 * descriptor32 - Array output
 * descriptor64 - Array output
 * number_of_descriptors[4] - Array of 4 elements that shows how long the descriptors are
 */
void brisk(float X[], float sigma1, float sigma2, uint8_t threshold_sobel, int threshold_fast, FAST_METHOD fast_method, uint8_t* descriptor8[], uint16_t* descriptor16[], uint32_t* descriptor32[], uint64_t* descriptor64[], uint32_t number_of_descriptors[], size_t row, size_t column) {
	/* Apply gaussian blurr for making small objects not recognizable */
	if (sigma1 > 0.0f) {
		imgaussfilt(X, sigma1, row, column);
	}

	/* Apply sobel operator onto the blurred image X for finding the shapes - Also compute the orientations for later use */
	const size_t row_column = row * column;
	float* G = (float*)malloc(row_column * sizeof(float));
	float* O = (float*)malloc(row_column * sizeof(float));
	sobel(X, G, O, row, column, false);

	/* Apply FAST on the gradients for finding interests points onto the shapes */
	size_t i, j;
	uint8_t* Xuint8 = (uint8_t*)malloc(row_column);
	memset(Xuint8, 0, row_column);
	for (i = 0; i < row_column; i++) {
		if (G[i] > threshold_sobel) {
			Xuint8[i] = (float)G[i];
		}
	}

	/* Use FAST */
	int num_corners;
	FAST_XY* xy = fast(Xuint8, row, column, threshold_fast, &num_corners, fast_method);

	/* Make another blur for computing the descriptors */
	if (sigma2 > 0.0f) {
		imgaussfilt(X, sigma2, row, column);
	}

	/* Compute the descriptors */
	const uint8_t radius[4] = { 2U, 3U, 6U, 10U };
	const LBP_BIT lbp_bit[4] = { LBP_BIT_8, LBP_BIT_16, LBP_BIT_32, LBP_BIT_64 };
	memset(number_of_descriptors, 0, 4 * sizeof(int));
	for (i = 0; i < num_corners; i++) {
		/* Get coordinates for the interest points */
		const int x = xy[i].x;
		const int y = xy[i].y;

		/* Find the descriptors */
		for (j = 0; j < 4U; j++) {
			/* Compute the squre area limits */
			const int x_min = x - radius[j];
			const int y_min = y - radius[j];
			const int x_max = x + radius[j];
			const int y_max = y + radius[j];

			/* Check if the O_part is not at the edge of the image */
			if (x_min >= 0 && y_min >= 0 && x_max < column && y_max < row) {
				/* Cut one part from the orientation matrix O */
				const uint16_t descriptor_size = (x_max - x_min + 1) * (y_max - y_min + 1);
				float* O_part = (float*)malloc(descriptor_size * sizeof(float));
				cut(O, column, O_part, y_min, y_max, x_min, x_max);

				/* Then find the principal orientation, a.k.a the mean value of O_part */
				const float init_angle = mean(O_part, descriptor_size);
				free(O_part);

				/* Find the rotated descriptor with different radius */
				uint64_t descriptors = lbp(X, row, column, init_angle, radius[j], lbp_bit[j]);

				/* Count */
				number_of_descriptors[j]++;

				/* Store */
				switch (j) {
				case 0:
					*descriptor8 = (uint8_t*)realloc(*descriptor8, number_of_descriptors[j] * sizeof(uint8_t));
					(*descriptor8)[number_of_descriptors[j] - 1] = descriptors;
					break;
				case 1:
					*descriptor16 = (uint16_t*)realloc(*descriptor16, number_of_descriptors[j] * sizeof(uint16_t));
					(*descriptor16)[number_of_descriptors[j] - 1] = descriptors;
					break;
				case 2:
					*descriptor32 = (uint32_t*)realloc(*descriptor32, number_of_descriptors[j] * sizeof(uint32_t));
					(*descriptor32)[number_of_descriptors[j] - 1] = descriptors;
					break;
				case 3:
					*descriptor64 = (uint64_t*)realloc(*descriptor64, number_of_descriptors[j] * sizeof(uint64_t));
					(*descriptor64)[number_of_descriptors[j] - 1] = descriptors;
					break;
				}
			}
		}
	}

	/* Free */
	free(G);
	free(O);
	free(xy);
}
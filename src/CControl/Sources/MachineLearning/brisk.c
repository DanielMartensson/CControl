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
 */
void brisk(float X[], float sigma1, float sigma2, float threshold_sobel, int threshold_fast, FAST_METHOD fast_method, size_t row, size_t column) {
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
	uint64_t descriptors[4];
	bool descriptors_computed[4] = { false };
	for (i = 0; i < num_corners; i++) {
		/* Get coordinates for the interest points */
		const int x = xy[i].x;
		const int y = xy[i].y;

		/* Find the descriptors */
		for (j = 0; j < 4; j++) {
			/* Compute the area limits */
			const int x_min = x - radius[j];
			const int y_min = y - radius[j];
			const int x_max = x + radius[j];
			const int y_max = y + radius[j];

			/* Check if the O_part is not at the edge of the image */
			if (x_min >= 0 && y_min >= 0 && x_max < column && y_max < row) {
				/* Cut one part from the orientation matrix O */
				const uint8_t descriptor_size = radius[j] * radius[j];
				float* O_part = (float*)malloc(descriptor_size * sizeof(float));
				cut(O, column, O_part, y_min, y_max, x_min, x_max);

				/* Then find the principal orientation, a.k.a the mean value of O_part */
				const float init_angle = mean(O_part, descriptor_size);
				free(O_part);

				/* Find the rotated descriptor with radius 2, 3, 6, and 10 */
				switch (j) {
				case 0:
					descriptors[0] = lbp(X, row, column, 2.0f, init_angle, 2, LBP_BIT_8);
					break;
				case 1:
					descriptors[1] = lbp(X, row, column, 3.0f, init_angle, 3, LBP_BIT_16);
					break;
				case 2:
					descriptors[2] = lbp(X, row, column, 6.0f, init_angle, 6, LBP_BIT_32);
					break;
				case 3:
					descriptors[3] = lbp(X, row, column, 10.0f, init_angle, 10, LBP_BIT_64);
					break;
				}
			
				/* Flag */
				descriptors_computed[j] = true;
			}
			else {
				/* Flag */
				descriptors_computed[j] = false;
			}
		}

		/* Descriptor matching */
		for (j = 0; j < 4; j++) {
			if (descriptors_computed[j]) {

			}
		}
	}

	/* Free */
	free(G);
	free(O);
	free(xy);
}
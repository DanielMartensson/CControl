/*
 * brisk.c
 *
 *  Created on: 17 oktober 2023
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
 * histogram[1024] - Holders for the histogram descriptor
 */
void brisk(float X[], float sigma1, float sigma2, uint8_t threshold_sobel, int threshold_fast, FAST_METHOD fast_method, float histogram[], size_t row, size_t column) {
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
	const uint8_t radius[4] = { 4U, 8U, 12U, 16U };
	const LBP_BIT lbp_bit[4] = { LBP_BIT_8, LBP_BIT_16, LBP_BIT_24, LBP_BIT_32 };
	memset(histogram, 0, 1024 * sizeof(float));
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

				/* 
				 * Find the rotated descriptor index with different radius 
				 * If lgb_bit is 8-bit, then descriptor_index will be from 0 to 255
				 * If lgb_bit is 16-bit, then descriptor_index will be from 256 to 511
				 * If lgb_bit is 24-bit, then descriptor_index will be from 512 to 767
				 * If lgb_bit is 32-bit, then descriptor_index will be from 768 to 1023
				 */
				uint32_t descriptor_index = lbp(X, row, column, init_angle, radius[j], lbp_bit[j]);

				/* Scale descriptor index to 8 bit */
				switch (j) {
					/* case 0 is just 8-bit */
				case 1:
					/* 16-bit */
					descriptor_index = descriptor_index / 0x100U + 0x100U; /* Index from 256 to 511 */
					break;
				case 2:
					/* 24-bit */
					descriptor_index = descriptor_index / 0x10000U + 0x200U; /* Index from 512 to 767 */
					break;
				case 3:
					/* 32-bit */
					descriptor_index = descriptor_index / 0x1000000U + 0x300U; /* Index from 768 to 1023 */
					break;
				}

				/* Add to histogram */
				histogram[descriptor_index]++;
			}
		}
	}

	/* Free */
	free(G);
	free(O);
	free(Xuint8);
	free(xy);
}

/*
 C code for scaling the histogram

#include <stdio.h>
#include <stdint.h>

int main()
{
	uint16_t A = 0xFFFFU;
	uint32_t B = 0xFFFFFFU;
	uint32_t C = 0xFFFFFFFFU;

	float A1 = A/0x100U + 0x100U;
	float B1 = B/0x10000U + 0x200U;
	float C1 = C/0x1000000U + 0x300U;

	float D = 0;
	D++;
	D++;
	D++;

	printf("A1 = %f, B1 = %f, C1 = %f, D= %f\n", A1, B1, C1, D);

	return 0;
}
*/
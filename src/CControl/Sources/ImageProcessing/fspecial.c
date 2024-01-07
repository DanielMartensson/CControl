/*
 * fspecial.c
 *
 *  Created on: 28 November 2023
 *      Author: Daniel Mårtensson
 */

#include "imageprocessing.h"

/*
 * Create different types of matricies for image processing
 * output[output_size * output_size]
 * 
 * When FSPEICAL_TYPE_SOBEL_X,
 *		FSPEICAL_TYPE_SOBEL_Y,
 *		FSPECIAL_TYPE_BOX_OF_ONES
 *		is used, output_size can be NULL and value can be 0.0f
 */
float* fspecial(const float value, size_t* output_size, const FSPECIAL_TYPE type) {
	/* Create output */
	float* output = NULL;
	
	switch (type){
	case FSPECIAL_TYPE_GAUSSIAN_2D: {
		/* Create gaussian kernel size
		  kernel_size = round(6 * sigma);

		  % Create mesh grid
		  [x, y] = meshgrid(-kernel_size:kernel_size, -kernel_size:kernel_size);

		  % Create gaussian 2D kernel
		  K = 1/(2*pi*sigma^2)*exp(-(x.^2 + y.^2)/(2*sigma^2));
		*/
		const size_t kernel_size = roundf(6.0f * value);
		int step = kernel_size;

		/* Create mesh grid */
		const size_t xy_size = 2 * kernel_size + 1;
		*output_size = xy_size;
		const size_t xy_size_bytes = xy_size * sizeof(float);
		float* x = (float*)malloc(xy_size * xy_size * sizeof(float));
		float* y = (float*)malloc(xy_size * xy_size * sizeof(float));
		float* y0 = y;
		float* x0 = x;
		size_t i, j;
		for (i = 0; i < xy_size; i++) {
			x[i] = (float)-step + i;
			y[i] = -step;
		}
		step--;
		y += xy_size;
		x += xy_size;
		for (i = 1; i < xy_size; i++) {
			memcpy(x, x0, xy_size_bytes);
			for (j = 0; j < xy_size; j++) {
				y[j] = -step;
			}
			step--;
			y += xy_size;
			x += xy_size;
		}
		x = x0;
		y = y0;

		/* Create gaussian 2D kernel */
		const float a = 1.0f / (2.0f * PI * value * value);
		const float b = 2.0f * value * value;
		output = (float*)malloc(xy_size * xy_size * sizeof(float));
		float* K = output;
		for (i = 0; i < xy_size; i++) {
			for (j = 0; j < xy_size; j++) {
				K[j] = a * expf(-(x0[j] * x0[j] + y0[j] * y0[j]) / b);
			}
			K += xy_size;
			x0 += xy_size;
			y0 += xy_size;
		}

		/* Free */
		free(x);
		free(y);
		break;
	}
	case FSPECIAL_TYPE_GAUSSIAN_1D: {
		/*
		  kernel_size = round(6 * sigma);
		  x = -kernel_size:kernel_size;
	      y = 1/(sqrt(2*pi)*sigma * exp(-x.^2/(2*sigma^2));
		 */
		const size_t kernel_size = roundf(6.0f * value);
		const size_t x_size = 2 * kernel_size + 1;
		*output_size = x_size;
		output = (float*)malloc(x_size * sizeof(float));
		const float a = 1.0f / (sqrtf(2.0f * PI) * value);
		const float b = 2.0f * value * value;
		size_t i;
		float x = -((float)kernel_size);
		for (i = 0; i < x_size; i++) {
			output[i] = a * expf(-x * x / b);
			x += 1.0f;
		}

		return output;
	}
	case FSPEICAL_TYPE_SOBEL_X: {
		/* Create kernel matrix */
		static const float kernel_x[9] = { -1.0f, 0.0f, 1.0f,
										   -2.0f, 0.0f, 2.0f,
										   -1.0f, 0.0f, 1.0f };
		output = kernel_x;

		/* Check if it's not NULL */
		if (output_size) {
			*output_size = 3;
		}
		break;
	}
	case FSPEICAL_TYPE_SOBEL_Y: {
		/* Create kernel matrix */
		static const float kernel_y[9] = { -1.0f, -2.0f, -1.0f,
											0.0f,  0.0f,  0.0f,
											1.0f,  2.0f,  1.0f };
		output = kernel_y;

		/* Check if it's not NULL */
		if (output_size) {
			*output_size = 3;
		}
		break;
	}
	case FSPECIAL_TYPE_BOX_OF_ONES: {
		/* Create kernel matrix */
		static const float kernel[9] = { 1.0f, 1.0f, 1.0f, 
										 1.0f, 1.0f, 1.0f, 
										 1.0f, 1.0f, 0.1f};
		output = kernel;

		/* Check if it's not NULL */
		if (output_size) {
			*output_size = 3;
		}
		break;
	}
	}

	/* Return output */
	return output;
}
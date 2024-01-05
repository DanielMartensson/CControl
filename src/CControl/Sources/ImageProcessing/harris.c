/*
 * harris.c
 *
 *  Created on: 28 November 2023
 *      Author: Daniel Mårtensson
 */

#include "imageprocessing.h"

/* 
 * Harris corner detection
 * X[m*n] - Image
 * H[m*n] - Harris score, highest peaks describe the coordinates of a corner
 */
void harris(const float X[], float H[], const float sigma, const uint8_t edge, const size_t row, const size_t column){
	/* Sobel */
	const size_t row_column = row * column;
	float* Ix = (float*)malloc(row_column * sizeof(float));
	float* Iy = (float*)malloc(row_column * sizeof(float));
	float* Ixy = (float*)malloc(row_column * sizeof(float));
	sobel(X, Ix, Iy, row, column, SOBEL_METHOD_GRADIENT_X_Y);

	/* Get gaussian matrix kernel */
	size_t K_size;
	float* K = fspecial(sigma, &K_size, FSPECIAL_TYPE_GAUSSIAN_1D);

	/* Raise by two */
	size_t i;
	for (i = 0; i < row_column; i++) {
		Ixy[i] = Ix[i] * Iy[i];
		Ix[i] = Ix[i] * Ix[i];
		Iy[i] = Iy[i] * Iy[i];
	}

	/* Do Gaussian blurring */
	float* Ix2 = (float*)malloc(row_column * sizeof(float));
	float* Iy2 = (float*)malloc(row_column * sizeof(float));
	float* Ixy2 = (float*)malloc(row_column * sizeof(float));
	conv2(Ix, K, Ix2, row, column, K_size, K_size, CONV_SHAPE_SAME_SEPARABLE_KERNEL);
	conv2(Iy, K, Iy2, row, column, K_size, K_size, CONV_SHAPE_SAME_SEPARABLE_KERNEL);
	conv2(Ixy, K, Ixy2, row, column, K_size, K_size, CONV_SHAPE_SAME_SEPARABLE_KERNEL);

	/* Free */
	free(Ix);
	free(Iy);
	free(Ixy);
	free(K);

	/* Find Harris score */
	const float k = 0.07f;
	for (i = 0; i < row_column; i++) {
		H[i] = Ix2[i] * Iy2[i] - Ixy2[i] * Ixy2[i] - k * (Ix2[i] + Iy2[i]) * (Ix2[i] + Iy2[i]);
	}

	/* Normalize */
	size_t index;
	float value = fabsf(amax(H, &index, row_column));
	for (i = 0; i < row_column; i++) {
		H[i] = 10000.0f / value * H[i];
	}

	/* Free */
	free(Ix2);
	free(Iy2);
	free(Ixy2);

	/* Set edges to zero */
	if (edge > 0) {
		size_t j;
		const size_t row_minus_edge = row - edge;
		const size_t column_minus_edge = column - edge;
		for (i = 0; i < row; i++) {
			for (j = 0; j < column; j++) {
				if (i < edge || i >= row_minus_edge || j < edge || j >= column_minus_edge) {
					H[i * column + j] = 0.0f;
				}
			}
		}
	}
}

/*
 GNU Octave code:
	 function H = harris_score(I)

	  % Gray scale
	  if(size(I, 3) > 1)
		I = rgb2gray(I);
	  end
	  I = double(I);

	  % Parameters
	  sigma=5;
	  r = 0;

	  % Sobel
	  dx = [-1 0 1; -2 0 2; -1 0 1];
	  dy = [-1 -2 -1; 0 0 0; 1 2 1];
	  Ix = conv2(I, dx, 'same');
	  Iy = conv2(I, dy, 'same');

	  % Create mesh grid
	  kernel_size = round(6 * sigma);
	  [x, y] = meshgrid(-kernel_size:kernel_size, -kernel_size:kernel_size);

	  % Create gaussian 2D kernel
	  g = 1/(2*pi*sigma^2)*exp(-(x.^2 + y.^2)/(2*sigma^2));

	  Ix2 = mc.conv2fft(Ix.^2, g);
	  Iy2 = mc.conv2fft(Iy.^2, g);
	  Ixy = mc.conv2fft(Ix.*Iy, g);

	  k = 0.04;
	  Hp = (Ix2.*Iy2 - Ixy.^2) - k*(Ix2 + Iy2).^2;

	  % Normalize
	  H = (10000/max(abs(Hp(:))))*Hp;

	  % set edges zeros
	  H(1:r,:) = 0;
	  H(:,1:r) = 0;
	  H(end-r+1:end,:) = 0;
	  H(:,end-r+1:end) = 0;
	end
*/
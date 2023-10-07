/*
 * imgaussfilt.c
 *
 *  Created on: 7 oktober 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/*
 * Gaussian 2D filtering of a matrix
 * X[m*n] - Mimumum limit is m >= 6*row and n >= 6*column
 */
void imgaussfilt(float X[], float sigma, size_t row, size_t column) {
	/* Create gaussian kernel size */
	const size_t kernel_size = roundf(6.0f * sigma);
	int step = kernel_size;

	/* Create mesh grid */
	const size_t xy_size = 2 * kernel_size + 1;
	const size_t xy_size_bytes = xy_size * sizeof(float);
	float* x = (float*)malloc(xy_size * xy_size * sizeof(float));
	float* y = (float*)malloc(xy_size * xy_size * sizeof(float));
	float* y0 = y;
	float* x0 = x;
	size_t i, j;
	for (i = 0; i < xy_size; i++) {
		x[i] = (float) -step + i;
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
	float a = 1.0f / (2.0f * PI * sigma * sigma);
	float b = 2.0f * sigma * sigma;
	float* K_g = (float*)malloc(xy_size * xy_size * sizeof(float));
	float* K_g0 = K_g;
	for (i = 0; i < xy_size; i++) {
		for (j = 0; j < xy_size; j++) {
			K_g0[j] = a * expf(-(x0[j]*x0[j] + y0[j] * y0[j])/b);
		}
		K_g0 += xy_size;
		x0 += xy_size;
		y0 += xy_size;
	}

	/* Do conv2 with FFT */
	float* Y = (float*)malloc(row * column * sizeof(float));
	conv2fft(X, Y, row, column, K_g, xy_size);

	/* Give the same brightness */
	a = 0.0f;
	b = 0.0f;
	const size_t row_column = row * column;
	for (i = 0; i < row_column; i++) {
		a += X[i];
		b += Y[i];
	}
	const float a_div_b = a / b;
	for (i = 0; i < row_column; i++) {
		X[i] = Y[i] * a_div_b;
	}

	/* Free */
	free(x);
	free(y);
	free(K_g);
	free(Y);
}

/*
* GNU Octave coce
	% Gaussian 2D filter
	% Input: X(Data matrix), Sigma(Standard deviation)
	% Output: Y(Filtered data matrix)
	% Example 1: [Y] = mi.imgaussfilt(X);
	% Example 2: [Y] = mi.imgaussfilt(X, sigma);
	% Author: Daniel Mårtensson, 24 September 2023

	function [Y] = imgaussfilt(varargin)
	  % Check if there is any input
	  if(isempty(varargin))
		error('Missing input')
	  end

	  % Get data matrix X
	  if(length(varargin) >= 1)
		X = varargin{1};
	  else
		error('Missing input data matrix X')
	  end

	  % Get the sigma
	  if(length(varargin) >= 2)
		sigma = varargin{2};
	  else
		sigma = 1;
	  end

	  % Gray scaled image
	  if(size(X, 3) > 1)
		X = rgb2gray(X);
	  end

	  % Create gaussian kernel size
	  kernel_size = round(6 * sigma);

	  % Create mesh grid
	  [x, y] = meshgrid(-kernel_size:kernel_size, -kernel_size:kernel_size);

	  % Create gaussian 2D kernel
	  K_g = 1/(2*pi*sigma^2)*exp(-(x.^2 + y.^2)/(2*sigma^2));

	  % Do conv2 with FFT
	  Y = mc.conv2fft(X, K_g);

	  % Give the same brightness
	  Y = Y*sum(X(:))/sum(Y(:));
	end
*/
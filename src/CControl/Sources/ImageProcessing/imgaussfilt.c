/*
 * imgaussfilt.c
 *
 *  Created on: 7 oktober 2023
 *      Author: Daniel Mårtensson
 */

#include "imageprocessing.h"

/*
 * Gaussian 2D filtering of a matrix
 * X[m*n] - Mimimum limit is m >= 6*row and n >= 6*column
 */
void imgaussfilt(float X[], float sigma, size_t row, size_t column) {
	/* Get gaussian kernel */
	size_t K_size;
	float* K_g = fspecial(sigma, &K_size, FSPECIAL_TYPE_GAUSSIAN_1D);

	/* Do conv2 */
	float* Y = (float*)malloc(row * column * sizeof(float));
	conv2(X, K_g, Y, row, column, K_size, 1, CONV_SHAPE_SAME_SEPARABLE_KERNEL);

	/* Free */
	free(K_g);

	/* Give the same brightness */
	float a = 0.0f;
	float b = 0.0f;
	const size_t row_column = row * column;
	size_t i;
	for (i = 0; i < row_column; i++) {
		a += X[i];
		b += Y[i];
	}
	const float a_div_b = a / b;
	for (i = 0; i < row_column; i++) {
		X[i] = Y[i] * a_div_b;
	}

	/* Free */
	free(Y);
}

/*
* GNU Octave code
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
	  Y = conv2(X, K_g, 'same');

	  % Give the same brightness
	  Y = Y*sum(X(:))/sum(Y(:));
	end
*/
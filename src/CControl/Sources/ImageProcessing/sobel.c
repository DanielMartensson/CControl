/*
 * sobel.c
 *
 *  Created on: 6 september 2020
 *      Author: Daniel Mårtensson
 */

#include "imageprocessing.h"

/*
 * Compute sobel filter
 * X[m*n] - Image
 * G[m*n] - Gradients with L2-norm
 * O[m*n] - Orientations in radians
 */
void sobel(const float X[], float G[], float O[], const size_t row, const size_t column, const SOBEL_METHOD sobel_method){
	/* Create kernels */
	const float* kernel_x = fspecial(0.0f, NULL, FSPEICAL_TYPE_SOBEL_X);
	const float* kernel_y = fspecial(0.0f, NULL, FSPEICAL_TYPE_SOBEL_Y);

	/* Do conv2 */
	const size_t total_size = row * column;
	float* Gx = (float*)malloc(total_size * sizeof(float));
	float* Gy = (float*)malloc(total_size * sizeof(float));
	conv2(X, kernel_x, Gx, row, column, 3U, 3U, CONV_SHAPE_SAME_NO_FFT);
	conv2(X, kernel_y, Gy, row, column, 3U, 3U, CONV_SHAPE_SAME_NO_FFT);

	/* 
	 * Find gradients G = sqrt(Gx^2 + Gy^2) 
	 * Find orientations O = atan2(Gy, Gx) 
	 */
	size_t i;
	switch (sobel_method) {
	case SOBEL_METHOD_GRADIENT:
		for (i = 0; i < total_size; i++) {
			G[i] = sqrtf(Gx[i] * Gx[i] + Gy[i] * Gy[i]);
		}
		break;
	case SOBEL_METHOD_ORIENTATION:
		for (i = 0; i < total_size; i++) {
			O[i] = atan2f(Gy[i], Gx[i]);
		}
		break;
	case SOBEL_METHOD_GRADIENT_ORIENTATION:
		for (i = 0; i < total_size; i++) {
			G[i] = sqrtf(Gx[i] * Gx[i] + Gy[i] * Gy[i]);
			O[i] = atan2f(Gy[i], Gx[i]);
		}
		break;
	case SOBEL_METHOD_GRADIENT_X_Y:
		memcpy(G, Gx, total_size * sizeof(float));
		memcpy(O, Gy, total_size * sizeof(float));
		break;
	default:
		/* None */
		break;
	}

	/* Free */
	free(Gx);
	free(Gy);
}

/*
GNU octave code

% Sobel filter
% Input: X(Data matrix)
% Output: G(Gradients), O(Orientations)
% Example 1: [G, O] = mi.sobel(X);
% Author: Daniel Mårtensson, 6 September 2023

function [G, O] = sobel(varargin)
  % Check if there is any input
  if(isempty(varargin))
	error('Missing input')
  end

  % Get X
  X = varargin{1};

  % Gray scaled image
  if(size(X, 3) > 1)
	X = rgb2gray(X);
  end

  % Create kernels for X-direction and Y-direction
  K_x = [-1 0 1; -2 0 2; -1 0 1];
  K_y = [-1 -2 -1; 0 0 0; 1 2 1];

  % Do conv2 with FFT
  Gx = conv2(X, K_x, 'same');
  Gy = conv2(X, K_y, 'same');

  % Compute the gradients
  G = sqrt(Gx.^2 + Gy.^2);

  % Compute the orientations
  O = atan2d(Gy, Gx);
end
*/
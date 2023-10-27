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
  */
BRISK* brisk(float X[], float sigma1, float sigma2, uint8_t threshold_sobel, uint8_t threshold_fast, FAST_METHOD fast_method, size_t row, size_t column) {
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
	size_t i;
	uint8_t* Xuint8 = (uint8_t*)malloc(row_column);
	memset(Xuint8, 0, row_column);
	for (i = 0; i < row_column; i++) {
		if (G[i] > threshold_sobel) {
			Xuint8[i] = (float)G[i];
		}
	}

	/* Create brisk model */
	BRISK* brisk_model = (BRISK*)malloc(sizeof(BRISK));
	brisk_model->data_column = 11U;
	brisk_model->data_row = 0U;
	brisk_model->data = NULL;

	/* Use FAST */
	brisk_model->xy = fast(Xuint8, row, column, threshold_fast, &brisk_model->num_corners, fast_method);

	/* Make another blur for computing the descriptors */
	if (sigma2 > 0.0f) {
		imgaussfilt(X, sigma2, row, column);
	}

	/* Cut one part from the orientation matrix O */
	const uint8_t radius = 16U;
	const uint16_t O_part_row = radius + radius + 1;
	float* O_part = (float*)malloc(O_part_row * O_part_row * sizeof(float));
	
	/* Fill histogram */
	for (i = 0; i < brisk_model->num_corners; i++) {
		/* Get coordinates for the interest points - x is column and y is row */
		const int x = brisk_model->xy[i].x;
		const int y = brisk_model->xy[i].y;

		/* Compute the squre area limits */
		const int x_min = x - radius;
		const int y_min = y - radius;
		const int x_max = x + radius;
		const int y_max = y + radius;

		/* Check if the O_part is not at the edge of the image */
		if (x_min >= 0 && y_min >= 0 && x_max < column && y_max < row) {
			/* Cut a part of the orientation */
			cut(O, column, O_part, y_min, y_max, x_min, x_max);

			/* Then find the principal orientation, a.k.a the mean value of O_part */
			size_t total_elements;
			float init_angle = area(O_part, O_part_row, &total_elements, AREA_METHOD_CIRCLE) / ((float)total_elements);

			/* Allocate memory for new row */
			const size_t data_size = brisk_model->data_row * brisk_model->data_column;
			brisk_model->data = (uint32_t*)realloc(brisk_model->data, (data_size + brisk_model->data_column) * sizeof(uint32_t));

			/* Shift */
			uint32_t* data0 = brisk_model->data;
			brisk_model->data += data_size;

			/* Clear */
			memset(brisk_model->data, 0, brisk_model->data_column);
			
			/* First 16 bit data */
			uint16_t data = lbp(X, row, column, x, y, init_angle, 8.0f, LBP_BIT_16);
			brisk_model->data[0] = data >> 8;
			brisk_model->data[1] = data;

			/* Second 16-bit data */
			data = lbp(X, row, column, x, y, init_angle, 7.0f, LBP_BIT_16);
			brisk_model->data[2] = data >> 8;
			brisk_model->data[3] = data;

			/* Third 16-bit data */
			data = lbp(X, row, column, x, y, init_angle, 5.6f, LBP_BIT_16);
			brisk_model->data[4] = data >> 8;
			brisk_model->data[5] = data;

			/* Fourth 16-bit data */
			data = lbp(X, row, column, x, y, init_angle, 4.6f, LBP_BIT_16);
			brisk_model->data[6] = data >> 8;
			brisk_model->data[7] = data;

			/* Fifth 16-bit data */
			data = lbp(X, row, column, x, y, init_angle, 3.51f, LBP_BIT_16);
			brisk_model->data[8] = data >> 8;
			brisk_model->data[9] = data;

			/* The last 8-bit data */
			brisk_model->data[10] = lbp(X, row, column, x, y, init_angle, 2.0f, LBP_BIT_8);

			/* Count */
			brisk_model->data_row++;

			/* Reset address */
			brisk_model->data = data0;
		}
	}

	/* Free */
	free(G);
	free(O);
	free(O_part);
	free(Xuint8);

	/* Return histogram */
	return brisk_model;
}

void briskfree(BRISK* brisk_model) {
	if (brisk_model->data) {
		free(brisk_model->data);
	}
	if (brisk_model->xy) {
		free(brisk_model->xy);
	}
	free(brisk_model);
}

/*
 GNU octave code:

 % Binary Robust Invariant Scalable Keypoints
% Input: X(image), sigma1(background filtering), sigma2(descriptor filtering), threshold_sobel(corner filtering), threshold_fast(corner threshold), fast_method(enter: 9, 10, 11, 12)
% Output: data(classification data), X1(filtered background), X2(filtered data for descriptors), G(gradients for the corners), corners, scores(corner scores)
% Example 1: [data, X1, X2, G, corners, scores] = mi.brisk(X, sigma1, sigma2, threshold_sobel, threshold_fast, fast_method);
% Author: Daniel Mårtensson, Oktober 19:e 2023

function [data, X1, X2, G, corners, scores] = brisk(varargin)
  % Check if there is any input
  if(isempty(varargin))
	error('Missing inputs')
  end

  % Get image
  if(length(varargin) >= 1)
	X = varargin{1};
  else
	error('Missing image')
  end

  % Get sigma1
  if(length(varargin) >= 2)
	sigma1 = varargin{2};
  else
	error('Missing sigma1')
  end

  % Get sigma2
  if(length(varargin) >= 3)
	sigma2 = varargin{3};
  else
	error('Missing sigma2')
  end

  % Get threshold sobel
  if(length(varargin) >= 4)
	threshold_sobel = varargin{4};
  else
	error('Missing threshold sobel')
  end

  % Get threshold fast
  if(length(varargin) >= 5)
	threshold_fast = varargin{5};
  else
	error('Missing threshold fast')
  end

  % Get fast method
  if(length(varargin) >= 6)
	fast_method = varargin{6};
  else
	error('Missing fast method')
  end

  % Compute rows and columns
  [m, n] = size(X);

  % Apply gaussian blurr for making small objects not recognizable
  if(sigma1 > 0)
	X1 = mi.imgaussfilt(X, sigma1);
  end

  % Apply sobel operator onto the blurred image X1 for finding the shapes - Also compute the orientations for later use
  [G, O] = mi.sobel(X1);

  % Apply FAST on the gradients for finding interests points onto the shapes
  G(G <= threshold_sobel) = 0;

  % Use FAST
  [corners, scores] = mi.fast(G, threshold_fast, fast_method);

  % Make another blur for computing the descriptors
  if(sigma2 > 0)
	X2 = mi.imgaussfilt(X1, sigma2);
  end

  % Compute the descriptors
  data = [];
  for i = 1:length(corners)
	% Get coordinates for the interest points
	x = corners(i, 1);
	y = corners(i, 2);

	% Compute the squre area limits
	x_min = x - 16;
	x_max = x + 16;
	y_min = y - 16;
	y_max = y + 16;

	% Check if the O_part is not at the edge of the image
	if(and(x_min >= 1, y_min >= 1, x_max <= n, y_max <= m))
	  % Cut one part from the orientation matrix O
	  O_part = O(y_min:y_max, x_min:x_max);

	  % Then find the principal orientation, a.k.a the mean value of O_part
	  init_angle = circleaverage(O_part);

	  % Get data
	  d1 = mi.lbp(X2, x, y, 8, init_angle, 16);
	  d2 = mi.lbp(X2, x, y, 7, init_angle, 16);
	  d3 = mi.lbp(X2, x, y, 5.6, init_angle, 16);
	  d4 = mi.lbp(X2, x, y, 4.6, init_angle, 16);
	  d5 = mi.lbp(X2, x, y, 3.51, init_angle, 16);
	  d6 = mi.lbp(X2, x, y, 2, init_angle, 8); % This is 8-bit, not 16-bit

	  % Create binary array of 8-bit
	  d = [bitshift(d1, -8), bitand(d1, 0xFF), bitshift(d2, -8), bitand(d2, 0xFF), bitshift(d3, -8), bitand(d3, 0xFF), bitshift(d4, -8), bitand(d4, 0xFF), bitshift(d5, -8), bitand(d5, 0xFF), d6];

	  % Save
	  data = [data; d];
	end
  end
end

function avg = circleaverage(X)
  % Compute row
  [m, n] = size(X);

  % Compute the radius
  radius = m / 2;

  % Total iterations
  counter = 0;
  s = 0.0;
  coordinate = zeros(1, 2);
  for i = 1:m
	for j = 1:n
	  % Compute the distance by giving the row and column coordinates for L2-norm
	  coordinate(1) = radius - i + 1;
	  coordinate(2) = radius - j + 1;
	  distance = norm(coordinate, 2);

	  % Check if distance is equal or less
	  if distance <= radius
		s = s + X(i, j);
		counter = counter + 1;
	  end
	end
  end

  % Compute the average
  avg = s / counter;
end
*/
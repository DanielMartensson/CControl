/*
 * lbp.c
 *
 *  Created on: 18 oktober 2023
 *      Author: Daniel Mårtensson
 */

#include "imageprocessing.h"

/*
 * Local Binary Pattern - Use the commented C-code inside lbp.c to tune in the radius
 * X[m*n] - The image
 * x(column) and y(row) - The coordinate of the center pixel P
 * radius - The radius of the circle
 * init_angle - The beginning of the 360 degrees angle in radian
 * angles[total_angles] - Angles in radians
 * total_angles - Total angles
 */
uint32_t lbp(const uint8_t X[], const size_t row, const size_t column, const size_t x, const size_t y, const float init_angle, const float radius, LBP_BIT lbp_bit) {
	/* Center pixel */
	const uint8_t P = X[y * column + x];

	/* Get the angle */
	uint8_t total_angles;
	switch (lbp_bit) {
	case LBP_BIT_8:
		total_angles = 8U;
		break;
	case LBP_BIT_16:
		total_angles = 16U;
		break;
	case LBP_BIT_24:
		total_angles = 24U;
		break;
	case LBP_BIT_32:
		total_angles = 32U;
		break;
	default:
		total_angles = 8U;
		break;
	}
	float* angles = radianvector(total_angles);

	/* Compute Local Binary Pattern */
	uint8_t i;
	uint32_t descriptor = 0U;
	for (i = 0; i < total_angles; i++) {
		/* Find the initial angle */
		const float angle = init_angle + angles[i];

		/* Find the coordinates for the descriptor */
		const size_t coordinate_row = roundf(y + radius * cosf(angle));
		const size_t coordinate_column = roundf(x + radius * sinf(angle));

		/* Compute the descriptor */
		descriptor |= (X[coordinate_row * column + coordinate_column] > P) << i;
	}

	/* Free */
	free(angles);

	/* Return the descriptor */
	return descriptor;
}

/* C-code for drawing the circle
#include "CControl/ccontrol.h"

void drawCircle(const const int M, const int N, const float radius, char matrix[], const float init_angle, const float angles[], const uint8_t total_angles) {
	size_t centerX = M / 2;
	size_t centerY = N / 2;

	for (int i = 0; i < total_angles; i++) {
		float angle = init_angle + angles[i];
		int x = roundf(centerX + radius * cosf(angle));
		int y = roundf(centerY + radius * sinf(angle));
		matrix[x * N + y] = '+';
	}
}

void printMatrix(int M, int N, char matrix[]) {
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			printf("%c ", matrix[i * N + j]);
		}
		printf("\n");
	}
}

int main() {
	// Size of the matrix
	int M = 100;
	int N = 100;
	char* matrix = (char*)malloc(M * N);

	// Change this
	uint8_t radius = 10;
	int init_angle = 0;

	// Angles in radians
	const float steps8[8] = { 0.0f, 0.7854f, 1.5708f, 2.3562f, 3.1416f, 3.927f, 4.7124f, 5.4978f };
	const float steps16[16] = { 0.0f, 0.3927f, 0.7854f, 1.1781f, 1.5708f, 1.9635f, 2.3562f, 2.7489f, 3.1416f, 3.5343f, 3.927f, 4.3197f, 4.7124f, 5.1051f, 5.4978f, 5.8905f };
	const float steps24[24] = { 0.0f, 0.2618f, 0.5236f, 0.7854f, 1.0472f, 1.3090f, 1.5708f, 1.8326f, 2.0944f, 2.3562f, 2.6180f, 2.8798f, 3.1416f, 3.4034f, 3.6652f, 3.9270f, 4.1888f, 4.4506f, 4.7124f, 4.9742f, 5.2360f, 5.4978f, 5.7596f, 6.0214f };
	const float steps32[32] = { 0.0f, 0.19635f, 0.3927f, 0.58905f, 0.7854f, 0.98175f, 1.1781f, 1.3744f, 1.5708f, 1.7671f, 1.9635f, 2.1598f, 2.3562f, 2.5525f, 2.7489f, 2.9452f, 3.1416f, 3.3379f, 3.5343f, 3.7306f, 3.927f, 4.1233f, 4.3197f, 4.516f, 4.7124f, 4.9087f, 5.1051f, 5.3014f, 5.4978f, 5.6941f, 5.8905f, 6.0868f };

	// Fill the matrix
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			matrix[i * N + j] = ' ';
		}
	}
	size_t centerX = M / 2;
	size_t centerY = N / 2;
	matrix[centerX * N + centerY] = '+';

	// Draw a circle
	drawCircle(M, N, 8, matrix, init_angle * PI / 180.0f, steps16, 16);
	drawCircle(M, N, 7, matrix, init_angle * PI / 180.0f, steps16, 16);
	drawCircle(M, N, 6, matrix, init_angle * PI / 180.0f, steps8, 8);
	drawCircle(M, N, 5, matrix, init_angle * PI / 180.0f, steps8, 8);
	drawCircle(M, N, 4, matrix, init_angle * PI / 180.0f, steps8, 8);
	drawCircle(M, N, 3, matrix, init_angle * PI / 180.0f, steps8, 8);


	// Print a circle
	printMatrix(M, N, matrix);

	// Free
	free(matrix);

	// Return zero
	return 0;
}
*/

/* GNU Octave code
% Local Binary Pattern of the centrum of an image
% Input: X(image), x(column coordinate of center pixel), y(row coordinate of center pixel), radius, init_angle(where to begin with counting), lbp_bit(enter: 8, 16, 24, 32)
% Output: descriptor(binary value)
% Example 1: [descriptor] = mi.lbp(X, x, y, radius, init_angle, lbp_bit);
% Author: Daniel Mårtensson, Oktober 18:e 2023

function [descriptor] = lbp(varargin)
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

  % Get x coordinate
  if(length(varargin) >= 2)
	x = varargin{2};
  else
	error('Missing x coordinate')
  end

  % Get y coordinate
  if(length(varargin) >= 3)
	y = varargin{3};
  else
	error('Missing y coordinate')
  end

  % Get radius
  if(length(varargin) >= 4)
	radius = varargin{4};
  else
	error('Missing radius')
  end

  % Get init angle
  if(length(varargin) >= 5)
	init_angle = varargin{5};
  else
	init_angle = 0;
  end

  % Get lbp_bit
  if(length(varargin) >= 6)
	lbp_bit = varargin{6};
  else
	lbp_bit = 8;
  end

  % Constants
  angles8 = [ 0.0, 0.7854, 1.5708, 2.3562, 3.1416, 3.927, 4.7124, 5.4978 ];
  angles16 = [ 0.0, 0.3927, 0.7854, 1.1781, 1.5708, 1.9635, 2.3562, 2.7489, 3.1416, 3.5343, 3.927, 4.3197, 4.7124, 5.1051, 5.4978, 5.8905 ];
  angles24 = [ 0.0, 0.2618, 0.5236, 0.7854, 1.0472, 1.3090, 1.5708, 1.8326, 2.0944, 2.3562, 2.6180, 2.8798, 3.1416, 3.4034, 3.6652, 3.9270, 4.1888, 4.4506, 4.7124, 4.9742, 5.2360, 5.4978, 5.7596, 6.0214 ];
  angles32 = [ 0.0, 0.19635, 0.3927, 0.58905, 0.7854, 0.98175, 1.1781, 1.3744, 1.5708, 1.7671, 1.9635, 2.1598, 2.3562, 2.5525, 2.7489, 2.9452, 3.1416, 3.3379, 3.5343, 3.7306, 3.927, 4.1233, 4.3197, 4.516, 4.7124, 4.9087, 5.1051, 5.3014, 5.4978, 5.6941, 5.8905, 6.0868 ];

  % Center coordinates
  [m, n] = size(X);

  % Get the center value
  P = X(y, x);

  % Initial parameters
  switch (lbp_bit)
  case 8
	angles = angles8;
  case 16
	angles = angles16;
  case 24
	angles = angles24;
  case 32
	angles = angles32;
  otherwise
	angles = angles8;
  end

  % Create the descriptor
  descriptor = 0;
  for i = 1:length(angles)
	% Compute the angle
	angle = init_angle + angles(i);

	% Find the circle coordinates
	coordinate_y = round(y + radius * cos(angle));
	coordinate_x = round(x + radius * sin(angle));

	% Check if the coordinate has a brighter color than centrum P
	descriptor = bitset(descriptor, i, X(coordinate_y, coordinate_x) > P);
  end
end
*/
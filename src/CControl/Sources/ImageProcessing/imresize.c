/*
 * imresize.c
 *
 *  Created on: 8 oktober 2023
 *      Author: Daniel Mårtensson
 */

#include "imageprocessing.h"

/*
 * Resize an image of the scale d that is larger than 1
 * X[m*n]
 * Y[m2*n2];
 * m2 = (2 * floorf(floorf(row / 2 + 1) / d) + 1)
 * n2 = (2 * floorf(floorf(column / 2 + 1) / d) + 1)
 */
void imresize(float X[], float Y[], float d, size_t row, size_t column) {
	/* The middle size */
	const size_t m1 = row / 2 + 1;
	const size_t n1 = column / 2 + 1;

	/* The shrink size */
	const size_t m2 = m1 / d;
	const size_t n2 = n1 / d;

	/* Do FFT2 */
	float* XI = (float*)malloc(row * column * sizeof(float));
	fft2(X, XI, row, column);

	/* Do shift onto the image */
	fftshift(X, row, column);
	fftshift(XI, row, column);

	/* Select middle of the image */
	const size_t Y_row = 2 * m2 + 1;
	const size_t Y_column = 2 * n2 + 1;
	const size_t Y_size = Y_row * Y_column;
	float* YI = (float*)malloc(Y_size * sizeof(float));
	cut(X, column, Y, m1 - m2 - 1, m1 + m2 - 1, n1 - n2 - 1, n1 + n2 - 1);
	cut(XI, column, YI, m1 - m2 - 1, m1 + m2 - 1, n1 - n2 - 1, n1 + n2 - 1);

	/* Do inverse FFT shift */
	size_t i;
	const float dd = d * d;
	for (i = 0; i < Y_size; i++) {
		Y[i] = Y[i] / dd;
		YI[i] = YI[i] / dd;
	}
	ifftshift(Y, Y_row, Y_column);
	ifftshift(YI, Y_row, Y_column);

	/* Do inverse FFT */
	ifft2(Y, YI, Y_row, Y_column);

	/* Free */
	free(XI);
	free(YI);
}

/*
 GNU octave code
	
	% Load image
	image = imread('lab.pgm');

	% Check if gray scale
	if(size(image, 3) > 1)
	  image = rgb2gray(image);
	end

	% Shrink rate
	d = 2;

	% Size of the image
	[m, n] = size(image);

	% The middle size
	m1 = floor(m/2) + 1;
	n1 = floor(n/2) + 1;

	% The shrink size
	m2 = floor(m1/d);
	n2 = floor(n1/d);

	% Do FFT2
	X = fft2(image);

	% Do shift onto the image
	X = fftshift(X);

	% Select middle of the image
	Y = X(m1 - m2:m1 + m2, n1 - n2:n1 + n2);

	% Do inverse FFT shift
	Y = ifftshift(Y/d^2);

	% Do inverse FFT
	Y = real(ifft2(Y));

	% Show image
	imshow(uint8(Y));
*/
/*
 * convfft.c
 *
 *  Created on: 28 December 2023
 *      Author: Daniel Mårtensson
 */

#include "linearalgebra.h"

 /*
  * Do convolutional vector multiplication by using FFT
  * a[row_a] - Input
  * b[row_a] - Output
  * k[row_k] - k_row MUST be an odd number e.g 5 or 3 or 7 etc.
  */
void convfft(const float a[], const float k[], float b[], const size_t row_a, const size_t row_k) {	
	/* Constants */
	const size_t N = (row_k - 1) / 2;
	const length = row_a + N;

	/* Do FFT on xr */
	float* xr = (float*)malloc(length * sizeof(float));
	float* xi = (float*)malloc(length * sizeof(float));
	memset(xr, 0, length * sizeof(float));
	memcpy(xr, a, row_a * sizeof(float));
	fft(xr, xi, length);

	/* Do FFT on y */
	float* yr = (float*)malloc(length * sizeof(float));
	float* yi = (float*)malloc(length * sizeof(float));
	memset(yr, 0, length * sizeof(float));
	memcpy(yr, k, row_k * sizeof(float));
	fft(yr, yi, length);

	/* This is complex number multiplication (c + d*i)*(e + f*i) = (c*e - d*f) + i*(c*f + d*e) */
	size_t i;
	float c, d, e, f;
	for (i = 0; i < length; i++) {
		c = xr[i];
		d = xi[i];
		e = yr[i];
		f = yi[i];
		xr[i] = c * e - d * f;
		xi[i] = c * f + d * e;
	}

	/* Do inverse FFT */
	ifft(xr, xi, length);

	/* Copy over to b vector */
	memcpy(b, xr + N, row_a * sizeof(float));

	/* Free */
	free(xr);
	free(xi);
	free(yr);
	free(yi);
}

/* GNU Octave code
	% Generate a random input vector x and a kernel k
	N = 30;
	Nk = 5;
	x = randn(1, N);  % N is the length of x
	k = randn(1, Nk); % Kernel of length 3

	% Apply convolution with 'same' and display the result
	conv_result_same = conv(x, k, 'same');

	% Compute the FFT of x and k
	t = (Nk-1)/2;
	x = [x zeros(1, t)];
	X = fft(x);
	k =  [k zeros(1, length(x)-Nk)];
	K = fft(k);

	% Perform element-wise multiplication in the frequency domain
	Y = X .* K;

	% Perform inverse FFT to obtain the result of convolution
	conv_result_fft = ifft(Y);

	conv_result_fft = conv_result_fft(1+t:end);
	% Trim the result to match the length of the input

	% Compare the results
	disp('Difference between the results:');
	disp(max(abs(conv_result_same - conv_result_fft)));

*/
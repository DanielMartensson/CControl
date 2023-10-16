/*
 * conv2fft.c
 *
 *  Created on: 4 september 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/*
 * Do convolutional matrix multiplication by using FFT2
 * A[row_a * column_a] - Input
 * B[row_a * column_a] - Output
 * K[row_k * row_k] - k_row MUST be an odd number e.g 5 or 3 or 7 etc.
 */
void conv2fft(float A[], float B[], size_t row_a, size_t column_a, float K[], size_t row_k) {
	/* Do a copy */
	const size_t total_length = row_a * column_a;
	const size_t bytes_A = total_length * sizeof(float);
	float* A_real = (float*)malloc(bytes_A);
	memcpy(A_real, A, bytes_A);
	
	/* Do FFT onto A */
	float* A_imag = (float*)malloc(bytes_A);
	fft2(A_real, A_imag, row_a, column_a);

	/* Create an empy zero kernel matrix with the same size of A */
	float* kernel_real = (float*)malloc(bytes_A);
	memset(kernel_real, 0, bytes_A);

	/* Compute the sizes */
	const size_t row_middle = row_k / 2;
	const size_t column_middle = row_middle;

	/* Insert kernel */
	size_t i, j;
	for (i = 0; i <= row_middle; i++) {
		for (j = 0; j <= column_middle; j++) {
			kernel_real[i * column_a + j] = K[(i + row_middle) * row_k + (j + column_middle)];
		}
	}
	for (j = 0; j <= column_middle; j++) {
		kernel_real[(row_a-1) * column_a + j] = K[j + column_middle];
	}
	for (i = 0; i <= row_middle; i++) {
		kernel_real[i * column_a + column_a - 1] = K[(i + row_middle) * row_k];
	}
	kernel_real[row_a * column_a - 1] = K[0];

	/* Do FFT on kernel */
	float* kernel_imag = (float*)malloc(bytes_A);
	fft2(kernel_real, kernel_imag, row_a, column_a);

	/* Dot product with (A_real + A_imag).*(kernel_real + kernel_imag) */
	float a, b, c, d;
	for (i = 0; i < total_length; i++) {
		/* This is complex number multiplication (a + b*i)*(c + d*i) = (a*c - b*d) + i*(a*d + b*c) */
		a = A_real[i];
		b = A_imag[i];
		c = kernel_real[i];
		d = kernel_imag[i];
		A_real[i] = a * c - b * d;
		A_imag[i] = a * d + b * c;
	}

	/* Do inverse FFT */
	ifft2(A_real, A_imag, row_a, column_a);

	/* Copy over to B */
	memcpy(B, A_real, bytes_A);
	
	/* Free */
	free(A_real);
	free(A_imag);
	free(kernel_real);
	free(kernel_imag);
}

/* GNU octave code
	% Convolution via FFT2
	% Input: X(Data matrix), K(Kernel matrix)
	% Output: C(Filtered data matrix)
	% Example 1: [C] = mc.conv2fft(X, K);
	% Author: Daniel Mårtensson, 24 September 2023

	function C = conv2fft(varargin)
	  % Check if there is any input
	  if(isempty(varargin))
		error('Missing input')
	  end

	  % Get input matrix X
	  if(length(varargin) >= 1)
		X = varargin{1};
	  else
		error('Missing input data matrix X')
	  end

	  % Get the kernel
	  if(length(varargin) >= 2)
		K = varargin{2};
	  else
		error('Missing kernel data matrix K')
	  end

	  % Create kernel
	  [m, n] = size(X);
	  kernel = zeros(m, n);
	  [m, n] = size(K);

	  % Compute the sizes
	  m_middle = ceil(m/2);
	  n_middle = ceil(n/2);

	  % Insert kernel
	  kernel(1:m_middle, 1:n_middle) = K(m_middle:end, n_middle:end);
	  kernel(end, 1:n_middle) = K(1, n_middle:end);
	  kernel(1:m_middle, end) = K(m_middle:end, 1);
	  kernel(end, end) = K(1,1);

	  % Do FFT2 on X and kernel
	  A = fft2(X);
	  B = fft2(kernel);

	  % Compute the convolutional matrix - real to remove zero imaginary numbers
	  C = real(ifft2(A.*B));
	end
*/
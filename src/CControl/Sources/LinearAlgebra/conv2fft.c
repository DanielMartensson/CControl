/*
 * conv2fft.c
 *
 *  Created on: 4 september 2023
 *      Author: Daniel Mårtensson
 */

#include "linearalgebra.h"

/*
 * Do convolutional matrix multiplication by using FFT2
 * A[row_a * column_a] - Input
 * B[row_a * column_a] - Output
 * K[row_k * row_k] - k_row MUST be an odd number e.g 5 or 3 or 7 etc.
 */
void conv2fft(const float A[], const float K[], float B[], const size_t row_a, const size_t column_a, const size_t row_k) {
	/* Do a copy */
	const size_t total_length = row_a * column_a;
	const size_t bytes_A = total_length * sizeof(float);
	float* A_real = (float*)malloc(bytes_A);
	memcpy(A_real, A, bytes_A);
	
	/* Do FFT2 onto A */
	float* A_imag = (float*)malloc(bytes_A);
	fft2(A_real, A_imag, row_a, column_a);

	/* Create an empy zero kernel matrix with the same size of A */
	float* kernel_real = (float*)malloc(bytes_A);
	memset(kernel_real, 0, bytes_A);

	/* Compute the sizes */
	const size_t m_middle = ceilf(((float)row_k) / 2.0f);
	const size_t n_middle = m_middle;

	/* kernel(1:m_middle, 1:n_middle) = K(m_middle:end, n_middle:end); */
	size_t i, j;
	const size_t n_middle_minus_1 = n_middle - 1;
	const size_t m_middle_minus_1 = m_middle - 1;
	const size_t column_a_minus_n_middle = column_a - n_middle;
	const size_t row_a_minus_m_middle = row_a - m_middle;
	for (i = m_middle_minus_1; i < row_k; i++) {
		for (j = n_middle_minus_1; j < row_k; j++) {
			kernel_real[(i - m_middle_minus_1) * column_a + j - n_middle_minus_1] = K[i * row_k + j];
		}
	}

	/* kernel(end-m_middle+1:end, 1:n_middle) = K(1:m_middle, n_middle:end) */
	for (i = 0; i < m_middle; i++) {
		for (j = n_middle_minus_1; j < row_k; j++) {
			kernel_real[(row_a_minus_m_middle + i) * column_a + j - n_middle_minus_1] = K[i * row_k + j];
		}
	}

	/* kernel(1:m_middle, end-n_middle+1:end) = K(m_middle:end, 1:n_middle) */
	for (i = m_middle_minus_1; i < row_k; i++) {
		for (j = 0; j < n_middle; j++) {
			kernel_real[(i - m_middle_minus_1) * column_a + column_a_minus_n_middle + j] = K[i * row_k + j];
		}
	}

	/* kernel(end-m_middle+1:end, end-n_middle+1:end) = K(1:m_middle, 1:n_middle) */
	for (i = 0; i < m_middle; i++) {
		for (j = 0; j < n_middle; j++) {
			kernel_real[(row_a_minus_m_middle + i) * column_a + column_a_minus_n_middle + j] = K[i * row_k + j];
		}
	}

	/* Do FFT2 on kernel */
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

	function [C] = conv2fft(varargin)
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

	  % Get the sigma
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
	  n_middle = m_middle;

	  % Insert kernel
	  kernel(1:m_middle, 1:n_middle) = K(m_middle:end, n_middle:end);
	  kernel(end-m_middle+1:end, 1:n_middle) = K(1:m_middle, n_middle:end);
	  kernel(1:m_middle, end-n_middle+1:end) = K(m_middle:end, 1:n_middle);
	  kernel(end-m_middle+1:end, end-n_middle+1:end) = K(1:m_middle, 1:n_middle);

	  % Do FFT2 on X and kernel
	  A = fft2(X);
	  B = fft2(kernel);

	  % Compute the convolutional matrix - real to remove zero imaginary numbers
	  C = real(ifft2(A.*B));
	end
*/
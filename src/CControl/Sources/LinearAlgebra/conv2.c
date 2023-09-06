/*
 * conv2.c
 *
 *  Created on: 4 september 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/*
 * Do convolutional matrix multiplication by using FFT2
 * A[row_a * column_a] - Input
 * B[row_a * column_a] - Output
 * K[row_k * row_k]
 */
void conv2(float A[], float B[], size_t row_a, size_t column_a, float K[], size_t row_k) {
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
	float* kernel_real0 = kernel_real;

	/* Fill the upper left corner with K */
	size_t i;
	const size_t bytes_row_K = row_k * sizeof(float);
	for (i = 0; i < row_k; i++) {
		memcpy(kernel_real, K, bytes_row_K);

		/* New row */
		kernel_real += column_a;
		K += row_k;
	}
	kernel_real = kernel_real0;

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
	free(kernel_real);
	free(kernel_imag);
	free(A_imag);
}


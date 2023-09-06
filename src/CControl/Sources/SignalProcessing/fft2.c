/*
 * fft2.c
 *
 *  Created on: 3 september 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

 /* Import the library FFTPack */
#include "FFTpack/fftpack.h"

/* Special case when we want to turn complex into complex */
static void complex_to_complex_fft(float xr[], float xi[], size_t n);

/* Fast Fourier Transform
 * XR[m*n] - Real values
 * XI[m*n] - Imaginary values
 * This is exactly the same as fft2(A) = fft(fft(A).').' inside Matlab. The .' is very important
 */
void fft2(float XR[], float XI[], size_t row, size_t column) {
	/* Transpose XR for column major */
	tran(XR, row, column);

	/* Do FFT on each column of XR and XI */
	size_t i, shift;
	for (i = 0; i < column; i++) {
		shift = row * i;
		fft(XR + shift, XI + shift, row);
	}

	/* Do transpose */
	tran(XR, column, row);
	tran(XI, column, row);

	/* Do FFT on each row of XR and XI */
	for (i = 0; i < row; i++) {
		shift = column * i;
		complex_to_complex_fft(XR + shift, XI + shift, column);
	}
}

static void complex_to_complex_fft(float xr[], float xi[], size_t n) {
	/* Init */
	fftpack_real* wsave = (fftpack_real*)malloc((4 * n + 15) * sizeof(fftpack_real));
	cffti(n, wsave);

	/* Backward transform */
	fftpack_real* c = (fftpack_real*)malloc(2 * n * sizeof(fftpack_real));
	size_t i, index = 0;
	for (i = 0; i < n; i++) {
		c[index++] = xr[i];
		c[index++] = -xi[i]; /* This is the same as .' in Matlab */
	}
	cfftb(n, c, wsave);

	/* Fill */
	index = 0;
	for (i = 0; i < n; i++) {
		xr[i] = c[index++];
		xi[i] = -c[index++]; /* This is the same as .' in Matlab */
	}

	/* Free */
	free(wsave);
	free(c);
}
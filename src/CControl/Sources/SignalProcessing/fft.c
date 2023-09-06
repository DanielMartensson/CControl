/*
 * fft.c
 *
 *  Created on: 27 augusti 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/* Import the library FFTPack */
#include "FFTpack/fftpack.h"

/* 
 * Fast Fourier Transform 
 * xr[n] - Real values
 * xi[n] - Imaginary values
 * n - Data length
 */
void fft(float xr[], float xi[], size_t n) {
	/* Init */
	fftpack_real* wsave = (float*)malloc((2 * n + 15) * sizeof(fftpack_real));
	rffti(n, wsave);

	/* Forward transform */
	rfftf(n, xr, wsave);

	/* Fixing imaginary numbers */
	size_t i;
	memset(xi, 0, n * sizeof(float));
	size_t index = 0;
	xi += 1;
	for (i = 2; i < n; i += 2) {
		xi[n - index - 2] = -xr[i];
		xi[index++] = xr[i];
		xr[i] = 0;
	}
	
	/* Pack */
	const size_t half = n/2 + 1;
	for (i = 2; i < half; i++) {
		index = i + 1 + (i - 2);
		xr[i] = xr[index];
	}

	/* Mirror */
	index = half - (n % 2 == 0); /* if n % 2 == 0 is true, then 1(evnd), else 0(odd) */
	for (i = half; i < n; i++) {
		index--;
		xr[i] = xr[index];
	}

	/* Free */
	free(wsave);
}
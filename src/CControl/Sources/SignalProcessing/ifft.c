/*
 * ifft.c
 *
 *  Created on: 28 augusti 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

 /* Import the library FFTPack */
#include "FFTpack/fftpack.h"

/* Fast Fourier Transform
 * x[n] - Will contains the inverse FFT values. The first value will have no imaginary part
 */
void ifft(float x[], size_t n) {

	/* Init */
	float* wsave = (float*)malloc((2 * n + 15) * sizeof(float));
	int* ifac = (int*)malloc((n + 15) * sizeof(float));
	__ogg_fdrffti(n, wsave, ifac);

	/* Backward transform */
	__ogg_fdrfftb(n, x, wsave, ifac);
	
	/* Important to divide all outputs with x[0] */
	size_t i;
	float first_value = x[0];
	for (i = 0; i < n; i++) {
		x[i] /= first_value;
	}

	/* Free */
	free(wsave);
	free(ifac);
}
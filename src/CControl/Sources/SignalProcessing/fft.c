/*
 * fft.c
 *
 *  Created on: 27 augusti 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/* Import the library FFTPack */
#include "FFTpack/fftpack.h"

/* Fast Fourier Transform 
 * x[n] - Will contains the FFT values. The first value will have no imaginary part
 */
void fft(float x[], size_t n) {

	/* Init */
	float* wsave = (float*)malloc((2 * n + 15) * sizeof(float));
	int* ifac = (int*)malloc((n + 15) * sizeof(float));
	__ogg_fdrffti(n, wsave, ifac);

	/* Forward transform */
	__ogg_fdrfftf(n, x, wsave, ifac);

	/* Free */
	free(wsave);
	free(ifac);
}
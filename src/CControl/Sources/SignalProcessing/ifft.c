/*
 * ifft.c
 *
 *  Created on: 28 augusti 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

 /* Import the library FFTPack */
#include "FFTpack_5_0/fftpack.h"

/* Fast Fourier Transform
 * x[n] - Will contains the inverse FFT values
 */
void ifft(float x[], size_t n) {
	/* Init */
	float* wsave = (float*)malloc((2 * n + 15) * sizeof(float));
	int* ifac = (int*)malloc((n + 15) * sizeof(float));
	rffti(n, wsave, ifac);

	/* Backward transform */
	rfftb(n, x, wsave, ifac);

	/* Free */
	free(wsave);
	free(ifac);

	/* Normalize */
	size_t i;
	for (i = 0; i < n; i++) {
		x[i] /= n;
	}
}
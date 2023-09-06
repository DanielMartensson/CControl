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
 * xr[n] - Real frequency domain values
 * xi[n] - Imaginary frequency domain values
 */
void ifft(float xr[], float xi[], size_t n) {
	/* Init */
	fftpack_real* wsave = (fftpack_real*)malloc((4 * n + 15) * sizeof(fftpack_real));
	cffti(n, wsave);

	/* Backward transform */
	const size_t c_size = 2 * n;
	fftpack_real* c = (fftpack_real*)malloc(c_size * sizeof(fftpack_real));
	size_t i, index = 0;
	for (i = 0; i < n; i++) {
		c[index++] = xr[i];
		c[index++] = xi[i];
	}
	cfftb(n, c, wsave);

	/* Fill */
	index = 0;
	for (i = 0; i < n; i++) {
		xr[i] = c[index++] / n;
		xi[i] = c[index++] / n;
	}

	/* Free */
	free(wsave);
	free(c);
}
/*
 * fft.c
 *
 *  Created on: 27 augusti 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/* Import the library FFTPack */
#include "FFTpack_5_1/fftpack.h"

/* 
 * Fast Fourier Transform 
 * xr[n] - Real values
 * xi[n] - Imaginary values
 */
void fft(float xr[], float xi[], size_t n) {
	/* Init */
	const integer N = n;
	const integer lensav = 2 * N + (integer)(logf((real)N) / logf(2.0f)) + 4;
	integer ier;
	real* wsave = (float*)malloc(lensav * sizeof(real));
	cfft1i_(&N, wsave, &lensav, &ier);

	/* Forward transform */
	const integer inc = 1;
	const integer lenc = inc * (N - 1) + 1;
	const integer lenwrk = 2 * N;
	real* work = (real*)malloc(lenwrk * sizeof(real));
	complex* c = (complex*)malloc(lenc * sizeof(complex));
	memset(c, 0, lenc * sizeof(complex));
	size_t i;
	for (i = 0; i < n; i++) {
		c[i].r = xr[i];
	}
	cfft1f_(&N, &inc, c, &lenc, wsave, &lensav, work, &lenwrk, &ier);

	/* Fill */
	for (i = 0; i < n; i++) {
		xr[i] = c[i].r;
		xi[i] = c[i].i;
	}

	/* Free */
	free(wsave);
	free(c);
	free(work);
}
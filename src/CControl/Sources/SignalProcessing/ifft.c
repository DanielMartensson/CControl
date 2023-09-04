/*
 * ifft.c
 *
 *  Created on: 28 augusti 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

 /* Import the library FFTPack */
#include "FFTpack_5_1/fftpack.h"

/* Fast Fourier Transform
 * xr[n] - Real frequency domain values
 * xi[n] - Imaginary frequency domain values
 */
void ifft(float xr[], float xi[], size_t n) {
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
	size_t i;
	for (i = 0; i < n; i++) {
		c[i].r = xr[i];
		c[i].i = xi[i];
	}
	cfft1b_(&N, &inc, c, &lenc, wsave, &lensav, work, &lenwrk, &ier);

	/* Fill */
	memset(xi, 0, n * sizeof(float));
	for (i = 0; i < n; i++) {
		xr[i] = c[i].r;
	}

	/* Free */
	free(wsave);
	free(c);
	free(work);
}
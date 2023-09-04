/*
 * fft2.c
 *
 *  Created on: 3 september 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

 /* Import the library FFTPack */
#include "FFTpack_5_1/fftpack.h"

/* Fast Fourier Transform
 * XR[m*n] - Real values
 * XI[m*n] - Imaginary values
 */
void fft2(float XR[], float XI[], size_t row, size_t column) {
	/* Init */
	const integer L = row;
	const integer M = column;
	const integer lensav = 2 * (L + M) + (integer)(logf((real)L) / logf(2.0f)) + (integer)(logf((real)M) / logf(2.0f)) + 8;
	real* wsave = (real*)malloc(lensav * sizeof(real));
	integer ier;
	cfft2i_(&L, &M, wsave, &lensav, &ier);

	/* FFT forward */
	const integer LM = L * M;
	const integer lenwrk = 2 * LM;
	real* work = (real*)malloc(lenwrk * sizeof(real));
	complex* c = (complex*)malloc(LM * sizeof(complex));
	memset(c, 0, LM * sizeof(complex));
	size_t i;
	for (i = 0; i < LM; i++) {
		c[i].r = XR[i];
	}
	cfft2f_(&L, &L, &M, c, wsave, &lensav, work, &lenwrk, &ier);

	/* Fill */
	for (i = 0; i < LM; i++) {
		XR[i] = c[i].r;
		XI[i] = c[i].i;
	}

	/* Free */
	free(wsave);
	free(work);
	free(c);
}
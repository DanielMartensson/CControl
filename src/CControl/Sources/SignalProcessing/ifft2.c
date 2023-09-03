/*
 * ifft2.c
 *
 *  Created on: 3 september 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

 /* Import the library FFTPack */
#include "FFTpack_5_1/fftpack.h"

/* Inverse Fast Fourier Transform
 * x[m*n] - Will contains the FFT values.
 */
void ifft2(float X[], size_t row, size_t column) {
	/* Init */
	integer l = row;
	integer m = column;
	integer lensav = l + 3 * m + (integer)(logf((real)l) / logf(2.0f)) + 2 * (integer)(logf((real)m) / logf(2.0f)) + 12;
	real* wsave = (real*)malloc(lensav * sizeof(real));
	integer ier;
	rfft2i_(&l, &m, wsave, &lensav, &ier);

	/* FFT backward */
	integer lenwrk = (l + 1) * m;
	real* work = (real*)malloc(lenwrk * sizeof(real));
	rfft2b_(&l, &l, &m, X, wsave, &lensav, work, &lenwrk, &ier);

	/* Free */
	free(wsave);
	free(work);
}
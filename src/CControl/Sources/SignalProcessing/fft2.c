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
 * x[m*n] - Will contains the FFT values.
 */
void fft2(float X[], size_t row, size_t column) {
	/* Init */
	integer l = row;
	integer m = column;
	integer lensav = l + 3 * m + (integer)(logf((real)l) / logf(2.0f)) + 2 * (integer)(logf((real)m) / logf(2.0f)) + 12;
	real* wsave = (real*)malloc(lensav * sizeof(real));
	integer ier;
	rfft2i_(&l, &m, wsave, &lensav, &ier);

	/* FFT forward */
	integer lenwrk = (l + 1) * m;
	real* work = (real*)malloc(lenwrk * sizeof(real));
	rfft2f_(&l, &l, &m, X, wsave, &lensav, work, &lenwrk, &ier);

	/* Free */
	free(wsave);
	free(work);
}
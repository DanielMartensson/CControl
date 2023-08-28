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
#ifdef MKL_USED
	/* Create handle */
	DFTI_DESCRIPTOR_HANDLE handle = NULL;

	/* Prepare handle */
	DftiCreateDescriptor(&handle, DFTI_SINGLE, DFTI_REAL, 1, n);

	/* Commit */
	DftiCommitDescriptor(handle);

	/* Compute backward */
	DftiComputeBackward(handle, x);

	/* Delete model*/
	DftiFreeDescriptor(&handle);

#else 

	/* Init */
	float* wsave = (float*)malloc((2 * n + 15) * sizeof(float));
	int* ifac = (int*)malloc((n + 15) * sizeof(float));
	__ogg_fdrffti(n, wsave, ifac);

	/* Backward transform */
	__ogg_fdrfftb(n, x, wsave, ifac);
	
	/* Free */
	free(wsave);
	free(ifac);
#endif

	/* Normalize */
	size_t i;
	for (i = 0; i < n; i++) {
		x[i] /= n;
	}
}
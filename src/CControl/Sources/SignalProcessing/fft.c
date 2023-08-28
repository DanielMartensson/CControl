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
#ifdef MKL_USED
	/* Create handle */
	DFTI_DESCRIPTOR_HANDLE handle = NULL;

	/* Prepare handle */
	DftiCreateDescriptor(&handle, DFTI_SINGLE, DFTI_REAL, 1, n);

	/* Commit */
	DftiCommitDescriptor(handle);

	/* Compute forward */
	DftiComputeForward(handle, x);

	/* Delete model*/
	DftiFreeDescriptor(&handle);

#else 
	/* Init */
	float* wsave = (float*)malloc((2 * n + 15) * sizeof(float));
	int* ifac = (int*)malloc((n + 15) * sizeof(float));
	rffti(n, wsave, ifac);

	/* Forward transform */
	rfftf(n, x, wsave, ifac);

	/* Free */
	free(wsave);
	free(ifac);
#endif
}
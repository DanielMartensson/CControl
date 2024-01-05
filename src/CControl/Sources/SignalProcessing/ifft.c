/*
 * ifft.c
 *
 *  Created on: 28 augusti 2023
 *      Author: Daniel Mårtensson
 */

#include "signalprocessing.h"

 /* Import the library FFTPack */
#ifndef MKL_FFT_USED
#include "FFTpack/fftpack.h"
#endif

/* Fast Fourier Transform
 * xr[n] - Real frequency domain values
 * xi[n] - Imaginary frequency domain values
 */
void ifft(float xr[], float xi[], size_t n) {
#ifdef MKL_FFT_USED
	/* Load data */
	MKL_Complex8* data = (MKL_Complex8*)malloc(n * sizeof(MKL_Complex8));
	size_t i;
	for (i = 0; i < n; i++) {
		data[i].real = xr[i];
		data[i].imag = xi[i];
	}

	/* Prepare FFT */
	DFTI_DESCRIPTOR_HANDLE descriptor;
	DftiCreateDescriptor(&descriptor, DFTI_SINGLE, DFTI_COMPLEX, 1, n);
	DftiSetValue(descriptor, DFTI_BACKWARD_SCALE, 1.0f / n);
	DftiSetValue(descriptor, DFTI_PLACEMENT, DFTI_INPLACE);
	DftiCommitDescriptor(descriptor);

	/* Compute FFT */
	DftiComputeBackward(descriptor, data);

	/* Load */
	for (i = 0; i < n; i++) {
		xr[i] = data[i].real;
		xi[i] = data[i].imag;
	}

	/* Free */
	DftiFreeDescriptor(&descriptor);
	free(data);
#else

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
#endif
}
/*
 * fft.c
 *
 *  Created on: 27 augusti 2023
 *      Author: Daniel Mårtensson
 */

#include "signalprocessing.h"

/* Import the library FFTPack */
#ifndef MKL_FFT_USED
#include "FFTpack/fftpack.h"
#endif

/* 
 * Fast Fourier Transform 
 * xr[n] - Real values
 * xi[n] - Imaginary values
 * n - Data length
 */
void fft(float xr[], float xi[], size_t n) {
#ifdef MKL_FFT_USED
	/* Load data */
	MKL_Complex8* data = (MKL_Complex8*)malloc(n * sizeof(MKL_Complex8));
	size_t i;
	memset(data, 0, n * sizeof(MKL_Complex8));
	for (i = 0; i < n; i++) {
		data[i].real = xr[i];
	}

	/* Prepare FFT */
	DFTI_DESCRIPTOR_HANDLE descriptor;
	DftiCreateDescriptor(&descriptor, DFTI_SINGLE, DFTI_COMPLEX, 1, n);
	DftiSetValue(descriptor, DFTI_PLACEMENT, DFTI_INPLACE);
	DftiCommitDescriptor(descriptor);

	/* Compute FFT */
	DftiComputeForward(descriptor, data);

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
	fftpack_real* wsave = (float*)malloc((2 * n + 15) * sizeof(fftpack_real));
	rffti(n, wsave);

	/* Forward transform */
	rfftf(n, xr, wsave);

	/* Fixing imaginary numbers */
	size_t i;
	memset(xi, 0, n * sizeof(float));
	size_t index = 0;
	xi += 1;
	for (i = 2; i < n; i += 2) {
		xi[n - index - 2] = -xr[i];
		xi[index++] = xr[i];
		xr[i] = 0;
	}
	
	/* Pack */
	const size_t half = n/2 + 1;
	for (i = 2; i < half; i++) {
		index = i + 1 + (i - 2);
		xr[i] = xr[index];
	}

	/* Mirror */
	index = half - (n % 2 == 0); /* if n % 2 == 0 is true, then 1(even), else 0(odd) */
	for (i = half; i < n; i++) {
		index--;
		xr[i] = xr[index];
	}

	/* Free */
	free(wsave);
#endif
}
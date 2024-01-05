/*
 * fft2.c
 *
 *  Created on: 3 september 2023
 *      Author: Daniel Mårtensson
 */

#include "signalprocessing.h"

 /* Import the library FFTPack */
#ifndef MKL_FFT_USED
#include "FFTpack/fftpack.h"

/* Special case when we want to turn complex into complex */
static void complex_to_complex_fft(float xr[], float xi[], size_t n);
#endif

/* Fast Fourier Transform
 * XR[m*n] - Real values
 * XI[m*n] - Imaginary values
 * This is exactly the same as fft2(A) = fft(fft(A).').' inside Matlab. The .' is very important
 */
void fft2(float XR[], float XI[], size_t row, size_t column) {
#ifdef MKL_FFT_USED
	/* Load data */
	const size_t row_column = row * column;
	MKL_Complex8* data = (MKL_Complex8*)malloc(row_column * sizeof(MKL_Complex8));
	size_t i;
	MKL_LONG dim_sizes[2] = { row, column };
	memset(data, 0, row_column * sizeof(MKL_Complex8));
	for (i = 0; i < row_column; i++) {
		data[i].real = XR[i];
	}

	/* Prepare FFT */
	DFTI_DESCRIPTOR_HANDLE descriptor;
	DftiCreateDescriptor(&descriptor, DFTI_SINGLE, DFTI_COMPLEX, 2, dim_sizes);
	DftiSetValue(descriptor, DFTI_PLACEMENT, DFTI_INPLACE);
	DftiCommitDescriptor(descriptor);

	/* Compute FFT */
	DftiComputeForward(descriptor, data);

	/* Load */
	for (i = 0; i < row_column; i++) {
		XR[i] = data[i].real;
		XI[i] = data[i].imag;
	}

	/* Free */
	DftiFreeDescriptor(&descriptor);
	free(data);
#else
	/* Transpose XR for column major */
	tran(XR, row, column);

	/* Do FFT on each column of XR and XI */
	size_t i, shift;
	for (i = 0; i < column; i++) {
		shift = row * i;
		fft(XR + shift, XI + shift, row);
	}

	/* Do transpose */
	tran(XR, column, row);
	tran(XI, column, row);

	/* Do FFT on each row of XR and XI */
	for (i = 0; i < row; i++) {
		shift = column * i;
		complex_to_complex_fft(XR + shift, XI + shift, column);
	}
#endif
}

#ifndef MKL_FFT_USED
static void complex_to_complex_fft(float xr[], float xi[], size_t n) {
	/* Init */
	fftpack_real* wsave = (fftpack_real*)malloc((4 * n + 15) * sizeof(fftpack_real));
	cffti(n, wsave);

	/* Backward transform */
	fftpack_real* c = (fftpack_real*)malloc(2 * n * sizeof(fftpack_real));
	size_t i, index = 0;
	for (i = 0; i < n; i++) {
		c[index++] = xr[i];
		c[index++] = -xi[i]; /* This is the same as .' in Matlab */
	}
	cfftb(n, c, wsave);

	/* Fill */
	index = 0;
	for (i = 0; i < n; i++) {
		xr[i] = c[index++];
		xi[i] = -c[index++]; /* This is the same as .' in Matlab */
	}

	/* Free */
	free(wsave);
	free(c);
}
#endif
/*
 * ifft2.c
 *
 *  Created on: 3 september 2023
 *      Author: Daniel Mårtensson
 */

#include "signalprocessing.h"

 /* Import the library FFTPack */
#ifndef MKL_FFT_USED
#include "FFTpack/fftpack.h"
#endif

/* Inverse Fast Fourier Transform
 * XR[m*n] - Real frequency domain values
 * XI[m*n] - Imaginary frequency domain values
 * This is exactly the same as ifft2(A) = ifft(ifft(A).').' inside Matlab. The .' is very important
 */
void ifft2(float XR[], float XI[], size_t row, size_t column) {
#ifdef MKL_FFT_USED
	/* Load data */
	const size_t row_column = row * column;
	MKL_Complex8* data = (MKL_Complex8*)malloc(row_column * sizeof(MKL_Complex8));
	size_t i;
	MKL_LONG dim_sizes[2] = { row, column };
	for (i = 0; i < row_column; i++) {
		data[i].real = XR[i];
		data[i].imag = XI[i];
	}

	/* Prepare FFT */
	DFTI_DESCRIPTOR_HANDLE descriptor;
	DftiCreateDescriptor(&descriptor, DFTI_SINGLE, DFTI_COMPLEX, 2, dim_sizes);
	DftiSetValue(descriptor, DFTI_BACKWARD_SCALE, 1.0f / row_column);
	DftiSetValue(descriptor, DFTI_PLACEMENT, DFTI_INPLACE);
	DftiCommitDescriptor(descriptor);

	/* Compute FFT */
	DftiComputeBackward(descriptor, data);

	/* Load */
	for (i = 0; i < row_column; i++) {
		XR[i] = data[i].real;
		XI[i] = data[i].imag;
	}

	/* Free */
	DftiFreeDescriptor(&descriptor);
	free(data);
#else
	/* Transpose for column major */
	tran(XR, row, column);
	tran(XI, row, column);

	/* Do IFFT on each column of XR and XI */
	size_t i, shift;
	for (i = 0; i < column; i++) {
		shift = row * i;
		ifft(XR + shift, XI + shift, row);
	}

	/* Do transpose */
	tran(XR, column, row);
	tran(XI, column, row);

	/* Do FFT on each row of XR and XI */
	for (i = 0; i < row; i++) {
		shift = column * i;
		ifft(XR + shift, XI + shift, column);
	}
#endif
}
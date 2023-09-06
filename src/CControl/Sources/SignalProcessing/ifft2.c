/*
 * ifft2.c
 *
 *  Created on: 3 september 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

 /* Import the library FFTPack */
#include "FFTpack/fftpack.h"

/* Inverse Fast Fourier Transform
 * XR[m*n] - Real frequency domain values
 * XI[m*n] - Imaginary frequency domain values
 * This is exactly the same as ifft2(A) = ifft(ifft(A).').' inside Matlab. The .' is very important
 */
void ifft2(float XR[], float XI[], size_t row, size_t column) {
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
}
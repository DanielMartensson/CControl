/*
 * integralimage.c
 *
 *  Created on: 18 Januari 2024
 *      Author: Daniel Mårtensson
 */

#include "imageprocessing.h"

/*
 * Create an integral image
 * A[m*n] - Input
 * B[m*n] - Output
 */
void integralimage(const float A[], float B[], const size_t row, const size_t column) {
	/* Copy */
	const size_t row_column_bytes = row * column * sizeof(float);
	float* A_copy = (float*)malloc(row_column_bytes);
	memcpy(A_copy, A, row_column_bytes);

	/* Transpose A_copy */
	tran(A_copy, row, column);

	/* Do cumsum */
	cumsum(A_copy, B, column, row);

	/* Transpose B */
	tran(B, column, row);

	/* Do cumsum again - We borrow A_copy matrix as output */
	cumsum(B, A_copy, row, column);

	/* Copy over A_copy to B matrix */
	memcpy(B, A_copy, row_column_bytes);

	/* Free */
	free(A_copy);
}
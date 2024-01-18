/*
 * cumsum.c
 *
 *  Created on: Januari 18, 2024
 *      Author: Daniel Mårtensson
 */

#include "miscellaneous.h"

/*
 * Do cumulative sum
 * A[m*n] - Input
 * B[m*n] - Output
 */
void cumsum(const float A[], float B[], const size_t row, const size_t column) {
	size_t i, j;
	memcpy(B, A, row * column * sizeof(float));
	for (j = 0; j < column; j++) {
		for (i = 1; i < row; i++) {
			B[i * column + j] += B[(i-1) * column + j];
		}
	}
}
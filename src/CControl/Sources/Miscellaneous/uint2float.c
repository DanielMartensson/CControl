/*
 * uint2float.c
 *
 *  Created on: Jan 6, 2024
 *      Author: Daniel Mårtensson
 */

#include "miscellaneous.h"

/*
 * Turn uint8_t X to float X
 * X[m*n]
 */
float* uint2float(const uint8_t X[], const size_t row, const size_t column) {
	const size_t row_column = row * column;
	float* X_float = (float*)malloc(row_column * sizeof(float));
	size_t i;
	for (i = 0; i < row_column; i++) {
		X_float[i] = (float)X[i];
	}
	return X_float;
}
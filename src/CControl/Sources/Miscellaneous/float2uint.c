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
uint8_t* float2uint(const float X[], const size_t row, const size_t column) {
	const size_t row_column = row * column;
	uint8_t* X_uint8 = (uint8_t*)malloc(row_column * sizeof(uint8_t));
	size_t i;
	for (i = 0; i < row_column; i++) {
		X_uint8[i] = (uint8_t)X[i];
	}
	return X_uint8;
}
/*
 * sum.c
 *
 *  Created on: 8 août 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * A[m*n]
 * l = 1 sum on rows -> First row is the result
 * l = 2 sum on columns -> First column is the result
 */
void sum(float A[], uint16_t row, uint16_t column, uint8_t l) {
	if (l == 1) {
		for (uint16_t i = 1; i < row; i++) {
			for (uint16_t j = 0; j < column; j++) {
				A[j] += A[i * column + j];
			}
		}
	} else if (l == 2) {
		for (uint16_t i = 0; i < row; i++) {
			for (uint16_t j = 1; j < column; j++) {
				A[i * column] += A[i * column + j];
			}
		}
	}
}


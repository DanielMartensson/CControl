/*
 * eye.c
 *
 *  Created on: 29 Jaunari 2025
 *      Author: Daniel Mårtensson
 */

#include "linearalgebra.h"

/*
 * Create identity matrix I
 * I[row * column]
 */
void eye(float I[], const float value, const size_t row, const size_t column) {
	memset(I, 0, row * column * sizeof(float));
	size_t i;
	const size_t smallest = (size_t)vmin(row, column);
	for (i = 0; i < smallest; i++) {
		I[i * column + i] = value;
	}
}
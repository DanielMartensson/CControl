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
void eye(float I[], const size_t row, const size_t column) {
	memset(I, 0, row * column * sizeof(float));
	size_t i;
	for (i = 0; i < row; i++) {
		I[i * column] = 1.0f;
	}
}
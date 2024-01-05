/*
 * unit.c
 *
 *  Created on: 17 december 2023
 *      Author: Daniel Mårtensson
 */

#include "miscellaneous.h"

/*
 * Create a unit vector of x
 * x[m]
 */
void unit(float x[], const size_t row) {
	/* Create L2 Norm */
	const float R = norm(x, row, 1, NORM_METHOD_L2);
	size_t i;
	for (i = 0; i < row; i++) {
		x[i] /= R;
	}
}
/*
 * std.c
 *
 *  Created on: 26 juni 2021
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"


/*
 * Compute Bessel's Standard deviation
 * x[L] Vector with values
 * L = Length of vector x
 */
float stddev(float x[], uint16_t length) {
	return sqrtf(var(x, length));
}

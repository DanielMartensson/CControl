/*
 * std.c
 *
 *  Created on: 26 juni 2021
 *      Author: Daniel Mårtensson
 */

#include "statistics.h"

/*
 * Compute Bessel's Standard deviation
 * x[L] Vector with values
 * L = Length of vector x
 */
INLINE float stddev(const float x[], const size_t length) {
	return sqrtf(var(x, length));
}

/*
 * mean.c
 *
 *  Created on: 26 juni 2021
 *      Author: Daniel Mårtensson
 */

#include "statistics.h"

/*
 * Compute the mean of vector x
 * x[L] Vector with values
 * L = Length of vector x
 */
float mean(float x[], const size_t length) {
	float s = 0.0f;
	size_t i;
	for (i = 0; i < length; i++) {
		s += x[i];
	}
	return s / ((float)length);
}

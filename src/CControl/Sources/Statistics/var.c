/*
 * var.c
 *
 *  Created on: 1 april 2023
 *      Author: Daniel Mårtensson
 */

#include "statistics.h"

/*
 * Compute Bessel's variance
 * x[L] Vector with values
 * L = Length of vector x
 */
float var(const float x[], const size_t length) {
	const float mu = mean(x, length);
	float sigma = 0.0f;
	float p;
	size_t i;
	for (i = 0; i < length; i++) {
		p = x[i] - mu;
		sigma += p * p;
	}
	return sigma / ((float)(length - 1));
}
/*
 * cov.c
 *
 *  Created on: 1 april 2023
 *      Author: Daniel Mårtensson
 */

#include "statistics.h"

 /*
   * Compute covariance of two vectors
   * x[L] Vector with values
   * y[L] Vector with values
   * L = Length of vector x and y
   */
float cov(const float x[], const float y[], const size_t length) {
	float x_mu = mean(x, length);
	float y_mu = mean(y, length);
	float sigma = 0.0f;
	size_t i;
	for (i = 0; i < length; i++) {
		sigma += (x[i] - x_mu)*(y[i] - y_mu);
	}
	return sigma / ((float)(length - 1)); 
}
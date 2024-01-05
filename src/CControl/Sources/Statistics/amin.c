/*
 * amin.c
 *
 *  Created on: 4 december 2023
 *      Author: Daniel Mårtensson
 */

#include "statistics.h"

 /*
  * Compute the min value of vector x
  * x[L] Vector with values
  * min_index is the minimum index of the minimum value
  * L = Length of vector x
  */
float amin(const float x[], size_t* min_index, const size_t length) {
	/* Default settings */
	float min_value = x[0];
	*min_index = 0;

	/* Find if the default setting was not true */
	size_t i;
	for (i = 1; i < length; i++) {
		if (x[i] < min_value) {
			min_value = x[i];
			*min_index = i;
		}
	}
	return min_value;
}
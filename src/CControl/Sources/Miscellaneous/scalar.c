/*
 * scalar.c
 *
 *  Created on: 17 december 2023
 *      Author: Daniel Mårtensson
 */

#include "miscellaneous.h"

 /*
  * Scale a vector x
  * x[m]
  */
void scalar(float x[], const float k, const size_t row) {
	size_t i;
	for (i = 0; i < row; i++) {
		x[i] *= k;
	}
}
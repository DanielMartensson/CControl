/*
 * vmin.c
 *
 *  Created on: 27 dec. 2021
 *      Author: Daniel Mårtensson
 */

#include "miscellaneous.h"

/*
 * Return min value of a and b
 */
float vmin(const float a, const float b){
	if (a < b) {
		return a;
	}
	else {
		return b;
	}
}

/*
 * vmax.c
 *
 *  Created on: 27 dec. 2021
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Return max value of a and b
 */
float vmax(float a, float b){
	if (a > b) {
		return a;
	}
	else {
		return b;
	}
}


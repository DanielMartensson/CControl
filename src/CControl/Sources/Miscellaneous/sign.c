/*
 * sign.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "miscellaneous.h"

 /*
  * Sign function
  */
float sign(const float number) {
	if (number > 0.0f) {
		return 1.0f; 
	}else if (number < 0.0f) {
		return -1.0f; 
	}else{
		return 0.0f;
	}
}



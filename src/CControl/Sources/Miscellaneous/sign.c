/*
 * sign.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

 /*
  * Sign function
  */
float sign(float number) {
	float s;
	if (number > 0.0f) {
		s = 1.0f; 
	}else if (number < 0.0f) {
		s = -1.0f; 
	}else{
		s = 0.0f;
	}
	return s;
}



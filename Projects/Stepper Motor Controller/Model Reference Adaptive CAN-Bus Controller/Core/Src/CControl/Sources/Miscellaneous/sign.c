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
float sign(float number){
	float s;
	if(number > 0) s = 1;
	else if(number < 0) s = -1;
	else s = 0;
	return s;
}



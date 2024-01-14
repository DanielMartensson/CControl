/*
 * saturation.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "miscellaneous.h"

/*
 * This will saturate the input value with limits
 */
INLINE float saturation(const float input, const float lower_limit, const float upper_limit){
	if(input > upper_limit){
		return upper_limit;
	}else if(input < lower_limit){
		return lower_limit;
	}else{
		return input; /* No action */
	}
}

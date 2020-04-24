/*
 * saturation.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Configurations.h"
#include "../../Headers/Functions.h"

/*
 * This will saturate the input value with limits
 */
void saturation(float* input, float lower_limit, float upper_limit){
	if(*input > upper_limit){
		*input = upper_limit;
	}else if(*input < lower_limit){
		*input = lower_limit;
	}// else = nothing happens to *input
}

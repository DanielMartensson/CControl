/*
 * C89_Functions.c
 *
 *  Created on: 27 feb. 2023
 *      Author: Daniel Mårtensson
 */

#ifndef C89_Functions
#define C89_Functions
#ifndef _MSC_VER
#include "../../Headers/Functions.h"
#ifndef __cplusplus

 /* C99 has the __STDC_VERSION 199901L */
#if __STDC_VERSION__ < 199901L		
float sqrtf(float x){
	return (float) sqrt(x);
}

float fabsf(float x){
	return (float) fabs(x);
}

float acosf(float x){
	return (float) acos(x);
}

float expf(float x){
	return (float) exp(x);
}

float powf(float base, float power){
	return (float) pow(base, power);
}

float logf(float x){
	return (float) log(x);
}

float sinf(float x){
	return (float) sin(x);
}
#endif
#endif
#endif // !_MSC_VER
#endif

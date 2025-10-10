/*
 * functions.h
 *
 *  Created on: 5 oct. 2019
 *      Author: Daniel Mårtensson
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#ifdef __cplusplus
extern "C" {
#endif

#if __STDC_VERSION__ < 199901L
#if !defined(_MSC_VER) && !defined(ARM_IS_USED)
float sqrtf(float x) {
	return (float)sqrt(x);
}

float fabsf(float x) {
	return (float)fabs(x);
}

float acosf(float x) {
	return (float)acos(x);
}
/*
float atan2f(float x) {
	return (float)atan(x);
}
*/
float expf(float x) {
	return (float)exp(x);
}

float powf(float base, float power) {
	return (float)pow(base, power);
}

float logf(float x) {
	return (float)log(x);
}

float sinf(float x) {
	return (float)sin(x);
}

float tanhf(float x) {
	return (float)tanh(x);
}

float roundf(float x) {
	return (float)round(x);
}

float ceilf(float x) {
	return (float)ceil(x);
}

float floorf(float x) {
	return (float)floor(x);
}

float fmodf(float x, float y) {
	return (float)fmod(x, y);
}

#endif /* !defined(_MSC_VER) && !defined(ARM_IS_USED) */
#endif /* !__STDC_VERSION__ */
#ifdef __cplusplus
}
#endif

#endif /* !FUNCTIONS_H_ */
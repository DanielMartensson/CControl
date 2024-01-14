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

#if __STDC_VERSION___ < 199901L
#ifndef _MSC_VER
INLINE float sqrtf(float x) {
	return (float)sqrt(x);
}

INLINE float fabsf(float x) {
	return (float)fabs(x);
}

INLINE float acosf(float x) {
	return (float)acos(x);
}

INLINE float atan2f(float x) {
	return (float)atan(x);
}

INLINE float expf(float x) {
	return (float)exp(x);
}

INLINE float powf(float base, float power) {
	return (float)pow(base, power);
}

INLINE float logf(float x) {
	return (float)log(x);
}

INLINE float sinf(float x) {
	return (float)sin(x);
}

INLINE float tanhf(float x) {
	return (float)tanh(x);
}

float roundf(float x) {
	return (float)round(x);
}

INLINE float ceilf(float x) {
	return (float)ceil(x);
}

INLINE float floorf(float x) {
	return (float)floor(x);
}

INLINE float fmodf(float x, float y) {
	return (float)fmod(x, y);
}

#endif /* !_MSC_VER */
#endif /* !__STDC_VERSION___ */
#ifdef __cplusplus
}
#endif

#endif /* !FUNCTIONS_H_ */

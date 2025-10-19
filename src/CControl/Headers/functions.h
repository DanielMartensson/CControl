#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(__STDC_VERSION__) || (__STDC_VERSION__ < 199901L)

#if !defined(_MSC_VER)

#ifndef sqrtf
static float sqrtf(float x) {
    return (float)sqrt((double)x);
}
#endif

#ifndef fabsf
static float fabsf(float x) {
    return (float)fabs((double)x);
}
#endif

#ifndef acosf
static float acosf(float x) {
    return (float)acos((double)x);
}
#endif

#ifndef atan2f
static float atan2f(float x) {
	return (float)atan(x);
}
#endif

#ifndef expf
static float expf(float x) {
    return (float)exp((double)x);
}
#endif

#ifndef powf
static float powf(float base, float power) {
    return (float)pow((double)base, (double)power);
}
#endif

#ifndef logf
static float logf(float x) {
    return (float)log((double)x);
}
#endif

#ifndef sinf
static float sinf(float x) {
    return (float)sin((double)x);
}
#endif

#ifndef tanhf
static float tanhf(float x) {
    return (float)tanh((double)x);
}
#endif

#ifndef roundf
static float roundf(float x) {
    return (float)round((double)x);
}
#endif

#ifndef ceilf
static float ceilf(float x) {
    return (float)ceil((double)x);
}
#endif

#ifndef floorf
static float floorf(float x) {
    return (float)floor((double)x);
}
#endif

#ifndef fmodf
static float fmodf(float x, float y) {
    return (float)fmod((double)x, (double)y);
}
#endif

#endif /* !defined(_MSC_VER) */

#endif /*__STDC_VERSION__*/

#ifdef __cplusplus
}
#endif

#endif /* FUNCTIONS_H_ */
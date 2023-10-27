/*
 * defines.h
 *
 *  Created on: 20 Juli. 2023
 *      Author: Daniel Mårtensson
 */

#ifndef CCONTROL_HEADERS_DEFINES_H_
#define CCONTROL_HEADERS_DEFINES_H_

#ifndef __cplusplus
/* In ANSI C (C89), the __STDC_VERSION__ is not defined */
#ifndef __STDC_VERSION__
#define __STDC_VERSION__ 199409L		/* STDC version of C89 standard */
#endif

#if __STDC_VERSION__ < 199901L
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef uint8_t bool;
#define true 1
#define false 0
#ifndef _MSC_VER
#define NULL ((void *)0)
typedef unsigned long long size_t;
#endif /* !_MSC_VER */
#else
#include <stdbool.h>					/* For bool datatype */
#include <stdint.h>						/* For uint8_t, uint16_t and uint32_t etc. */
#endif /* !__STDC_VERSION__ */
#endif /* !__cplusplus */

 /* Define for all */
#define PI 3.14159265358979323846f		/* Constant PI */
#define MIN_VALUE 1e-11f				/* Tuning parameter for the smalles value that can be allowed */
#define MAX_ITERATIONS 10000U			/* For all iteration algorithsm */

/* Select library by uncomment - If non of these are uncomment, then CControl will use the internal library instead */
#define MKL_LAPACK_USED 				/* For large matrices on a regular computer */
#define MKL_FFT_USED					/* For large matrices on a regular computer */
/* #define CLAPACK_USED   				/* For larger embedded systems */

/* Load the MKL library */
#if defined(MKL_LAPACK_USED) || defined(MKL_FFT_USED)
#include <mkl.h>
#endif

#endif /* !CCONTROL_HEADERS_DEFINES_H_ */
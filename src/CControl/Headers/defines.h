/*
 * defines.h
 *
 *  Created on: 20 Juli. 2023
 *      Author: Daniel Mårtensson
 */

#ifndef CCONTROL_HEADERS_DEFINES_H_
#define CCONTROL_HEADERS_DEFINES_H_

/* In ANSI C (C89), the __STDC_VERSION__ is not defined */
#ifndef __STDC_VERSION__
#define __STDC_VERSION__ 199409L		/* STDC version of C89 standard */
#endif

#if __STDC_VERSION__ < 199901L
#define true 1
#define false 0
#endif /* !__STDC_VERSION___ */

#if __STDC_VERSION__ < 199901L
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef uint8_t bool;
#ifndef _MSC_VER
#define NULL ((void *)0)
typedef unsigned long size_t;
#endif /* !_MSC_VER */
#else
#include <stdbool.h>					/* For bool datatype */
#include <stdint.h>						/* For uint8_t, uint16_t and uint16_t */
#endif /* !__STDC_VERSION__ */

 /* Define for all */
#define PI 2*acosf(0.0f)				/* acos(0) is pi/2 */
#define MIN_VALUE 1e-14f				/* Tuning parameter for the smalles value that can be allowed */
#define MAX_ITERATIONS 10000U			/* For all iteration algorithsm */

/* If we are using a regular computer, we use LAPACK as much as possible */
#if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__) && defined(__MACH__)
#define LAPACK_USED 
#endif

#endif /* !CCONTROL_HEADERS_DEFINES_H_ */
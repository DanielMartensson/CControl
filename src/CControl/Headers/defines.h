/*
 * defines.h
 *
 *  Created on: 20 Juli. 2023
 *      Author: Daniel M?rtensson
 */

#ifndef DEFINES_H_
#define DEFINES_H_

#ifndef __cplusplus
/* In ANSI C (C89), the __STDC_VERSION__ is not defined */
#ifndef __STDC_VERSION__
#define __STDC_VERSION__ 199409L		/* STDC version of C89 standard */
#endif

#if __STDC_VERSION__ < 199901L
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef uint8_t bool;
#define true 1
#define false 0
#define IS_C89
#ifndef _MSC_VER
#define NULL ((void *)0)
typedef unsigned long long size_t;
#endif /* !_MSC_VER */
#else
#include <stdbool.h>					/* For bool datatype */
#include <stdint.h>						/* For uint8_t, uint16_t and uint32_t etc. */
#endif /* !__STDC_VERSION__ */
#endif /* !__cplusplus */

/* For memory leaks in Visual Studio */
#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC
#endif /* !_MSC_VER */

/* If ARM compiler is used */
#if defined(__aarch64__) || defined(__arm__)
#define ARM_IS_USED
#endif /* !!defined(__aarch64__) || !defined(__arm__) */

/* Define for all */
#define PI 3.14159265358979323846f		/* Constant PI */
#define MIN_VALUE 1e-11f				/* Tuning parameter for the smallest value that can be allowed */
#define MAX_ITERATIONS 10000U			/* For all iteration algorithms */
#define CONV_MAX_KERNEL_FFT_INSTEAD 80  /* When we are going to use FFT with conv or conv2 */

#endif /* !DEFINES_H_ */

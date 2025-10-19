#ifndef HEADERS_H_
#define HEADERS_H_

/* C standard library */
#include <string.h>						/* For memcpy, memset etc */
#include <stdio.h>						/* For printf */
#include <stdlib.h>						/* Standard library */
#include <math.h>						/* For sqrtf */
#include <float.h>						/* Required for FLT_EPSILON, FLT_MAX, FLT_MIN */
#include <stddef.h>						/* Requried for size_t,  NULL etc... */
#include <time.h> 						/* For srand, clock */
#include <stdarg.h>                                             /* For ... arguments */

/* Libraries for Visual Studio */
#ifdef _MSC_VER
#include <crtdbg.h>
#include <intrin.h>
#endif /* !_MSC_VER */

/* CControl headers */
#include "defines.h"
#include "enums.h"
#include "macros.h"
#include "functions.h"
#include "structs.h"
#include "unions.h"
#include "typedefs.h"

/* Load the MKL library */
#if defined(MKL_LAPACK_USED) || defined(MKL_FFT_USED)
#include <mkl.h>
#endif

/* CControl functions */
#include "../Sources/sources.h"

#endif /* !HEADERS_H_ */

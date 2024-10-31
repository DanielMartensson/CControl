#ifndef HEADERS_H_
#define HEADERS_H_

/* C89 standard library */
#include <string.h>						/* For memcpy, memset etc */
#include <stdio.h>						/* For printf */
#include <stdlib.h>						/* Standard library */
#include <math.h>						/* For sqrtf */
#include <float.h>						/* Required for FLT_EPSILON, FLT_MAX, FLT_MIN */
#include <stddef.h>						/* Requried for NULL */
#include <time.h> 						/* For srand, clock */

/* Libraries for Visual Studio */
#ifdef _MSC_VER
#include <crtdbg.h>
#include <intrin.h>
#endif /* !_MSC_VER */

/* Load the MKL library
#include <mkl.h>
*/

/* CControl headers */
#include "defines.h"
#include "enums.h"
#include "macros.h"
#include "functions.h"
#include "structs.h"
#include "unions.h"

/* CControl functions */
#include "../Sources/sources.h"

#endif /* !HEADERS_H_ */

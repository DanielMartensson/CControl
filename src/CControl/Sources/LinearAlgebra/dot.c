/*
 * dot.c
 *
 *  Created on: 25 April 2023
 *      Author: Daniel M�rtensson
 */

#include "linearalgebra.h"


/* Include LAPACK routines */
#ifdef CLAPACK_USED
#include "CLapack/clapack.h"
#endif

/*
 * Compute dot product of two vectors
 * a[m]
 * b[m]
 */
float dot(const float a[], const float b[], const size_t row) {
#ifdef CLAPACK_USED
    integer n = row;
    integer inc = 1;
    return (float) sdot_(&n, a, &inc, b, &inc);
#elif defined(MKL_LAPACK_USED)
    return cblas_sdot(row, a, 1, b, 1);
#else
    float result = 0.0f;
    size_t i;
    for (i = 0; i < row; i++) {
        result += a[i] * b[i];
    }
    return result;
#endif

}
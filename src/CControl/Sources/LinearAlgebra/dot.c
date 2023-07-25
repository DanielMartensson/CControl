/*
 * dot.c
 *
 *  Created on: 25 April 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/*
 * Compute dot product of two vectors
 * a[m]
 * b[m]
 */
float dot(float a[], float b[], size_t row) {
    float result = 0.0f;
    size_t i;
    for (i = 0; i < row; i++) {
        result += a[i] * b[i];
    }
    return result;
}
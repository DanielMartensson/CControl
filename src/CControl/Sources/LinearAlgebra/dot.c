/*
 * dot.c
 *
 *  Created on: 25 April 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

 /*
  * Compute dot product of two vectors
  * a[m]
  * b[m]
  */
float dot(float a[], float b[], uint16_t row) {
    float result = 0.0f;
    uint16_t i;
    for (i = 0; i < row; i++) {
        result += a[i] * b[i];
    }
    return result;
}
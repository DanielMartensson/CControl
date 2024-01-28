/*
 * integralimage.c
 *
 *  Created on: 18 Januari 2024
 *      Author: Daniel Mårtensson
 */

#include "imageprocessing.h"

/*
 * Create an integral image
 * A[m*n] - Input
 * B[m*n] - Output
 */
void integralimage(const uint8_t A[], uint32_t B[], const size_t row, const size_t column) {
    /* Copy the first row to the integral map */
    size_t i;
    for (i = 0; i < column; i++) {
        B[i] = A[i];
    }

    /* Compute the value of the integral maps for each row */
    size_t j;
    for (i = 1; i < row; i++) {
        for (j = 0; j < column; j++) {
            B[i * column + j] = B[(i - 1) * column + j] + A[i * column + j];
        }
    }

    /* Compute the value of the integral maps for each column */
    for (i = 0; i < row; i++) {
        for (j = 1; j < column; j++) {
            B[i * column + j] += B[i * column + j - 1];
        }
    }
}
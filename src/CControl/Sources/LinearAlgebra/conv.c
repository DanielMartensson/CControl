/*
 * conv.c
 *
 *  Created on: 26 December 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/*
 * 1D Convolution vector multiplication
 * a[row_a] - Input
 * k[row_k] - Input
 * 
 * b_full[row_a + row_k - 1]    - Output
 * b_same[row_a]                - Output
 * b_valid[row_a - row_k + 1]   - Output
 */
void conv(const float a[], const float k[], float b[], const size_t row_a, const size_t row_k, const CONV_SHAPE shape) {
    /* Variables */
    size_t i, j, idx;

    /* Select 1D convolution */
    switch (shape) {
    case CONV_SHAPE_FULL: {
        const size_t row_a_row_k_minus_1 = row_a + row_k - 1;
        for (i = 0; i < row_a_row_k_minus_1; i++) {
            b[i] = 0.0f;
            for (j = 0; j < row_k; j++) {
                if (i >= j && i - j < row_a) {
                    b[i] += a[i - j] * k[j];
                }
            }
        }
        break;
    }
    case CONV_SHAPE_SAME: {
        const size_t padding = row_k / 2;
        for (i = 0; i < row_a; i++) {
            b[i] = 0.0f;
            for (j = 0; j < row_k; j++) {
                idx = i >= padding ? i - padding + j : j;
                if (idx < row_a) {
                    b[i] += a[idx] * k[j];
                }
            }
        }
        break;
    }
    case CONV_SHAPE_VALID: {
        const size_t result_length = row_a >= row_k ? row_a - row_k + 1 : 0;
        for (i = 0; i < result_length; i++) {
            b[i] = 0.0f;
            for (j = 0; j < row_k; j++) {
                b[i] += a[i + j] * k[j];
            }
        }
        break;
    }
    }
}
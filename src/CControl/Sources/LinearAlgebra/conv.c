/*
 * conv.c
 *
 *  Created on: 26 December 2023
 *      Author: Daniel Mårtensson
 */

#include "linearalgebra.h"

/*
 * 1D Convolution vector multiplication
 * a[row_a] - Input
 * k[row_k] - Input
 * 
 * b_full[row_a + row_k - 1]    - Output
 * b_same[row_a - 1]            - Output
 * b_valid[row_a - row_k + 1]   - Output
 */
void conv(const float a[], const float k[], float b[], const size_t row_a, const size_t row_k, const CONV_SHAPE shape) {
    /* Variables */
    size_t i, j;

    /* Select 1D convolution */
    switch (shape) {
    case CONV_SHAPE_FULL: {
        const size_t padding = row_k - 1;
        const size_t row_a1 = row_a + 2 * padding;
        const size_t row_b = row_a + row_k - 1;
        memset(b, 0, row_b * sizeof(float));
        float* a1 = (float*)malloc(row_a1 * sizeof(float));
        memset(a1, 0, row_a1 * sizeof(float));
        memcpy(a1 + padding, a, row_a * sizeof(float));
        float* a2 = a1;
        for (i = 0; i < row_b; i++) {
            for (j = 0; j < row_k; j++) {
                b[i] += a1[j] * k[j];
            }

            /* New row */
            a1++;
        }

        /* Free */
        free(a2);

        break;
    }
    case CONV_SHAPE_SAME:
        /* Check if kernel size is over CONV_MAX_KERNEL_FFT_INSTEAD */
        if (row_k > CONV_MAX_KERNEL_FFT_INSTEAD) {
            /* row_k must be an odd number */
            if (row_k % 2 >= 1) {
                /* Do FFT instead */
                convfft(a, k, b, row_a, row_k);
                return;
            }
        }
        conv(a, k, b, row_a, row_k, CONV_SHAPE_SAME_NO_FFT);
        break;
    case CONV_SHAPE_SAME_NO_FFT: {
        const size_t padding = (row_k - 1) / 2;
        const size_t row_a1 = row_a + 2 * padding;
        memset(b, 0, row_a * sizeof(float));
        float* a1 = (float*)malloc(row_a1 * sizeof(float));
        memset(a1, 0, row_a1 * sizeof(float));
        memcpy(a1 + padding, a, row_a * sizeof(float));
        float* a2 = a1;
        for (i = 0; i < row_a; i++) {
            for (j = 0; j < row_k; j++) {
                b[i] += a1[j] * k[j];
            }

            /* New row */
            a1++;
        }

        /* Free */
        free(a2);

        break;
    }
    case CONV_SHAPE_VALID: {
        const size_t row_b = row_a - row_k + 1;
        memset(b, 0, row_b * sizeof(float));
        for (i = 0; i < row_b; i++) {
            for (j = 0; j < row_k; j++) {
                b[i] += a[j] * k[j];
            }

            /* Next row */
            a++;
        }
        break;
    }
    default:
        printf("No support for that shape yet\n");
        return;
    }
}


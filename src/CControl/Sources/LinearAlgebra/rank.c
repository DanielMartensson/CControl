/*
 * rank.c
 *
 *  Created on: 11 augusti 2023
 *      Author: Daniel Mårtensson
 */

#include "linearalgebra.h"

size_t rank(float A[], size_t row, size_t column) {
	/* Do QR factorization */
    float* R = (float*)malloc(row * column * sizeof(float));
    float* R0 = R;
    qr(A, NULL, R, row, column, true);

    /* Count non zero rows in R */
    size_t i, ranks = 0;
    for (i = 0; i < column; i++) {
        /* Count non zero in the diagnoal */
        if (fabsf(R[i]) > 0) {
            ranks++;
        }

        /* Next row */
        R += column;
    }

    /* Free */
    free(R0);

    /* Return ranks */
    return ranks;
}
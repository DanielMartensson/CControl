/*
 * fftshift.c
 *
 *  Created on: 8 oktober 2023
 *      Author: Daniel Mårtensson
 */

#include "signalprocessing.h"

/*
 * FFT shift
 * X[m*n]
 */
void fftshift(float X[], size_t row, size_t column) {
    /* Create the temporary matrix */
    float* temp = (float*)malloc(row * column * sizeof(float));

    /* Compute the middle index for both width and height */
    size_t midX = column / 2;
    size_t midY = row / 2;

    /* Fill */
    size_t i, j, x, y;
    float* X0 = X;
    for (i = 0; i < row; i++) {
        for (j = 0; j < column; j++) {
            /* Compute new index with fftshift */
            x = (j + midX) % column;
            y = (i + midY) % row;

            /* Copy X to the temporary matrix temp width a changed index */ 
            temp[y * column + x] = X0[j];
        }
        X0 += column;
    }

    /* Copy the results to the original matrix */
    memcpy(X, temp, row * column * sizeof(float));

    /* Free */
    free(temp);
}
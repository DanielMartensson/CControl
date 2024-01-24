/*
 * unique.c
 *
 *  Created on: Jan 24, 2024
 *      Author: Daniel Mårtensson
 */

#include "miscellaneous.h"

/*
 * Create an array of unique numbers 
 * A[N]
 * Return: B[length]
 */
float* unique(const float A[], const size_t N, size_t* length) {
    float* B = NULL;
    *length = 0; 
    size_t i, j;
    for (i = 0; i < N; i++) {
        bool isUnique = true;
        for (j = 0; j < *length; j++) {
            if (fabsf(A[i] - B[j]) < MIN_VALUE) {
                isUnique = false;
                break;
            }
        }

        if (isUnique) {
            (*length)++;
            B = (float*)realloc(B, *length * sizeof(float));
            B[*length - 1] = A[i];
        }
    }

    return B;
}
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
 * Sorting means that if the output array B should be sorted in ascending order
 */
float* unique(const float A[], const size_t N, size_t* length, const bool sorting) {
    float* B = NULL;
    *length = 0; 
    size_t i, j;
    for (i = 0; i < N; i++) {
        bool isUnique = true;
        for (j = 0; j < *length; j++) {
            if (fabsf(A[i] - B[j]) < FLT_EPSILON) {
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

    /* Sort */
    if (sorting) {
        size_t* index = (size_t*)malloc(*length * sizeof(size_t));
        sort(B, index, 1, *length, SORT_MODE_COLUMN_DIRECTION_ASCEND);

        /* Free */
        free(index);
    }


    return B;
}
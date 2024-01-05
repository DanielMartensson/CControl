/*
 * issymmetric.c
 *
 *  Created on: 1 Augusti 2023
 *      Author: Daniel Mårtensson
 */

#include "miscellaneous.h"

bool issymmetric(const float A[], const size_t row, const size_t column) {
	/* Check size */
	if (row != column) {
		return false;
	}

    /* Check the total matrix */
    size_t i, j;
    const float* B;
    const float* B0 = A;
    for (i = 0; i < row; i++) {
        B = B0;
        for (j = 0; j < column; j++) {
            if (fabsf(A[j] - B[i]) > MIN_VALUE) {
                return false;  
            }
            /*printf("A[%i] = %f, B[%i] = %f\n", j, A[j], i, B[i]);*/
            B += column;
        }
        A += column;
    }

    /* Yes */
    return true;
}
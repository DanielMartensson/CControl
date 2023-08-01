/*
 * issymmetric.c
 *
 *  Created on: 1 Augusti 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

bool issymmetric(float A[], size_t row, size_t column) {
	/* Check size */
	if (row != column) {
		return false;
	}

    /* Check the total matrix */
    size_t i, j;
    float* B = A;
    float* B0 = B;
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
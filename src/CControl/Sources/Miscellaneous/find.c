/*
 * find.c
 *
 *  Created on: 22 juli 2023
 *      Author: Daniel Mårtensson
 */

#include "miscellaneous.h"

/*
 * Find elements
 * A[m]
 * index[m]
 * m = row
 * condition FIND_CONDITION_METOD A[]
 * Returning the count of the find
 */
size_t find(const float A[], int32_t index[], const float condition, const size_t row, const FIND_CONDITION_METOD condition_method) {
	size_t i;
    size_t count = 0;
    memset(index, -1, row * sizeof(int32_t));
    for (i = 0; i < row; i++) {
        switch (condition_method) {
        case FIND_CONDITION_METOD_E:
            if (fabsf(condition - A[i]) < MIN_VALUE) {
                index[count++] = i;
            }
            break;
        case FIND_CONDITION_METOD_GE:
            if (condition >= A[i]) {
                index[count++] = i;
            }
            break;
        case FIND_CONDITION_METOD_G:
            if (condition > A[i]) {
                index[count++] = i;
            }
            break;
        case FIND_CONDITION_METOD_LE:
            if (condition <= A[i]) {
                index[count++] = i;
            }
            break;
        case FIND_CONDITION_METOD_L:
            if (condition < A[i]) {
                index[count++] = i;
            }
            break;
        }
    }
    return count;
}
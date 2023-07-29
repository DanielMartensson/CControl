/*
 * randperm.c
 *
 *  Created on: 28 juli 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/*
 * Return a row vector containing a random permutation of '0:N-1' of M values
 * A[M]
 */
void randperm(size_t A[], size_t N, size_t M) {
    /* Fill the array A */
    size_t i = 0, j, value;
    bool contains;
    srand(time(NULL));
    while (i < M) {
        /* Get random value */
        value = rand() % N;

        /* Check if A[] contains value */
        contains = false;
        for (j = 0; j < M; j++) {
            if (A[j] == value) {
                contains = true;
            }
        }

        /* If no */
        if (!contains) {
            A[i] = value;
            i++;
        }
        
    }
}
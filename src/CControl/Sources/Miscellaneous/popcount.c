/*
 * popcount.c
 *
 *  Created on: Nov 12, 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/*
 * Count the amount of 1:s inside a binary number
 */
uint8_t popcount(uint64_t x) {
    uint8_t count = 0U;
    while (x) {
        count += x & 1U;
        x >>= 1U;
    }
    return count;
}
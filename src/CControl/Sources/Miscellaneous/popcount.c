/*
 * popcount.c
 *
 *  Created on: Nov 12, 2023
 *      Author: Daniel Mårtensson
 */

#include "miscellaneous.h"

/*
 * Count the amount of 1:s inside a binary number
 */
uint8_t popcount(const uint64_t x) {
    uint64_t y = x;
    uint8_t count = 0U;
    while (y) {
        count += y & 1U;
        y >>= 1U;
    }
    return count;
}
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
INLINE uint8_t popcount(const uint32_t x) {
#ifdef _MSC_VER
    return __popcnt(x);
#elif defined(__GNUC__)
    return __builtin_popcount(x);
#else
    uint32_t y = x;
    uint8_t count = 0U;
    while (y) {
        count += y & 1U;
        y >>= 1U;
    }
    return count;
#endif /* _MSC_VER */
}
/*
 * normpdf.c
 *
 *  Created on: 10 januari 2024
 *      Author: Daniel Mårtensson
 */

#include "statistics.h"

/*
 * Compute the normal propability density function
 */
INLINE float normpdf(const float x, const float mu, const float sigma) {
	return 1.0f / (sigma * sqrtf(2.0f * PI)) * expf(-1.0f / 2.0f * (x - mu) * (x - mu) / (sigma * sigma));
}

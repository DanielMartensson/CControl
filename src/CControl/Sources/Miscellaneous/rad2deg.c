/*
 * rad2deg.c
 *
 *  Created on: 16 september 2023
 *      Author: Daniel M�rtensson
 */

#include "miscellaneous.h"

/*
 * Turn radians into degrees
 */
INLINE float rad2deg(const float radians) {
	return radians * 180.0f / PI;
}
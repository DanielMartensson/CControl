/*
 * deg2rad.c
 *
 *  Created on: 16 september 2023
 *      Author: Daniel Mårtensson
 */

#include "miscellaneous.h"

 /*
  * Turn degrees into radians
  */
INLINE float deg2rad(const float degrees) {
	return degrees * PI / 180.0f;
}
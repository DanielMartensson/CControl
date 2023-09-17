/*
 * rad2deg.c
 *
 *  Created on: 16 september 2023
 *      Author: Daniel M�rtensson
 */

#include "../../Headers/functions.h"

/*
 * Turn radians into degrees
 */
float rad2deg(float radians) {
	return radians * 180.0f / PI;
}
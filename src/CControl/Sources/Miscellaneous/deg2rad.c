/*
 * deg2rad.c
 *
 *  Created on: 16 september 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

 /*
  * Turn degrees into radians
  */
float deg2rad(float degrees) {
	return degrees * PI / 180.0f;
}
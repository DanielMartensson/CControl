/*
 * randi.c
 *
 *  Created on: Jan 27, 2024
 *      Author: Daniel Mårtensson
 */

#include "miscellaneous.h"

INLINE size_t randi(const size_t x) {
	return rand() % x;
}
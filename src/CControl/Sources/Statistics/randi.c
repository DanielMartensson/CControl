/*
 * randi.c
 *
 *  Created on: Jan 27, 2024
 *      Author: Daniel Mårtensson
 */

#include "statistics.h"

/* 
 * Compute random number between 0 and x - 1
 */
INLINE size_t randi(const size_t x) {
	return rand() % x;
}
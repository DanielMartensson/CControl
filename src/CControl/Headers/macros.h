/*
 * macros.h
 *
 *  Created on: 8 November 2023
 *      Author: Daniel M?rtensson
 */

#ifndef MACROS_H_
#define MACROS_H_

/* Compute array size */
#define ARRAY_SIZE(X) sizeof(X) / sizeof(sizeof(X[0]))	/* Length mesaurement for non-pointer array */

/* Swap macro */
#define SWAP(x, y, T) do { T SWAP = x; x = y; y = SWAP; } while (0)

#endif /* !MACROS_H_*/

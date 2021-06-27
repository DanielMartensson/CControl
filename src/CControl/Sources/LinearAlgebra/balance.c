/*
 * balanace.c
 *
 *  Created on: 25 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Balance a real matrix
 * A [m*n]
 */
void balance(float A[], uint16_t row){
	uint16_t i, j, last = 0;
	float s, r, g, f, c, sqrdx;

	sqrdx = 4.0;
	while (last == 0) {
		last = 1;
		for (i = 0; i < row; i++) {
			r = c = 0.0;
			for (j = 0; j < row; j++)
				if (j != i) {
					c += fabsf(*(A + row*j + i));
					r += fabsf(*(A + row*i + j));
				}
			if (c != 0.0 && r != 0.0) {
				g = r / 2.0;
				f = 1.0;
				s = c + r;
				while (c < g) {
					f *= 2.0;
					c *= sqrdx;
				}
				g = r * 2.0;
				while (c > g) {
					f /= 2.0;
					c /= sqrdx;
				}
				if ((c + r) / f < 0.95 * s) {
					last = 0;
					g = 1.0 / f;
					for (j = 0; j < row; j++)
						*(A + row*i + j) *= g;
					for (j = 0; j < row; j++)
						*(A + row*j + i) *= f;
				}
			}
		}
	}
}

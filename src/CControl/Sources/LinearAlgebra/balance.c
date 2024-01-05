/*
 * balanace.c
 *
 *  Created on: 25 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "linearalgebra.h"

/*
 * Balance a real matrix
 * A [m*n]
 */
void balance(float A[], size_t row){
	size_t i, j, last = 0;
	float s, r, g, f, c, sqrdx;

	/* Save memory */
	float *Ai;
	float *Aj;

	sqrdx = 4.0f;
	while (last == 0) {
		last = 1;
		Ai = A;
		for (i = 0; i < row; i++) {
			r = c = 0.0f;
			Aj = A;
			for (j = 0; j < row; j++){
				if (j != i) {
					c += fabsf(Aj[i]);
					/* c += fabsf(A[row*j + i]); */
					r += fabsf(Ai[j]);
					/* r += fabsf(A[row*i + j]); */
				}
				Aj += row;
			}
			if (c != 0.0f && r != 0.0f) {
				g = r / 2.0f;
				f = 1.0f;
				s = c + r;
				while (c < g) {
					f *= 2.0f;
					c *= sqrdx;
				}
				g = r * 2.0f;
				while (c > g) {
					f /= 2.0f;
					c /= sqrdx;
				}
				if ((c + r) / f < 0.95f * s) {
					last = 0;
					g = 1.0f / f;
					for (j = 0; j < row; j++){
						Ai[j] *= g;
						/* A[row*i + j] *= g; */
					}
					Aj = A;
					for (j = 0; j < row; j++){
						Aj[i] *= f;
						Aj += row;
						/* A[row*j + i] *= f; */
					}
				}
			}
			Ai += row;
		}
	}
}

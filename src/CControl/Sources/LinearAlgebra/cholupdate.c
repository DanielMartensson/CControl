/*
 * cholupdate.c
 *
 *  Created on: 15 sep. 2021
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Create L = cholupdate(L, x, rank_one_update)
 * L is a lower triangular matrix with real and positive diagonal entries from cholesky decomposition L = chol(A)
 * A [m*n]
 * x [m]
 * n == m
 */
void cholupdate(float L[], float x[], uint16_t row, bool rank_one_update) {
	uint16_t k, i;
	int8_t F = -1;
	float r, c, s, a, b;
	if (rank_one_update)
		F = 1;
	for (k = 0; k < row; k++) {
		a = L[k * row + k];
		b = x[k];
		r = sqrtf(a * a + F * b * b);
		c = r / a;
		s = b / a;
		L[k * row + k] = r;
		if (k < row) {
			for (i = k + 1; i < row; i++) {
				L[k * row + i] = (L[k * row + i] + F * s * x[i]) / c;
				x[i] = c * x[i] - s * L[k * row + i];
			}
		}
	}
}

/*
 * GNU Octave code:
 * function [L] = cholupdate2(L, x, OP)
	  cholupdate(L, x, OP) % OP can be "+" = rank_one_update or "-" = rank_one_downdate
	  F = 1;
	  if(strcmp(OP, '-') == 1)
		F = -1; % Rank_one_downdate
	  end
	  n = length(x);
	  for k = 1:n
		  a = L(k,k);
		  b = x(k);
		  r = sqrt(a*a + F * b*b);
		  c = r / a;
		  s = b / a;
		  L(k, k) = r;
		  if k < n
			  for i = k+1:n
				L(k, i) = (L(k,i) + F * s * x(i)) / c;
				x(i) = c * x(i) - s * L(k, i);
			  end
		  end
	  end
	end
 */

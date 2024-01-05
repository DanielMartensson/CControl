/*
 * linsolve_upper_triangular.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "linearalgebra.h"

/*
 * This solves Ax = b.
 * A need to be square and upper triangular.
 * A [m*n]
 * b [m]
 * m == n
 */
void linsolve_upper_triangular(float A[], float x[], float b[], size_t column) {
	/* Save address - This is the first column at the last row */
	float *A0 = &A[(column-1)*column];

	/* Time to solve x from Ax = b. */
	memset(x, 0, column*sizeof(float));
	float sum;
	int32_t i, j;
	for(i = column-1; i >= 0; i--){ /* Column */
		sum = 0.0f; /* This is our sum */
		for(j = i; j < column; j++){ /* Row */
			sum += A0[j] * x[j];
			/* sum += A[i*column + j] * x[j]; */
		}
		x[i] = (b[i] - sum) / A0[i];
		/* x[i] = (b[i] - sum) / A[i*column + i]; */
		A0 -= column;
	}

}

/*
 * GNU Octave code:
 *  function [x] = linsolve(A, b)
	  s = length(A);
	  x = zeros(s,1);
	  x(s) = b(s)/A(s,s);
	  for i = s-1:-1:1
		  sum = 0;
		  for j = s:-1:i+1
			  sum = sum + A(i,j)*x(j);
		  end
		  x(i) = (b(i)- sum)/A(i,i);
	  end
	end
 */

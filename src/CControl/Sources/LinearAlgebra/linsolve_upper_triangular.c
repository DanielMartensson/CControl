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
void linsolve_upper_triangular(const float A[], float x[], const float b[], const size_t row) {	
	/* Shift address - This is the first column at the last row */
	A += (row - 1) * row;

	/* Time to solve x from Ax = b */
	memset(x, 0, row * sizeof(float));
	int32_t i;
	for(i = row -1; i >= 0; i--){
		const float s = dot(A + i, x + i, row - i);
		x[i] = (b[i] - s) / A[i];
		A -= row;
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

/*
 * linsolve_upper_triangular.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"



/*
 * This solves Ax = b.
 * A need to be square and upper triangular.
 * A [m*n]
 * b [m]
 * m == n
 */
void linsolve_upper_triangular(float A[], float x[], float b[], uint16_t column) {

	// Time to solve x from Ax = b.
	memset(x, 0, column*sizeof(float));
	float sum;
	for(uint16_t i = column-1; i >= 0; i--){ // Column
		sum = 0.0; // This is our sum
		for(uint16_t j = i; j < column; j++){ // Row
			sum += A[i*column + j] * x[j];
		}
		x[i] = (b[i] - sum) / A[i*column + i];

		// For backwards unsigned for-loops, important because uint16 i = -1 is actually 65535
		if(i == 0)
			break;
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

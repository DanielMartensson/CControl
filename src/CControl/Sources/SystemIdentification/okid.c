/*
 * okid.c
 *
 *  Created on: 13 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "systemidentification.h"

/*
 * Observer kalman filter identification.
 * This is the basic version, e.g it won't give you the kalman gain K matrix.
 * If you need fully version, then look for MataveID at GitHub
 * First collect your inputs u and outputs y and create impulse response g, called Markov parameters.
 * Then you must use era.c algorithm to convert impulse response g into a linear state space model.
 * Data length need to be the same as the column length n!
 * u [m*n]
 * y [m*n]
 * g [m*n] Markov parameters
 */
void okid(float u[], float y[], float g[], size_t row, size_t column){

	/* This is just a simple linear solve Ax = b where A is lower toeplitz triangular shape
	 * but A is a vector of g. So the formula is y' = g*u
	 * and we want to solve g = y/u
	 * u is a vector, but it's interpreted as
	 * [u0 	0 	0 	0 	0	0	0]		[g0]	[y0]
	 * [u1	u0	0	0	0	0	0]		[g1]	[y1]
	 * [u2	u1	u0	0	0	0	0]		[g2]	[y2]
	 * [u3	u2	u1	u0	0	0	0]   *  [g3] = 	[y3]
	 * [u4	u3	u2	u1	u0	0	0]		[g4]	[y4]
	 * [u5	u4	u3	u2	u1	u0	0]		[g5]	[y5]
	 * [..	u5	u4	u3	u2	u1	u0]		[g6]	[y6]
	 * [un	..	..	..	..	..	..]		[gn]	[yn]
	 *
	 * Where g0 = y0/u0 and g1 = (y1 - u1*g0)/u0 etc..
	 */
	memset(g, 0, row*column*sizeof(float));
	float sum;
	size_t k, i, j;
	for(k = 0; k < row; k++){ /* If we have more than 1 rows = MIMO system */
		for (i = 0; i < column; i++) {
			sum = 0;
			for (j = 0; j < i; j = j + 1){
				sum = sum + u[k*column + i - j] * g[k*column + j];
			}
			g[k*column + i] = (y[k*column + i] - sum) / u[k*column + 0];
		}
	}

}

/* 
 * GNU Octave code:
 
 g = zeros(row, column);
for k = 1:row
  for i = 1:column
  % Find markov parameter
  sum = 0;
    for j = 1:i-1
      sum = sum + u(k, i-j+1) * g(k, j);
    end
    g(k, i) = (y(k, i) - sum) / u(1,1);
  end
end
*/
 

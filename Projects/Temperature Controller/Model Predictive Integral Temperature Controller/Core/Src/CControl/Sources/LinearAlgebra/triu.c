/*
 * triu.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Prepare A and b into upper triangular with triu.
 * Then you can use linsolve later
 * A [m*n] A need to be square
 * b [m]
 * n == m
 */
void triu(float* A, float* b, int row) {

	// Make A to upper triangular. Also change b as well.
	float pivot = 0.0;
	for(int j = 0; j < row; j++){ // Column
		for(int i = 0; i < row; i++){ // row
			if(i > j){
				pivot = *(A + i*row + j) / *(A + j*row + j);
				for(int k = 0; k < row; k++){
					*(A + i*row + k) = *(A + i*row + k) - pivot * *(A + j*row + k);
				}
				*(b + i) = *(b + i) - pivot * *(b + j);
			}
		}
	}
}

/*
 * GNU Octave code:
 *  function [A, b] = backsubstitution(A, b)
	  s = length(A);
	  for j = 1:(s-1)
		  for i = s:-1:j+1
			  m = A(i,j)/A(j,j);
			  A(i,:) = A(i,:) - m*A(j,:);
			  b(i) = b(i) - m*b(j);
		  end
	  end
	end
 */



/*
 * Tools.c
 *
 *  Created on: 5 oct. 2019
 *      Author: Daniel Mårtensson
 */

#include "../../CControl/Headers/Configurations.h"
#include "../../CControl/Headers/Functions.h"

/*
 * Convert SISO transfer function parameters theta vector into a
 * state space model on Observable Canonical Form.
 * Vector theta can be found from rls function
 */
void theta2ss(float* A, float* B, float* C, float* theta, float* K){
	// Clear A, B, C, K
	memset(A, 0, ADIM*ADIM*sizeof(float));
	memset(B, 0, ADIM*RDIM*sizeof(float));
	memset(C, 0, YDIM*ADIM*sizeof(float));
	memset(K, 0, ADIM*YDIM*sizeof(float));

	if(INTEGRATION == TRUE){
		// Insert A = [A 0; CA 1]
		for(int i = 0; i < NP; i++){
			*(A + i*ADIM) = -*(theta + i); // Insert on column 0 only to almost last row
		}
		for(int i = 0; i < ADIM-2; i++){
			*(A + i*ADIM + 1 + i) = 1; // Ones at the diagonal with 1 in shift to right to third last row
		}
		*(A + ADIM*ADIM -1) = 1; // This adds 1 at the bottom corner to right  [A 0; CA ->1<-]
		for(int i = 0; i < NP; i++){
			*(A + NP*ADIM + i) = *(A + i); // This is exactly the same as C*A because C = [1 0] and C*A is the first row of A
		}

		// Insert B = [B;CB]
		for(int i = 0; i < NZ; i++){
			*(B + i) = *(theta + NP + i); // Insert the B in [B; CB]
		}
		*(B + ADIM-1) = *(B + 0); // This is C*B as long C = [1 0]

		// Insert C = [0 1]
		*(C + ADIM-1 ) = 1; // Notice that transfer functions is only SISO

		// Create the kalman gain matrix K
		for(int i = 0; i < ADIM; i++){
			*(K + i) = *(theta + NP + NZ + i) - *(theta + i);
		}
	}else{
		// Insert A = [A]
		for(int i = 0; i < ADIM; i++){
			*(A + i*ADIM) = -*(theta + i); // Insert on column 0 only to last row
		}
		for(int i = 0; i < ADIM-1; i++){
			*(A + i*ADIM + 1 + i) = 1; // Ones at the diagonal with 1 in shift to right to last row
		}

		// Insert B = [B]
		for(int i = 0; i < ADIM; i++){
			*(B + i) = *(theta + NP + i); // Insert the B in [B]
		}

		// Insert C = [1 0]
		*(C + 0) = 1; // Notice that transfer functions is only SISO

		// Create the kalman gain matrix K
		for(int i = 0; i < ADIM; i++){
			*(K + i) = *(theta + NP + NZ + i) - *(theta + i);
		}

	}

}

/*
 * This will saturate the input value input
 */
void saturation(float* input, float lower_limit, float upper_limit){
	if(*input > upper_limit){
		*input = upper_limit;
	}else if(*input < lower_limit){
		*input = lower_limit;
	}// else = nothing happens to *input
}

/*
 * Cut a matrix A with size row x columns into a matrix B with size (stop_row - start_row + 1) x (stop_column - start_column + 1).
 * Remember! Indexing start and stop are from zero!
 *
 * Example:
 * If you have a matrix A 5 x 6 and you want to cut the values from A to matrix B with size 3 x 3 and you want to start at 0,0 and end at 3,3
 * Code: cut(A, 5, 6, B, 0, 2, 0, 2); // Because indexing from zero
 */
void cut(float* A, int row, int column, float* B, int start_row, int stop_row, int start_column, int stop_column) {

	int in_columns = column;
	float* data = A + start_row * in_columns + start_column;

	// Create the output
	//double* ptr = B;
	int out_columns = stop_column - start_column + 1;

	// Instead of having two for loops, we just copy the whole row at once.
	for (int i = start_row; i < stop_row + 1; i++) {
		memcpy(B, data, sizeof(float) * out_columns);
		B += out_columns;
		data += in_columns;
	}

}


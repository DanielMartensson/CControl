/*
 ============================================================================
 Name        : det.c
 Author      : Daniel Mårtensson
 Version     : 1.0
 Copyright   : MIT
 Description : Get the determinant of a square matrix
 ============================================================================
 */

#include "CControl/Headers/Functions.h"

int main() {

	// Matrix A
	float A[2*2] = {0.798231,   0.191700,
				   -0.575101,   0.031430};


	clock_t start, end;
	float cpu_time_used;
	start = clock();
	float determinant = det(A, 2);
	end = clock();
	cpu_time_used = ((float) (end - start)) / CLOCKS_PER_SEC;
	printf("\nTotal speed  was %f\n", cpu_time_used);

	// Print the determinant
	printf("Det of A = %f\n", determinant);

	return EXIT_SUCCESS;
}

/*
 * GNU Octave code:
 * A = [0.798231,   0.191700,
	   -0.575101,   0.031430];

	d = det(A)
 */

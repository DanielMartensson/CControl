/*
 ============================================================================
 Name        : quadprog.c
 Author      : Daniel M�rtensson
 Version     : 1.0
 Copyright   : MIT
 Description : Solve Ax=b with constraints by using Hildreth's method
 ============================================================================
 */

#include "CControl/Headers/Functions.h"

int main() {
	clock_t start, end;
	float cpu_time_used;
	start = clock();

	// Objective function
	float Q[2 * 2] = { 1, -1,
				   -1,  10 };

	float c[2] = { -2,
				  -6 };

	// Constraints
	float A[3 * 2] = { 1, 1,
				   -1, 2,
					2, 1 };

	float b[3] = { 2,
				  2,
				  3 };

	// Solution
	float x[2];

	/*
	 * Do quadratic programming with Hildreth's method
	 *  	Min 1/2x^TQx + c^Tx
	 * 		S.t Ax <= b
	 *      	 x >= 0
	 *
	 */
	bool solution = quadprog(Q, c, A, b, x, 3, 2);

	end = clock();
	cpu_time_used = ((float)(end - start)) / CLOCKS_PER_SEC;
	printf("\nTotal speed  was %f\n", cpu_time_used);

	printf("Solution:\n");
	print(x, 2, 1);
	printf("Solution was found: %s\n", solution == true ? "yes" : "no");

	return EXIT_SUCCESS;
}

/*
 * GNU Octave code:
     % Objective function
  Q = [1 -1; -1 10];
  c = [-2; -6];

  % Constraints
  A = [1 1; -1 2; 2 1];
  b = [2; 2; 3];

  % Hildreth's method
  x = quadprog(Q, c, A, b)

  % Internal QP-solver
  [x, ~, info] = qp([], Q, c, [], [], [], [], [], A, b)

  x =

   1.1778
   0.6444

  x =

   1.1778
   0.6444

  info =

  scalar structure containing the fields:

	solveiter = 3
	info = 0
 */

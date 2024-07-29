/*
 * cab.c
 *
 *  Created on: 28 July 2024
 *      Author: Daniel Mårtensson
 */

#include "controlengineering.h"

/*
 * Lower triangular toeplitz of extended observability matrix
 * CAB stands for C*A^i*B because every element is C*A*B
 * 
 * GAMMA[(N*row_c)*(N*column_b)]
 * PHI[(N*row_c)*row_a]
 * B[row_a*columb_b]
 * C[row_c*row_a]
 */
void cab(float GAMMA[], const float PHI[], const float B[], const float C[], const size_t row_a, const size_t row_c, const size_t column_b, const size_t N) {
	/* Decleration */
	size_t i, j;

	/* First create the initial C*A^0*B == C*I*B == C*B */
	float* CB = (float*)malloc(row_c * column_b * sizeof(float));
	mul(C, B, CB, row_c, row_a, column_b);

	/* Take the transpose of CB so it will have dimension column_b*row_c instead */
	tran(CB, row_c, column_b);

	/* Create the CAB matrix from PHI*B */
	float* PHIB = (float*)malloc(N * row_c * column_b * sizeof(float));
	mul(PHI, B, PHIB, N * row_c, row_a, column_b); /* CAB = PHI*B */
	tran(PHIB, N * row_c, column_b);

	/*
	 * We insert GAMMA = [CB PHI;
	 *                    0  CB PHI;
	 *            		  0   0  CB PHI;
	 *            		  0   0   0  CB PHI] from left to right
	 */
	memset(GAMMA, 0, N * row_c * N * column_b * sizeof(float));
	for (i = 0; i < N; i++) {
		for (j = 0; j < column_b; j++) {
			memcpy(GAMMA + N * row_c * (i * column_b + j) + row_c * i, CB + row_c * j, row_c * sizeof(float)); /* Add CB */
			memcpy(GAMMA + N * row_c * (i * column_b + j) + row_c * i + row_c, PHIB + N * row_c * j, (N - i - 1) * row_c * sizeof(float)); /* Add PHI*B */
		}
	}

	/* Transpose of gamma */
	tran(GAMMA, N * column_b, N * row_c);

	/* Free */
	free(CB);
	free(PHIB);
}

/*
GNU octave code:

function GAMMA = gammaMat(A, B, C, N)

  % Create the lower triangular toeplitz matrix
  GAMMA = [];
  for i = 1:N
	GAMMA = horzcat(GAMMA, vertcat(zeros((i-1)*size(C*A*B, 1), size(C*A*B, 2)),cabMat(A, B, C, N-i+1)));
  end

end

function CAB = cabMat(A, B, C, N)

  % Create the column for the GAMMA matrix
  CAB = [];
  for i = 0:N-1
	CAB = vertcat(CAB, C*A^i*B);
  end

end
*/
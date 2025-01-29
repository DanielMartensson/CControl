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
 * Gamma[(N*row_c)*(N*column_b)]
 * Phi[(N*row_c)*row_a]
 * B[row_a*columb_b]
 * C[row_c*row_a]
 */
void cab(float Gamma[], const float Phi[], const float B[], const float C[], const size_t row_a, const size_t row_c, const size_t column_b, const size_t N) {
	/* Decleration */
	size_t i, j;

	/* First create the initial C*A^0*B == C*I*B == C*B */
	float* CB = (float*)malloc(row_c * column_b * sizeof(float));
	mul(C, B, CB, row_c, row_a, column_b);

	/* Take the transpose of CB so it will have dimension column_b*row_c instead */
	tran(CB, row_c, column_b);

	/* Create the CAB matrix from Phi*B */
	float* PhiB = (float*)malloc(N * row_c * column_b * sizeof(float));
	mul(Phi, B, PhiB, N * row_c, row_a, column_b); /* CAB = Phi*B */
	tran(PhiB, N * row_c, column_b);

	/*
	 * We insert Gamma = [CB Phi;
	 *                    0  CB Phi;
	 *            		  0   0  CB Phi;
	 *            		  0   0   0  CB Phi] from left to right
	 */
	memset(Gamma, 0, N * row_c * N * column_b * sizeof(float));
	for (i = 0; i < N; i++) {
		for (j = 0; j < column_b; j++) {
			memcpy(Gamma + N * row_c * (i * column_b + j) + row_c * i, CB + row_c * j, row_c * sizeof(float)); /* Add CB */
			memcpy(Gamma + N * row_c * (i * column_b + j) + row_c * i + row_c, PhiB + N * row_c * j, (N - i - 1) * row_c * sizeof(float)); /* Add Phi*B */
		}
	}

	/* Transpose of gamma */
	tran(Gamma, N * column_b, N * row_c);

	/* Free */
	free(CB);
	free(PhiB);
}

/*
GNU octave code:

function Gamma = gammaMat(A, B, C, N)

  % Create the lower triangular toeplitz matrix
  Gamma = [];
  for i = 1:N
	Gamma = horzcat(Gamma, vertcat(zeros((i-1)*size(C*A*B, 1), size(C*A*B, 2)),cabMat(A, B, C, N-i+1)));
  end

end

function CAB = cabMat(A, B, C, N)

  % Create the column for the Gamma matrix
  CAB = [];
  for i = 0:N-1
	CAB = vertcat(CAB, C*A^i*B);
  end

end
*/
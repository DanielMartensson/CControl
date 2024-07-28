/*
 * ssint.c
 *
 *  Created on: 28 June 2024
 *      Author: Daniel Mårtensson
 */


#include "controlengineering.h"

/*
 * Add integration into a state space model
 * A[row_a*row_a]
 * B[row_a*column_b]
 * C[row_c*row_a]
 * Ai[(row_a + column_b) * (row_a + column_b)]
 * Bi[(row_a + column_b) * column_b]
 * Ci[row_c * (row_a + column_b)]
 */
bool ssint(const float A[], const float B[], const float C[], float Ai[], float Bi[], float Ci[], const size_t row_a, const size_t column_b, const size_t row_c) {
	/* Check if the model already have integration */
	if (!stability(A, row_a)) {
		return false;
	}

	/* Ai = [A B; zeros(column_b, row_a) eye(column_b, column_b)]; */
	size_t i;
	memset(Ai, 0, (row_a + column_b) * (row_a + column_b) * sizeof(float));
	for (i = 0; i < row_a + column_b; i++) {
		Ai[i * (row_a + column_b) + i] = 1.0f;
	}
	cat(false, A, B, Ai, row_a, row_a, row_a, column_b, row_a + column_b, row_a + column_b);
	
	/* Bi = [zeros(am, bn); eye(bn, bn)]; */
	memset(Bi, 0, (row_a + column_b) * column_b * sizeof(float));
	for (i = 0; i < column_b; i++) {
		Bi[i * column_b + i + row_a * column_b] = 1.0f;
	}

	/* C = [C zeros(cm, bn)]; */
	memset(Ci, 0, row_c * (row_a + column_b) * sizeof(float));
	insert(C, Ci, row_c, row_a, row_a + column_b, 0, 0);

	/* Return true */
	return true;
}

/*
GNU Octave code:

    % Check if the system has integration behaviour already
	abseigenvalues = abs(mc.pole(sys));
	if(max(abseigenvalues) < 1)
	  % Add integral action - It's very good and pratical!
	  % A = [A B; 0 I]
	  % B = [0; I]
	  % C = [C 0]
	  % x = [x; u(k-1)]
	  am = size(A, 1);
	  bn = size(B, 2);
	  cm = size(C, 1);
	  A = [A B; zeros(bn, am) eye(bn, bn)];
	  B = [zeros(am, bn); eye(bn, bn)];
	  C = [C zeros(cm, bn)];
	  x = [x; zeros(bn, 1)];
	end

*/
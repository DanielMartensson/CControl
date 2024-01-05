/*
 * linprog.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "optimization.h"

static bool opti(float c[], float A[], float b[], float x[], size_t row_a, size_t column_a, bool maximization);

/**
 * This is linear programming with simplex method.
 * You can do a minimization problem as well with simplex.
 * Max c^Tx
 * S.t Ax <= b
 *      x >= 0
 *
 * If you got this problem:
 * Min c^Tx
 * S.t Ax >= b
 * 		x >= 0
 *
 * Then turn this into this problem:
 * Max b^Tx
 * S.t A'x <= c
 * 		x >= 0
 *
 * In other words. Swap b and c and take transponse of A
 *
 * Call this function with the sizes
 * A [m*n] // Matrix
 * b [m] // Constraints
 * c [n] // Objective function
 * x [n] // Solution
 * m >= n // Rows need to be greater or equal as columns
 * max_or_min == 0 -> Maximization
 * max_or_min == 1 -> Minimization
 *
 * Source Simplex method: https://www.youtube.com/watch?v=yL7JByLlfrw
 * Source Simplex Dual method: https://www.youtube.com/watch?v=8_D3gkrgeK8
 */
bool linprog(float c[], float A[], float b[], float x[], size_t row_a, size_t column_a, bool maximization){

	if(maximization){
		/* Maximization */
		return opti(c, A, b, x, row_a, column_a, maximization);
	}else{
		/* Minimization */
		tran(A, row_a, column_a);
		return opti(b, A, c, x, column_a, row_a, maximization);
	}

}
/* This is Simplex method with the Dual included */
static bool opti(float c[], float A[], float b[], float x[], size_t row_a, size_t column_a, bool maximization){

	/* Clear the solution */
	if (maximization) {
		memset(x, 0, column_a * sizeof(float));
	}else {
		memset(x, 0, row_a * sizeof(float));
	}

	/* Create the tableau with space for the slack variables s and p as well */
	float *tableau = (float*)malloc((row_a + 1) * (column_a + row_a + 2) * sizeof(float)); /* +1 because the extra row for objective function and +2 for the b vector and slackvariable for objective function */
	memset(tableau, 0, (row_a+1)*(column_a+row_a+2)*sizeof(float));

	/* Load the constraints */
	size_t j = 0;
	size_t i;
	for(i = 0; i < row_a; i++){
		/* First row */
		memcpy(tableau + i*(column_a+row_a+2), A + i*column_a, column_a*sizeof(float));

		/* Slack variable s */
		j = column_a + i;
		tableau[i*(column_a+row_a+2) + j] = 1.0f;

		/* Add b vector */
		tableau[i*(column_a+row_a+2) + (column_a+row_a+2-1)] = b[i];
	}

	/* Negative objective function */
	for (i = 0; i < column_a; i++) {
		tableau[(row_a + 1 - 1) * (column_a + row_a + 2) + i] = -c[i];
	}

	/* Slack variable for the objective function */
	tableau[(row_a+1-1)*(column_a+row_a+2) + (column_a+row_a+2-2)] = 1.0f;
	/* Done! */


	/* Do row operations */
	float entry = 0.0f;
	size_t pivotColumIndex = 0;
	size_t pivotRowIndex = 0;
	float pivot = 0.0f;
	float value1 = 0.0f;
	float value2 = 0.0f;
	float value3 = 0.0f;
	float smallest = 0.0f;
	size_t count = 0; /* Iterations */
	do{
		/* Find our pivot column */
		pivotColumIndex = 0;
		entry = 0.0f;
		for(i = 0; i < (column_a+row_a+2) -1; i++){ /* -1 because we don't want to count with the last column */
			value1 = tableau[(row_a+1-1)*(column_a+row_a+2) + i]; /* Bottom row */
			if(value1 < entry){
				entry = value1;
				pivotColumIndex = i;
			}
		}
		/* If the smallest entry is equal to 0 or larger than 0, break */
		if (entry >= 0.0f) {
			break;
		}

		/* If we found no solution */
		if (count > MAX_ITERATIONS) {
			free(tableau);
			return false;
		}

		/* Find our pivot row */
		pivotRowIndex = 0;
		value1 = tableau[0*(column_a+row_a+2) + pivotColumIndex]; /* Value in pivot column */
		if (fabsf(value1) < FLT_EPSILON) {
			/* Make sure that we don't divide by zero */
			value1 = FLT_EPSILON;
		} 
		value2 = tableau[(column_a+row_a+2-1)]; /* 0*(column_a+row_a+2) + (column_a+row_a+2-1) = Value in the b vector */
		smallest = value2/value1; /* Initial smallest value */
		for(i = 1; i < row_a; i++){
			value1 = tableau[i*(column_a+row_a+2) + pivotColumIndex]; /* Value in pivot column */
			if (fabsf(value1) < FLT_EPSILON) { 
				value1 = FLT_EPSILON; 
			}
			value2 = tableau[i*(column_a+row_a+2) + (column_a+row_a+2-1)]; /* Value in the b vector */
			value3 = value2/value1;
			if( (value3 > 0.0f  && value3 < smallest ) || smallest < 0.0f ){
				smallest = value3;
				pivotRowIndex = i;
			}
		}

		/* We know where our pivot is. Turn the pivot into 1 */
		/* 1/pivot * PIVOT_ROW -> PIVOT_ROW */
		pivot = tableau[pivotRowIndex*(column_a+row_a+2) + pivotColumIndex]; /* Our pivot value */
		if (fabsf(pivot) < FLT_EPSILON) { 
			pivot = FLT_EPSILON; 
		}
		for(i = 0; i < (column_a+row_a+2); i++){
			value1 = tableau[pivotRowIndex*(column_a+row_a+2) + i]; /* Our row value at pivot row */
			tableau[pivotRowIndex*(column_a+row_a+2) + i] = value1 * 1.0f/pivot; /* When value1 = pivot, then pivot will be 1 */
		}

		/* Turn all other values in pivot column into 0. Jump over pivot row */
		/* -value1* PIVOT_ROW + ROW -> ROW */
		for(i = 0; i < row_a + 1; i++){
			if(i != pivotRowIndex){
				value1 = tableau[i*(column_a+row_a+2) + pivotColumIndex]; /* This is at pivot column */
				for(j = 0; j < (column_a+row_a+2); j++){
					value2 = tableau[pivotRowIndex*(column_a+row_a+2) + j]; /* This is at pivot row */
					value3 = tableau[i*(column_a+row_a+2) + j]; /* This is at the row we want to be 0 at pivot column */
					tableau[i*(column_a+row_a+2) + j] = -value1*value2 + value3;
				}
			}
		}
		/* Count for the iteration */
		count++;

	}while(entry < 0.0f); /* Continue if we have still negative entries */

	/* If maximization == true -> Maximization problem */
	if(maximization){
		/* Now when we have shaped our tableau. Let's find the optimal solution. Sum the columns */
		for(i = 0; i < column_a; i++){
			value1 = 0.0f; /* Reset */
			for(j = 0; j < row_a + 1; j++){
				value1 += tableau[j*(column_a+row_a+2) + i]; /* Summary */
				value2 = tableau[j*(column_a+row_a+2) + i]; /* If this is 1 then we are on the selected */

				/* Check if we have a value that are very close to 1 */
				if(value1 < 1.0f + FLT_EPSILON && value1 > 1.0f - FLT_EPSILON && value2 > 1.0f - FLT_EPSILON){
					x[i] = tableau[j*(column_a+row_a+2) + (column_a+row_a+2-1)];
				}
			}
		}
	}else{
		/* Minimization (The Dual method) - Only take the bottom rows on the slack variables */
		for(i = 0; i < row_a; i++){
			x[i] = tableau[row_a*(column_a+row_a+2) + i + column_a]; /* We take only the bottom row at start index column_a */
		}
	}

	/* Free */
	free(tableau);

	/* We found solution */
	return true;
}

/*
 * GNU Octave code:
 *  >> A = [1 2; 1 -4]
	A =

	   1   2
	   1  -4

	>> b =  [2; 5]
	b =

	   2
	   5

	>> c = A'*b
	c =

		7
	  -16

	>> x = glpk(c, A, b, [0;0], [], "UU", "CC", -1) % -1 is for maximize
	x =

	   2
	   0

	>>

 */

/* GNU Octave code:
	% This is linear programming with simplex method.
	% You can do a minimization problem as well with simplex.
	% Max c^Tx
	% S.t Ax <= b
	%      x >= 0
	%
	% If you got this problem:
	% Min c^Tx
	% S.t Ax >= b
	% 		x >= 0
	%
	% Then turn this into this problem:
	% Max b^Tx
	% S.t A'x <= c
	% 		x >= 0
	%
	% Input: c(Objective function), A(Constraint matrix), b(Constraint vector), max_or_min(Maximization = 0, Minimization = 1)
	% Output: x(Solution vector), solution(boolean flag)
	% Example 1: [x, solution] = linprog(c, A, b, max_or_min)
	% Author: Daniel Mårtensson 2022 September 3

	function [x, solution] = linprog(c, A, b, max_or_min)
	  row_a = size(A, 1);
	  column_a = size(A, 2);

	  if(max_or_min == 0)
		% Maximization
		[x, solution] = opti(c, A, b, row_a, column_a, max_or_min);
	  else
		% Minimization
		[x, solution] = opti(b, A', c, column_a, row_a, max_or_min);
	  end
	end

	function [x, solution] = opti(c, A, b, row_a, column_a, max_or_min)
	  % This simplex method has been written as it was C code
	  % Clear the solution
	  if(max_or_min == 0)
		x = zeros(column_a, 1);
	  else
		x = zeros(row_a, 1);
	  end

	  % Create the tableau
	  tableau = zeros(row_a + 1, column_a + row_a + 2);
	  j = 1;
	  for i = 1:row_a
		% First row
		tableau(i, 1:column_a) = A(i, 1:column_a);

		% Slack variable s
		j = column_a + i;
		tableau(i, j) = 1;

		% Add b vector
		tableau(i, column_a + row_a + 2) = b(i);
	  end

	  % Negative objective function
	  tableau(row_a + 1, 1:column_a) = -c(1:column_a);

	  % Slack variable for objective function
	  tableau(row_a + 1, column_a + row_a + 1) = 1;

	  % Do row operations
	  entry = -1.0; % Need to start with a negative number because MATLAB don't have do-while! ;(
	  pivotColumIndex = 0;
	  pivotRowIndex = 0;
	  pivot = 0.0;
	  value1 = 0.0;
	  value2 = 0.0;
	  value3 = 0.0;
	  smallest = 0.0;
	  count = 0;
	  solution = true;
	  while(entry < 0) % Continue if we have still negative entries
		% Find our pivot column
		pivotColumIndex = 1;
		entry = 0.0;
		for i = 1:column_a + row_a + 2
		  value1 = tableau(row_a + 1, i);
		  if(value1 < entry)
			entry = value1;
			pivotColumIndex = i;
		  end
		end

		% If the smallest entry is equal to 0 or larger than 0, break
		if(entry >= 0.0)
		  break;
		end

		% If we found no solution
		if(count > 1000)
		  solution = false;
		  break;
		end

		% Find our pivot row
		pivotRowIndex = 1;
		value1 = tableau(1, pivotColumIndex); % Value in pivot column
		if(abs(value1) < eps)
		  value1 = eps;
		end
		value2 = tableau(1, column_a+row_a+2); % Value in the b vector
		smallest = value2/value1; % Initial smalles value1
		for i = 2:row_a
		  value1 = tableau(i, pivotColumIndex); % Value in pivot column
		  if(abs(value1) < eps)
			value1 = eps;
		  end
		  value2 = tableau(i, column_a+row_a+2); % Value in the b vector
		  value3 = value2/value1;
		  if(or(and(value3 > 0, value3 < smallest), smallest < 0))
			smallest = value3;
			pivotRowIndex = i;
		  end
		end

		% We know where our pivot is. Turn the pivot into 1
		% 1/pivot * PIVOT_ROW -> PIVOT_ROW
		pivot = tableau(pivotRowIndex, pivotColumIndex); % Our pivot value
		if(abs(pivot) < eps)
		  pivot = eps;
		end
		for i = 1:column_a + row_a + 2
		  value1 = tableau(pivotRowIndex, i); % Our row value at pivot row
		  tableau(pivotRowIndex, i) = value1 * 1/pivot; % When value1 = pivot, then pivot will be 1
		end


		% Turn all other values in pivot column into 0. Jump over pivot row
		% -value1* PIVOT_ROW + ROW -> ROW
		for i = 1:row_a + 1
		  if(i ~= pivotRowIndex)
			value1 = tableau(i, pivotColumIndex); %  This is at pivot column
			for j = 1:column_a+row_a+2
			  value2 = tableau(pivotRowIndex, j); % This is at pivot row
			  value3 = tableau(i, j); % This is at the row we want to be 0 at pivot column
			  tableau(i, j) = -value1*value2 + value3;
			end
		  end
		end


		% Count for the iteration
		count = count + 1;
	  end

	  % If max_or_min == 0 -> Maximization problem
	  if(max_or_min == 0)
		% Now when we have shaped our tableau. Let's find the optimal solution. Sum the columns
		for i = 1:column_a
		  value1 = 0; % Reset
		  for j = 1:row_a + 1
			value1 = value1 + tableau(j, i); % Summary
			value2 = tableau(j, i); %  If this is 1 then we are on the selected

			% Check if we have a value that are very close to 1
			if(and(and(value1 < 1 + eps, value1 > 1 - eps), value2 > 1 - eps))
			  x(i) = tableau(j, column_a+row_a+2);
			end
		  end
		end
	  else
		% Minimization (The Dual method) - Only take the bottom rows on the slack variables
		for i = 1:row_a
		  x(i) = tableau(row_a+1, i + column_a); % We take only the bottom row at start index column_a
		end
	  end
	end
 */

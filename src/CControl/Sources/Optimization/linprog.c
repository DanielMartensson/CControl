/*
 * linprog.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

static void opti(float c[], float A[], float b[], float x[], uint8_t row_a, uint8_t column_a, uint8_t max_or_min, uint8_t iteration_limit);


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
void linprog(float c[], float A[], float b[], float x[], uint8_t row_a, uint8_t column_a, uint8_t max_or_min, uint8_t iteration_limit){

	if(max_or_min == 0){
		// Maximization
		opti(c, A, b, x, row_a, column_a, max_or_min, iteration_limit);
	}else{
		// Minimization
		tran(A, row_a, column_a);

		opti(b, A, c, x, column_a, row_a, max_or_min, iteration_limit);
	}

}
// This is Simplex method with the Dual included
static void opti(float c[], float A[], float b[], float x[], uint8_t row_a, uint8_t column_a, uint8_t max_or_min, uint8_t iteration_limit){

	// Clear the solution
	if(max_or_min == 0)
		memset(x, 0, column_a*sizeof(float));
	else
		memset(x, 0, row_a*sizeof(float));

	// Create the tableau with space for the slack variables s and p as well
	float tableau[(row_a+1)*(column_a+row_a+2)]; // +1 because the extra row for objective function and +2 for the b vector and slackvariable for objective function
	memset(tableau, 0, (row_a+1)*(column_a+row_a+2)*sizeof(float));

	// Load the constraints
	int j = 0;
	for(uint8_t i = 0; i < row_a; i++){
		// First row
		memcpy(tableau + i*(column_a+row_a+2), A + i*column_a, column_a*sizeof(float));

		// Slack variable s
		j = column_a + i;
		tableau[i*(column_a+row_a+2) + j] = 1;

		// Add b vector
		tableau[i*(column_a+row_a+2) + (column_a+row_a+2-1)] = *(b+i);
	}

	// Negative objective function
	for(int i = 0; i < column_a; i++)
		tableau[(row_a+1-1)*(column_a+row_a+2) + i] = -*(c +i);

	// Slack variable for the objective function
	tableau[(row_a+1-1)*(column_a+row_a+2) + (column_a+row_a+2-2)] = 1;
	// Done!


	// Do row operations
	float entry = 0.0;
	int pivotColumIndex = 0;
	int pivotRowIndex = 0;
	float pivot = 0.0;
	float value1 = 0.0;
	float value2 = 0.0;
	float value3 = 0.0;
	float smallest = 0.0;
	int count = 0;
	do{
		// Find our pivot column
		pivotColumIndex = 0;
		entry = 0.0;
		for(uint8_t i = 0; i < (column_a+row_a+2) -1; i++){ // -1 because we don't want to count with the last column
			value1 = tableau[(row_a+1-1)*(column_a+row_a+2) + i]; // Bottom row
			if(value1 < entry){
				entry = value1;
				pivotColumIndex = i;
			}
		}
		// If the smallest entry is equal to 0 or larger than 0, break
		if(entry >= 0.0 || count >= iteration_limit)
			break;

		// Find our pivot row
		pivotRowIndex = 0;
		value1 = *(tableau + 0*(column_a+row_a+2) + pivotColumIndex); // Value in pivot column
		if(value1 == 0) value1 = FLT_EPSILON; // Make sure that we don't divide by zero
		value2 = tableau[0*(column_a+row_a+2) + (column_a+row_a+2-1)]; // Value in the b vector
		smallest = value2/value1; // Initial smallest value
		for(int i = 1; i < row_a; i++){
			value1 = tableau[i*(column_a+row_a+2) + pivotColumIndex]; // Value in pivot column
			if(value1 == 0) value1 = FLT_EPSILON;
			value2 = tableau[i*(column_a+row_a+2) + (column_a+row_a+2-1)]; // Value in the b vector
			value3 = value2/value1;
			if( (value3 > 0  && value3 < smallest ) || smallest < 0 ){
				smallest = value3;
				pivotRowIndex = i;
			}
		}

		// We know where our pivot is. Turn the pivot into 1
		// 1/pivot * PIVOT_ROW -> PIVOT_ROW
		pivot = tableau[pivotRowIndex*(column_a+row_a+2) + pivotColumIndex]; // Our pivot value
		if(pivot == 0) pivot = FLT_EPSILON;
		for(uint8_t i = 0; i < (column_a+row_a+2); i++){
			value1 = tableau[pivotRowIndex*(column_a+row_a+2) + i]; // Our row value at pivot row
			tableau[pivotRowIndex*(column_a+row_a+2) + i] = value1 * 1/pivot; // When value1 = pivot, then pivot will be 1
		}

		// Turn all other values in pivot column into 0. Jump over pivot row
		// -value1* PIVOT_ROW + ROW -> ROW
		for(int i = 0; i < row_a + 1; i++){
			if(i != pivotRowIndex){
				value1 = tableau[i*(column_a+row_a+2) + pivotColumIndex]; // This is at pivot column
				for(int j = 0; j < (column_a+row_a+2); j++){
					value2 = tableau[pivotRowIndex*(column_a+row_a+2) + j]; // This is at pivot row
					value3 = tableau[i*(column_a+row_a+2) + j]; // This is at the row we want to be 0 at pivot column
					tableau[i*(column_a+row_a+2) + j] = -value1*value2 + value3;
				}
			}
		}
		// Count for the iteration
		count++;

	}while(entry < 0); // Continue if we have still negative entries

	// If max_or_min == 0 -> Maximization problem
	if(max_or_min == 0){
		// Now when we have shaped our tableau. Let's find the optimal solution. Sum the columns
		for(uint8_t i = 0; i < column_a; i++){
			value1 = 0; // Reset
			for(uint8_t j = 0; j < row_a + 1; j++){
				value1 += tableau[j*(column_a+row_a+2) + i]; // Summary
				value2 = tableau[j*(column_a+row_a+2) + i]; // If this is 1 then we are on the selected

				// Check if we have a value that are very close to 1
				if(value1 < 1 + FLT_EPSILON && value1 > 1 - FLT_EPSILON && value2 > 1 - FLT_EPSILON){
					x[i] = tableau[j*(column_a+row_a+2) + (column_a+row_a+2-1)];
				}
			}
		}
	}else{
		// Minimization (The Dual method) - Only take the bottom rows on the slack variables
		for(uint8_t i = 0; i < row_a; i++){
			x[i] = tableau[row_a*(column_a+row_a+2) + i + column_a]; // We take only the bottom row at start index column_a
		}
	}
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

/* More Octave code:

% This simplex method has been written as it was C code
function [x] = linprog2(c, A, b, max_or_min, iteration_limit)
  row_a = size(A, 1);
  column_a = size(A, 2);

  if(max_or_min == 0)
    % Maximization
    x = opti(c, A, b, row_a, column_a, max_or_min, iteration_limit);
  else
    % Minimization
    x = opti(b, A', c, column_a, row_a, max_or_min, iteration_limit);
  end
end


function [x] = opti(c, A, b, row_a, column_a, max_or_min, iteration_limit)

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
    if(or(entry >= 0.0, count >= iteration_limit))
      break;
    end

    % Find our pivot row
    pivotRowIndex = 1;
    value1 = tableau(1, pivotColumIndex); % Value in pivot column
    value2 = tableau(1, column_a+row_a+2); % Value in the b vector
    smallest = value2/value1; % Initial smalles value1
    for i = 2:row_a
      value1 = tableau(i, pivotColumIndex); % Value in pivot column
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

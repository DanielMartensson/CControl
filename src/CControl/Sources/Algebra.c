/*
 * Algebra.c
 *
 *  Created on: 5 oct. 2019
 *      Author: Daniel Mårtensson
 */

#include "../../CControl/Headers/Functions.h"
#include "../../CControl/Headers/Configurations.h"

// Private functions
static void Householders_Reduction_to_Bidiagonal_Form(float *A, int nrows, int ncols, float *U, float *V, float *diagonal, float *superdiagonal);
static int Givens_Reduction_to_Diagonal_Form(int nrows, int ncols, float *U, float *V, float *diagonal, float *superdiagonal);
static void Sort_by_Decreasing_Singular_Values(int nrows, int ncols, float *singular_value, float *U, float *V);

/*
 * Turn A into transponse A^T
 */
void tran(float* A, int row, int column) {

	float B[row*column];
	float* transpose;
	float* ptr_A = A;

	for (int i = 0; i < row; i++) {
		transpose = &B[i];
		for (int j = 0; j < column; j++) {
			*transpose = *ptr_A;
			ptr_A++;
			transpose += row;
		}
	}

	// Copy!
	memcpy(A, B, row*column*sizeof(float));

}

/*
 * C = A*B
 */
void mul(float* A, float* B, float* C, int row_a, int column_a, int column_b) {

	// Data matrix
	float* data_a;
	float* data_b;

	for (int i = 0; i < row_a; i++) {
		// Then we go through every column of b
		for (int j = 0; j < column_b; j++) {
			data_a = &A[i * column_a];
			data_b = &B[j];

			*C = 0; // Reset
			// And we multiply rows from a with columns of b
			for (int k = 0; k < column_a; k++) {
				*C += *data_a * *data_b;
				data_a++;
				data_b += column_b;
			}
			C++; // ;)
		}
	}
}

/*
 * This solves x from (A^T*A + ALPHA*I)*x = A^T*b
 * In other words. This is Tikhonov regularization solved by Guassian Elimination for solving Ax = b
 */
void linsolve(float* A, float* x, float* b, int row_a, int column_a) {


	// AT - Transpose A
	float AT[column_a*row_a]; // Same dimension as A, just swapped rows and column
	memcpy(AT, A, column_a*row_a*sizeof(float)); // Copy A -> AT
	tran(AT, row_a, column_a); // Now turn the values of AT to transpose

	// ATb = AT*b
	float ATb[row_a];
	memset(ATb, 0, row_a * sizeof(float));
	mul(AT, b, ATb, column_a, row_a, 1);

	// ATA = AT*A
	float ATA[column_a*column_a];
	memset(ATA, 0, column_a*column_a * sizeof(float));
	mul(AT, A, ATA, column_a, row_a, column_a);

	// ATA = ATA + alpha*I.
	for(int i = 0; i < column_a; i++){
		*(ATA + i*column_a + i) = *(ATA + i*column_a + i) + ALPHA; // Don't need identity matrix here because we only add on diagonal
	}

	// Make ATA to upper triangular
	float pivot = 0.0;
	for(int j = 0; j < column_a; j++){ // Column
		for(int i = 0; i < column_a; i++){ // row
			if(i > j){
				pivot = *(ATA + i*column_a + j) / *(ATA + j*column_a + j);
				for(int k = 0; k < column_a; k++){
					*(ATA + i*column_a + k) = *(ATA + i*column_a + k) - pivot * *(ATA + j*column_a + k);
				}
				*(ATb + i) = *(ATb + i) - pivot * *(ATb + j);
			}
		}
	}

	// Time to solve x from ATAx = ATb.
	memset(x, 0, column_a * sizeof(float)); // Reset
	for(int i = column_a-1; i >= 0; i--){ // Column
		pivot = 0.0; // This is our sum
		for(int j = i; j < column_a; j++){ // Row
			pivot += *(ATA + i*column_a + j)* * (x + j);
		}
		*(x + i) = (*(ATb + i) - pivot) / *(ATA + i*column_a + i);
	}

}

/*
 * A to A^(-1) - Notice that only square matrices are allowed
 */
void inv(float* A, int row){

	// Create iA matrix - The inverse
	float iA[row*row];
	memset(iA, 0, row*row*sizeof(float));

	// Create identity vector and solve Ax=I_i for every i
	float I[row];
	memset(I, 0, row*sizeof(float));
	for(int i = 0; i < row; i++){
		*(I + i) = 1;
		linsolve(A, I, I, row, row);
		for(int j = 0; j < row; j++)
			*(iA + j*row + i) = *(I + j);
		memset(I, 0, row*sizeof(float));
	}

	// Copy over iA -> A
	memcpy(A, iA, row*row*sizeof(float));

}

/**
 * This is linear programming with simplex method:
 * Max c^Tx
 * S.t Ax <= b
 *      x >= 0
 *
 * Call this function with the sizes
 * A [m*n] // Matrix
 * b [m] // Constraints
 * c [n] // Objective function
 * x [n] // Solution
 * row_a = m
 * column_a = n
 *
 * This is used for Model Predictive Control.
 * The key idea is to find the U value from Y = PHI*X + GAMMA*U where U >= 0 and GAMMA*U <= Y - PHI*X.
 * It practice it means that your input signal cannot be negative and the input signal will result so your
 * output (e.g temperature, position, pressure, velocity etc) will not be larger than your reference vector
 */
void linprog(float* c, float* A, float* b, float* x, int row_a, int column_a, int iteration_limit){

	// Clear the solution
	memset(x, 0, column_a*sizeof(float));

	// Create the tableau with space for the slack variables s and p as well
	float tableau[(row_a+1)*(column_a+row_a+2)]; // +1 because the extra row for objective function and +2 for the b vector and slackvariable for objective function
	memset(tableau, 0, (row_a+1)*(column_a+row_a+2)*sizeof(float));

	// Load the constraints
	int j = 0;
	for(int i = 0; i < row_a; i++){
		// First row
		memcpy(tableau + i*(column_a+row_a+2), A + i*column_a, column_a*sizeof(float));

		// Slack variable s
		j = column_a + i;
		tableau[i*(column_a+row_a+2) + j] = 1;

		// Add b vector
		tableau[i*(column_a+row_a+2) + (column_a+row_a+2-1)] = *(b+i);
	}

	// Negative objective function
	for(int i = 0; i < column_a; i++){
		tableau[(row_a+1-1)*(column_a+row_a+2) + i] = -*(c +i);
	}
	// Slack variable for the objective function
	tableau[(row_a+1-1)*(column_a+row_a+2) + (column_a+row_a+2-2)] = 1;
	// Done!

	// Print tableau
	//print(tableau,(row_a+1),(column_a+row_a+2));

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
		for(int i = 0; i < (column_a+row_a+2) -1; i++){ // -1 because we don't want to count with the last column
			value1 = *(tableau + (row_a+1-1)*(column_a+row_a+2) + i); // Bottom row
			if(value1 < entry){
				entry = value1;
				pivotColumIndex = i;
			}
		}
		//printf("Entry = %f\n", entry);
		// If the smallest entry is equal to 0 or larger than 0, break
		if(entry >= 0.0 || count >= iteration_limit)
			break;

		// Find our pivot row
		pivotRowIndex = 0;
		value1 = *(tableau + 0*(column_a+row_a+2) + pivotColumIndex); // Value in pivot column
		value2 = *(tableau + 0*(column_a+row_a+2) + (column_a+row_a+2-1)); // Value in the b vector
		smallest = value2/value1; // Initial smallest value
		for(int i = 1; i < row_a; i++){
			value1 = *(tableau + i*(column_a+row_a+2) + pivotColumIndex); // Value in pivot column
			value2 = *(tableau + i*(column_a+row_a+2) + (column_a+row_a+2-1)); // Value in the b vector
			value3 = value2/value1;
			if( (value3 > 0  && value3 < smallest ) || smallest < 0 ){
				smallest = value3;
				pivotRowIndex = i;
			}
		}

		// We know where our pivot is. Turn the pivot into 1
		// 1/pivot * PIVOT_ROW -> PIVOT_ROW
		pivot = *(tableau + pivotRowIndex*(column_a+row_a+2) + pivotColumIndex); // Our pivot value
		//printf("pivotRowIndex = %i, pivotColumIndex = %i, pivot = %f\n", pivotRowIndex, pivotColumIndex, pivot);
		for(int i = 0; i < (column_a+row_a+2); i++){
			value1 = *(tableau + pivotRowIndex*(column_a+row_a+2) + i); // Our row value at pivot row
			*(tableau + pivotRowIndex*(column_a+row_a+2) + i) = value1 * 1/pivot; // When value1 = pivot, then pivot will be 1
		}
		//printf("Set to 1\n");
		//print(tableau,(row_a+1),(column_a+row_a+2));

		// Turn all other values in pivot column into 0. Jump over pivot row
		// -value1* PIVOT_ROW + ROW -> ROW
		for(int i = 0; i < row_a + 1; i++){
			if(i != pivotRowIndex){
				value1 = *(tableau + i*(column_a+row_a+2) + pivotColumIndex); // This is at pivot column
				for(int j = 0; j < (column_a+row_a+2); j++){
					value2 = *(tableau + pivotRowIndex*(column_a+row_a+2) + j); // This is at pivot row
					value3 = *(tableau + i*(column_a+row_a+2) + j); // This is at the row we want to be 0 at pivot column
					*(tableau + i*(column_a+row_a+2) + j) = -value1*value2 + value3;
				}
			}
		}
		//printf("Set to 0\n");
		//print(tableau,(row_a+1),(column_a+row_a+2));

		// Count for the iteration
		count++;

	}while(entry < 0); // Continue if we have still negative entries

	// Now when we have shaped our tableau. Let's find the optimal solution. Sum the columns
	for(int i = 0; i < column_a; i++){
		value1 = 0; // Reset
		for(int j = 0; j < row_a + 1; j++){
			value1 += *(tableau + j*(column_a+row_a+2) + i); // Summary
			value2 = *(tableau + j*(column_a+row_a+2) + i); // If this is 1 then we are on the selected

			// Check if we have a value that are very close to 1
			if(value1 < 1.00001 && value1 > 0.99999 && value2 > 0.99999){
				*(x + i) = *(tableau + j*(column_a+row_a+2) + (column_a+row_a+2-1));
			}
		}
	}
	
	//print(tableau,(row_a+1),(column_a+row_a+2));
}

/*
 * This is Singular Value Decomposition A = USV^T
 * This uses Jacobi rotation method. A need to be square.
 * http://www.netlib.org/lapack/lawnspdf/lawn15.pdf
 * A [m*m]
 * U [m*m]
 * S [m]
 * V [m*m]
 */
void svd_jacobi_one_sided(float *A, int row_a, float *U, float *S, float *V) {
	// i and j are the indices of the point we've chosen to zero out
	float al, b, c, l, t, cs, sn, tmp, sign;
	int i, j, p, k;

	// Create the identity matrix
	memset(U, 0, row_a*row_a*sizeof(float));
	memset(V, 0, row_a*row_a*sizeof(float));
	memset(S, 0, row_a*sizeof(float));
	for(i = 0; i < row_a; i++){
		*(U + row_a*i + i) = 1;
		*(V + row_a*i + i) = 1;
	}

	// Apply MAX_ITERATION_COUNT_SVD times. That should be good enough
	for (p = 0; p < MAX_ITERATION_COUNT_SVD; p++) {
		// For all pairs i < j
		for (i = 0; i < row_a; i++) {
			for (j = i + 1; j < row_a; j++) {
				al = b = c = l = t = cs = sn = tmp = sign = 0.0;
				// Find the 2x2 submatrix
				for (k = 0; k < row_a; k++) {
					al += *(A + row_a*k + i) * *(A + row_a*k + i);
					b += *(A + row_a*k + j) * *(A + row_a*k + j);
					c += *(A + row_a*k + i) * *(A + row_a*k + j);
				}

				// Compute Jacobi rotation
				l = (b - al) / (2.0 * c);
				sign = 1.0;
				if (l < 0.0)
					sign = -1.0;
				t = sign / ((sign * l) + sqrtf(1.0 + l * l));
				cs = 1.0 / sqrtf(1.0 + t * t);
				sn = cs * t;

				// Change columns i and j only
				for (k = 0; k < row_a; k++) {
					tmp = *(A + row_a*k + i);
					*(A + row_a*k + i) = cs * tmp - sn * *(A + row_a*k + j);
					*(A + row_a*k + j) = sn * tmp + cs * *(A + row_a*k + j);
				}

				// Update the right singular vectors
				for (k = 0; k < row_a; k++) {
					tmp = *(V + row_a*k + i);
					*(V + row_a*k + i) = cs * tmp - sn * *(V + row_a*k + j);
					*(V + row_a*k + j) = sn * tmp + cs * *(V + row_a*k + j);
				}

			}
		}
	}

	// Find the singular values by adding up squares of each column, then taking square root of each column
	for (j = 0; j < row_a; j++) {
		for (i = 0; i < row_a; i++) {
			*(S + j) += *(A + row_a*i + j) * *(A + row_a*i + j);
		}
		tmp = *(S + j);
		*(S + j) = sqrtf(tmp);
	}

	// Sort the singular values largest to smallest, and the right matrix accordingly
	for (p = 0; p < (row_a - 1); p++) {
		for (j = 0; j < row_a - p - 1; j++) {
			if (*(S + j) < *(S + j + 1)) {
				tmp = *(S + j);
				*(S + j) = *(S + j + 1);
				*(S + j + 1) = tmp;

				// Rearrange columns of u and v accordingly
				for (i = 0; i < row_a; i++) {
					tmp = *(V + row_a*i + j);
					*(V + row_a*i + j) = *(V + row_a*i + j + 1);
					*(V + row_a*i + j + 1) = tmp;
					tmp = *(A + row_a*i + j);
					*(A + row_a*i + j) = *(A + row_a*i + j + 1);
					*(A + row_a*i + j + 1) = tmp;
				}
			}
		}
	}

	// A is A*V, so in order to get U, we divide by S in each column
	for (i = 0; i < row_a; i++) {
		for (j = 0; j < row_a; j++) {
			*(A + row_a*i + j) = *(A + row_a*i + j) / *(S + j);
		}
	}

	// Set U to A, since we have been making modifications to A
	memcpy(U, A, row_a*row_a*sizeof(float));
}

/*
 * Discrete Lyapunov equation
 * Solves A * P * A' - P + Q = 0
 * A, P, Q need to be square and Q need to be positive and symmetric
 */
void dlyap(float* A, float* P, float* Q, int row_a){
	// Create an zero large matrix M
	float M[row_a*row_a*row_a*row_a]; // row_a^2 * row_a^2

	// Create a temporary B matrix
	float B[row_a*row_a];

	// Fill the M matrix
	for(int k = 0; k < row_a; k++){
		for(int l = 0; l < row_a; l++){
			memcpy(B, A, row_a*row_a*sizeof(float)); // B = A*A(k, l);
			for(int i = 0; i < row_a*row_a; i++) *(B + i) *= *(A + row_a*k + l);
			insert(B, M, row_a, row_a, row_a*row_a, row_a*k, row_a*l);
		}
	}

	// Turn M negative but add +1 on diagonals
	for(int i = 0; i < row_a*row_a; i++)
		for(int j = 0; j < row_a*row_a; j++)
			if(i == j)
				*(M + row_a*row_a*i + j) = - *(M + row_a*row_a*i + j) + 1;
			else
				*(M + row_a*row_a*i + j) = - *(M + row_a*row_a*i + j);

	// Solve MP = Q, where P is our solution
	linsolve(M, P, Q, row_a*row_a, row_a*row_a);  // TODO: Använd QR-decomposition istället för Tichonov regularization.
}


/*
 *
 *     This is Singular Value Decomposition A = USV^T.
 *     This routine decomposes an m x n matrix A, with m >= n, into a product
 * 	   of the three matrices U, D, and V', i.e. A = UDV', where U is an m x n
 *     matrix whose columns are orthogonal, D is a n x n diagonal matrix, and
 *     V is an n x n orthogonal matrix.  V' denotes the transpose of V.  If
 *     m < n, then the procedure may be used for the matrix A'.  The singular
 *     values of A are the diagonal elements of the diagonal matrix D and
 *     correspond to the positive square roots of the eigenvalues of the
 *     matrix A'A.
 *
 *     This procedure programmed here is based on the method of Golub and
 *     Reinsch as given on pages 134 - 151 of the "Handbook for Automatic
 *     Computation vol II - Linear Algebra" edited by Wilkinson and Reinsch
 *     and published by Springer-Verlag, 1971.
 *
 *     The Golub and Reinsch's method for decomposing the matrix A into the
 *     product U, D, and V' is performed in three stages:
 *       Stage 1:  Decompose A into the product of three matrices U1, B, V1'
 *         A = U1 B V1' where B is a bidiagonal matrix, and U1, and V1 are a
 *         product of Householder transformations.
 *       Stage 2:  Use Given' transformations to reduce the bidiagonal matrix
 *         B into the product of the three matrices U2, D, V2'.  The singular
 *         value decomposition is then UDV'where U = U2 U1 and V' = V1' V2'.
 *       Stage 3:  Sort the matrix D in decreasing order of the singular
 *         values and interchange the columns of both U and V to reflect any
 *         change in the order of the singular values.
 * A [m*n]
 * U [m*n]
 * S [m]
 * V [n*n]
 * m = row
 * n = column
 * The pros with this function is that m >= n.
 * If m < n, then take the transpose of A to A^T before
 * Return 0 = Success.
 * Return -1 = Fail.
 */
int svd_golub_reinsch(float *A, int nrows, int ncols, float *U, float *singular_values, float *V) {
	float dummy_array[ncols];

	Householders_Reduction_to_Bidiagonal_Form(A, nrows, ncols, U, V, singular_values, dummy_array);

	if (Givens_Reduction_to_Diagonal_Form(nrows, ncols, U, V, singular_values, dummy_array) < 0)
		return -1;

	Sort_by_Decreasing_Singular_Values(nrows, ncols, singular_values, U, V);

	return 0;
}
                                                                      //
static void Householders_Reduction_to_Bidiagonal_Form(float *A, int nrows, int ncols, float *U, float *V, float *diagonal, float *superdiagonal) {
	int i, j, k, ip1;
	float s, s2, si, scale;
	float *pu, *pui, *pv, *pvi;
	float half_norm_squared;

	// Copy A to U
	memcpy(U, A, sizeof(float) * nrows * ncols);

	diagonal[0] = 0.0;
	s = 0.0;
	scale = 0.0;
	for (i = 0, pui = U, ip1 = 1; i < ncols; pui += ncols, i++, ip1++) {
		superdiagonal[i] = scale * s;
		//
		//       Perform Householder transform on columns.
		//
		//       Calculate the normed squared of the i-th column vector starting at
		//       row i.
		//
		for (j = i, pu = pui, scale = 0.0; j < nrows; j++, pu += ncols)
			scale += fabsf(*(pu + i));

		if (scale > 0.0) {
			for (j = i, pu = pui, s2 = 0.0; j < nrows; j++, pu += ncols) {
				*(pu + i) /= scale;
				s2 += *(pu + i) * *(pu + i);
			}
			//
			//
			//       Chose sign of s which maximizes the norm
			//
			s = (*(pui + i) < 0.0) ? sqrtf(s2) : -sqrtf(s2);
			//
			//       Calculate -2/u'u
			//
			half_norm_squared = *(pui + i) * s - s2;
			//
			//       Transform remaining columns by the Householder transform.
			//
			*(pui + i) -= s;

			for (j = ip1; j < ncols; j++) {
				for (k = i, si = 0.0, pu = pui; k < nrows; k++, pu += ncols)
					si += *(pu + i) * *(pu + j);
				si /= half_norm_squared;
				for (k = i, pu = pui; k < nrows; k++, pu += ncols) {
					*(pu + j) += si * *(pu + i);
				}
			}
		}
		for (j = i, pu = pui; j < nrows; j++, pu += ncols)
			*(pu + i) *= scale;
		diagonal[i] = s * scale;
		//
		//       Perform Householder transform on rows.
		//
		//       Calculate the normed squared of the i-th row vector starting at
		//       column i.
		//
		s = 0.0;
		scale = 0.0;
		if (i >= nrows || i == (ncols - 1))
			continue;
		for (j = ip1; j < ncols; j++)
			scale += fabsf(*(pui + j));
		if (scale > 0.0) {
			for (j = ip1, s2 = 0.0; j < ncols; j++) {
				*(pui + j) /= scale;
				s2 += *(pui + j) * *(pui + j);
			}
			s = (*(pui + ip1) < 0.0) ? sqrtf(s2) : -sqrtf(s2);
			//
			//       Calculate -2/u'u
			//
			half_norm_squared = *(pui + ip1) * s - s2;
			//
			//       Transform the rows by the Householder transform.
			//
			*(pui + ip1) -= s;
			for (k = ip1; k < ncols; k++)
				superdiagonal[k] = *(pui + k) / half_norm_squared;
			if (i < (nrows - 1)) {
				for (j = ip1, pu = pui + ncols; j < nrows; j++, pu += ncols) {
					for (k = ip1, si = 0.0; k < ncols; k++)
						si += *(pui + k) * *(pu + k);
					for (k = ip1; k < ncols; k++) {
						*(pu + k) += si * superdiagonal[k];
					}
				}
			}
			for (k = ip1; k < ncols; k++)
				*(pui + k) *= scale;
		}
	}

	// Update V
	pui = U + ncols * (ncols - 2);
	pvi = V + ncols * (ncols - 1);
	*(pvi + ncols - 1) = 1.0;
	s = superdiagonal[ncols - 1];
	pvi -= ncols;
	for (i = ncols - 2, ip1 = ncols - 1; i >= 0; i--, pui -= ncols, pvi -= ncols, ip1--) {
		if (s != 0.0) {
			pv = pvi + ncols;
			for (j = ip1; j < ncols; j++, pv += ncols)
				*(pv + i) = (*(pui + j) / *(pui + ip1)) / s;
			for (j = ip1; j < ncols; j++) {
				si = 0.0;
				for (k = ip1, pv = pvi + ncols; k < ncols; k++, pv += ncols)
					si += *(pui + k) * *(pv + j);
				for (k = ip1, pv = pvi + ncols; k < ncols; k++, pv += ncols)
					*(pv + j) += si * *(pv + i);
			}
		}
		pv = pvi + ncols;
		for (j = ip1; j < ncols; j++, pv += ncols) {
			*(pvi + j) = 0.0;
			*(pv + i) = 0.0;
		}
		*(pvi + i) = 1.0;
		s = superdiagonal[i];
	}

	// Update U

	pui = U + ncols * (ncols - 1);
	for (i = ncols - 1, ip1 = ncols; i >= 0; ip1 = i, i--, pui -= ncols) {
		s = diagonal[i];
		for (j = ip1; j < ncols; j++)
			*(pui + j) = 0.0;
		if (s != 0.0) {
			for (j = ip1; j < ncols; j++) {
				si = 0.0;
				pu = pui + ncols;
				for (k = ip1; k < nrows; k++, pu += ncols)
					si += *(pu + i) * *(pu + j);
				si = (si / *(pui + i)) / s;
				for (k = i, pu = pui; k < nrows; k++, pu += ncols)
					*(pu + j) += si * *(pu + i);
			}
			for (j = i, pu = pui; j < nrows; j++, pu += ncols) {
				*(pu + i) /= s;
			}
		} else
			for (j = i, pu = pui; j < nrows; j++, pu += ncols)
				*(pu + i) = 0.0;
		*(pui + i) += 1.0;
	}
}
                                                                       //
static int Givens_Reduction_to_Diagonal_Form(int nrows, int ncols, float *U, float *V, float *diagonal, float *superdiagonal) {

	float epsilon;
	float c, s;
	float f, g, h;
	float x, y, z;
	float *pu, *pv;
	int i, j, k, m;
	int rotation_test;
	int iteration_count;

	for (i = 0, x = 0.0; i < ncols; i++) {
		y = fabsf(diagonal[i]) + fabsf(superdiagonal[i]);
		if (x < y)
			x = y;
	}
	epsilon = x * FLT_EPSILON;
	for (k = ncols - 1; k >= 0; k--) {
		iteration_count = 0;
		while (1) {
			rotation_test = 1;
			for (m = k; m >= 0; m--) {
				if (fabsf(superdiagonal[m]) <= epsilon) {
					rotation_test = 0;
					break;
				}
				if (fabsf(diagonal[m - 1]) <= epsilon)
					break;
			}
			if (rotation_test) {
				c = 0.0;
				s = 1.0;
				for (i = m; i <= k; i++) {
					f = s * superdiagonal[i];
					superdiagonal[i] *= c;
					if (fabsf(f) <= epsilon)
						break;
					g = diagonal[i];
					h = sqrtf(f * f + g * g);
					diagonal[i] = h;
					c = g / h;
					s = -f / h;
					for (j = 0, pu = U; j < nrows; j++, pu += ncols) {
						y = *(pu + m - 1);
						z = *(pu + i);
						*(pu + m - 1) = y * c + z * s;
						*(pu + i) = -y * s + z * c;
					}
				}
			}
			z = diagonal[k];
			if (m == k) {
				if (z < 0.0) {
					diagonal[k] = -z;
					for (j = 0, pv = V; j < ncols; j++, pv += ncols)
						*(pv + k) = -*(pv + k);
				}
				break;
			} else {
				if (iteration_count >= MAX_ITERATION_COUNT_SVD)
					return -1;
				iteration_count++;
				x = diagonal[m];
				y = diagonal[k - 1];
				g = superdiagonal[k - 1];
				h = superdiagonal[k];
				f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);
				g = sqrtf(f * f + 1.0);
				if (f < 0.0)
					g = -g;
				f = ((x - z) * (x + z) + h * (y / (f + g) - h)) / x;
				// Next QR Transformtion
				c = 1.0;
				s = 1.0;
				for (i = m + 1; i <= k; i++) {
					g = superdiagonal[i];
					y = diagonal[i];
					h = s * g;
					g *= c;
					z = sqrtf(f * f + h * h);
					superdiagonal[i - 1] = z;
					c = f / z;
					s = h / z;
					f = x * c + g * s;
					g = -x * s + g * c;
					h = y * s;
					y *= c;
					for (j = 0, pv = V; j < ncols; j++, pv += ncols) {
						x = *(pv + i - 1);
						z = *(pv + i);
						*(pv + i - 1) = x * c + z * s;
						*(pv + i) = -x * s + z * c;
					}
					z = sqrtf(f * f + h * h);
					diagonal[i - 1] = z;
					if (z != 0.0) {
						c = f / z;
						s = h / z;
					}
					f = c * g + s * y;
					x = -s * g + c * y;
					for (j = 0, pu = U; j < nrows; j++, pu += ncols) {
						y = *(pu + i - 1);
						z = *(pu + i);
						*(pu + i - 1) = c * y + s * z;
						*(pu + i) = -s * y + c * z;
					}
				}
				superdiagonal[m] = 0.0;
				superdiagonal[k] = f;
				diagonal[k] = x;
			}
		}
	}
	return 0;
}

static void Sort_by_Decreasing_Singular_Values(int nrows, int ncols, float *singular_values, float *U, float *V) {
	int i, j, max_index;
	float temp;
	float *p1, *p2;

	for (i = 0; i < ncols - 1; i++) {
		max_index = i;
		for (j = i + 1; j < ncols; j++)
			if (singular_values[j] > singular_values[max_index])
				max_index = j;
		if (max_index == i)
			continue;
		temp = singular_values[i];
		singular_values[i] = singular_values[max_index];
		singular_values[max_index] = temp;
		p1 = U + max_index;
		p2 = U + i;
		for (j = 0; j < nrows; j++, p1 += ncols, p2 += ncols) {
			temp = *p1;
			*p1 = *p2;
			*p2 = temp;
		}
		p1 = V + max_index;
		p2 = V + i;
		for (j = 0; j < ncols; j++, p1 += ncols, p2 += ncols) {
			temp = *p1;
			*p1 = *p2;
			*p2 = temp;
		}
	}
}

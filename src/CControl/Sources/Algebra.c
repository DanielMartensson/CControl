/*
 * Algebra.c
 *
 *  Created on: 5 oct. 2019
 *      Author: Daniel Mårtensson
 */

#include "../../CControl/Headers/Functions.h"
#include "../../CControl/Headers/Configurations.h"

/*
 * Transpose of A
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
	float* data_a = A;
	float* data_b = B;

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

/**
 * This is a special case of mul for Model Predictive control - Double
 */
void dmul(double* A, double* B, double* C, int row_a, int column_a, int column_b) {

	// Data matrix
	double* data_a = A;
	double* data_b = B;

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
 * This solves x from Ax = b
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

	// A = ATA + alpha*I. NOTICE: We call A for ATA just to save memory
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

	/*
	 * Time to solve x from ATAx = ATb.
	 */
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

/*
 * Print matrix or vector - Just for error check
 */
void print(float* A, int row, int column) {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			printf("%0.18f ", *(A++));
		}
		printf("\n");
	}
	printf("\n");
}


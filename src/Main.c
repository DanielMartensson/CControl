/*
 ============================================================================
 Name        : main.c
 Author      : Daniel Mårtensson
 Version     : 1.0
 Copyright   : MIT
 Description : Simple test
 ============================================================================
 */

#include "CControl/ccontrol.h"

 /* Constants */
#define row_A 2
#define column_B 2

/*
 * Compute R'*Y*U = I
 * R[row_r * column_r]
 * Y[row_y * column_y]
 * U[row_u * column_u]
 * I[column_r * column_u]
 */
static void RTYUI(const float R[], const float Y[], const float U[], float I[], const size_t row_r, const size_t column_r, const size_t row_y, const size_t column_y, const size_t row_u, const size_t column_u) {
	/* Turn R into transpose */
	float* RT = (float*)malloc(row_r * column_r * sizeof(float));
	memcpy(RT, R, row_r * column_r * sizeof(float));
	tran(RT, row_r, column_r);

	/* Compute YU = Y*U */
	float* YU = (float*)malloc(row_y * column_u * sizeof(float));
	mul(Y, U, YU, row_y, column_y, column_u);

	/* Compute I = RT*YU */
	mul(RT, YU, I, column_r, row_r, column_u);

	/* Free */
	free(RT);
	free(YU);
}

/* Discrete Algebraic Riccati Equation
 * Iterate: X = A'*X*A - X - A'*X*B*(B'*X*B + R)^{-1}*B'*X*A + Q
 */
static void odefun(const float t, float X[], const float* matrices[], const size_t rows[], const size_t columns[]) {
	/* Get the matrices */
	const float* A = matrices[0];
	const float* B = matrices[1];
	const float* Q = matrices[2];
	const float* R = matrices[3];

	/* row_a and column_a has the same value */
	const size_t row_a = rows[0];
	const size_t column_a = columns[0];

	/* row_b has the same value as row_a */
	const size_t row_b = rows[1];
	const size_t column_b = columns[1];

	/* row_q and column_q has the same value as row_a and column_a */
	const size_t row_q = rows[2];
	const size_t column_q = columns[2];

	/* row_r and column_r has the same value as column_b */
	const size_t row_r = rows[3];
	const size_t column_r = columns[3];

	/* row_x and column_x has the same value as row_a and column_a */
	const size_t row_x = row_a;
	const size_t column_x = column_a;

	/* Compute A'*X*A = ATXA */
	float* ATXA = (float*)malloc(column_a * column_a * sizeof(float));
	RTYUI(A, X, A, ATXA, row_a, column_a, row_x, column_x, row_a, column_a);

	/* Compute A'*X*B = ATXB */
	float* ATXB = (float*)malloc(column_a * column_b * sizeof(float));
	RTYUI(A, X, B, ATXB, row_a, column_a, row_x, column_x, row_b, column_b);

	/* Compute B'*X*B + R = BTXBpR */
	float* BTXBpR = (float*)malloc(column_b * column_b * sizeof(float));
	RTYUI(B, X, B, BTXBpR, row_b, column_b, row_x, column_x, row_b, column_b);
	size_t i;
	for (i = 0; i < column_b * column_b; i++) {
		BTXBpR[i] = R[i];
	}

	/* Do inverse of BTXBpR */
	inv(BTXBpR, column_b);

	/* Compute B'*X*A = BTXA */
	float* BTXA = (float*)malloc(column_b * column_a * sizeof(float));
	RTYUI(B, X, A, BTXA, row_b, column_b, row_x, column_x, row_a, column_a);

	/* Compute ATXB * BTXBpR * BTXA = ATXB_BTXBpR_BTXA */
	float* BTXBpR_BTXA = (float*)malloc(column_b * column_a * sizeof(float));
	mul(BTXBpR, BTXA, BTXBpR_BTXA, column_b, column_b, column_a);
	float* ATXB_BTXBpR_BTXA = (float*)malloc(column_a * column_a * sizeof(float));
	mul(ATXB, BTXBpR_BTXA, ATXB_BTXBpR_BTXA, column_a, column_b, column_a);

	/* Return X */
	for (i = 0; i < row_x * column_x; i++) {
		X[i] = ATXA[i] - X[i] - ATXB_BTXBpR_BTXA[i] + Q[i];
	}

	/* Free */
	free(ATXA);
	free(ATXB);
	free(BTXBpR);
	free(BTXA);
	free(BTXBpR_BTXA);
	free(ATXB_BTXBpR_BTXA);
}

int main() {
    clock_t start, end;
    float cpu_time_used;
    start = clock();

	const float A[row_A * row_A] = { 1, 2, 3, 4 };
	const float B[row_A * column_B] = { -2, 1, 4, 0 };
	const float Q[row_A * row_A] = { 1, 0, 2, 3 };
	const float R[column_B * column_B] = { -1, -2, -3, 4 };
	float X[row_A * row_A] = {0, 1, -2, 1 };

	const float* matrices[4] = { A, B, Q, R };
	const size_t rows[4] = { row_A, row_A, row_A, column_B };
	const size_t columns[4] = { row_A, column_B, row_A, column_B };

	odefun(0, X, matrices, rows, columns);

	printf("This is X:\n");
	print(X, row_A, row_A);


    end = clock();
    cpu_time_used = ((float)(end - start)) / CLOCKS_PER_SEC;
    printf("\nTotal speed  was %f\n", cpu_time_used);


    /* Check memory */
    detectmemoryleak();

    return EXIT_SUCCESS;
}

/*
 GNU Octave code:


A = [ 1, 2; 3, 4 ];
B = [ -2, 1; 4, 0 ];
Q = [ 1, 0; 2, 3 ];
R = [ -1, -2; -3, 4 ];
X = [0, 1; -2, 1 ];

X = A'*X*A - X - A'*X*B*inv(B'*X*B + R)*B'*X*A + Q

*/
/*
 * lbp.c
 *
 *  Created on: 15 oktober 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/*
 * Local Binary Pattern - Use the commented C-code inside lbp.c to tune in the radius
 * X[m*n]
 * radius - The radius of the circle
 * init_angle - The beginning of the 360 degrees angle in radian
 * angles[total_angles] - Angles in radians
 * total_angles - Total angles
 */
uint32_t lbp(const float X[], const size_t row, const size_t column, const float init_angle, const uint8_t radius, LBP_BIT lbp_bit) {
	/* Constants */
	const float angles8[8] = { 0.0f, 0.7854f, 1.5708f, 2.3562f, 3.1416f, 3.927f, 4.7124f, 5.4978f };
	const float angles16[16] = { 0.0f, 0.3927f, 0.7854f, 1.1781f, 1.5708f, 1.9635f, 2.3562f, 2.7489f, 3.1416f, 3.5343f, 3.927f, 4.3197f, 4.7124f, 5.1051f, 5.4978f, 5.8905f };
	const float angles24[24] = { 0.0f, 0.2618f, 0.5236f, 0.7854f, 1.0472f, 1.3090f, 1.5708f, 1.8326f, 2.0944f, 2.3562f, 2.6180f, 2.8798f, 3.1416f, 3.4034f, 3.6652f, 3.9270f, 4.1888f, 4.4506f, 4.7124f, 4.9742f, 5.2360f, 5.4978f, 5.7596f, 6.0214f };
	const float angles32[32] = { 0.0f, 0.19635f, 0.3927f, 0.58905f, 0.7854f, 0.98175f, 1.1781f, 1.3744f, 1.5708f, 1.7671f, 1.9635f, 2.1598f, 2.3562f, 2.5525f, 2.7489f, 2.9452f, 3.1416f, 3.3379f, 3.5343f, 3.7306f, 3.927f, 4.1233f, 4.3197f, 4.516f, 4.7124f, 4.9087f, 5.1051f, 5.3014f, 5.4978f, 5.6941f, 5.8905f, 6.0868f };
	
	/* Center coordinates */
	size_t half_row = row / 2U;
	size_t half_column = column / 2U;

	/* Get the center value */
	const float P = X[half_row * column + half_column];

	/* Initial parameters */
	uint8_t total_angles;
	const float* angles;
	switch (lbp_bit) {
	case LBP_BIT_8:
		total_angles = 8U;
		angles = angles8;
		break;
	case LBP_BIT_16:
		total_angles = 16U;
		angles = angles16;
		break;
	case LBP_BIT_24:
		total_angles = 24U;
		angles = angles24;
		break;
	case LBP_BIT_32:
		total_angles = 32U;
		angles = angles32;
		break;
	default:
		total_angles = 8U;
		angles = angles8;
		break;
	}

	/* Create the descriptor */
	uint8_t i;
	uint64_t descriptor = 0U;
	for (i = 0; i < total_angles; i++) {
		/* Compute the angle */
		const float angle = init_angle + angles[i];

		/* Find the circle coordinates */
		const size_t coordinate_row = roundf(half_row + radius * cosf(angle));
		const size_t coordinate_column = roundf(half_column + radius * sinf(angle));

		/* Check if the coordinate has a brighter color than centrum P */
		descriptor |= (X[coordinate_row * column + coordinate_column] > P) << i;
	}

	/* Return the descriptor */
	return descriptor;
}

/* C-code for drawing the circle
#include "CControl/ccontrol.h"

void drawCircle(const const int M, const int N, const uint8_t radius, char matrix[], const float init_angle, const float angles[], const uint8_t total_angles) {
	size_t centerX = M / 2;
	size_t centerY = N / 2;

	for (int i = 0; i < total_angles; i++) {
		float angle = init_angle + angles[i];
		int x = roundf(centerX + radius * cosf(angle));
		int y = roundf(centerY + radius * sinf(angle));
		matrix[x * N + y] = '.';
	}
}

void printMatrix(int M, int N, char matrix[]) {
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			printf("%c ", matrix[i * N + j]);
		}
		printf("\n");
	}
}

int main() {
	// Size of the matrix
	int M = 100;
	int N = 100;
	char* matrix = (char*)malloc(M * N);

	// Change this
	uint8_t radius = 16;
	int init_angle = 0;

	// Angles in radians
	const float steps8[8] = { 0.0f, 0.7854f, 1.5708f, 2.3562f, 3.1416f, 3.927f, 4.7124f, 5.4978f };
	const float steps16[16] = { 0.0f, 0.3927f, 0.7854f, 1.1781f, 1.5708f, 1.9635f, 2.3562f, 2.7489f, 3.1416f, 3.5343f, 3.927f, 4.3197f, 4.7124f, 5.1051f, 5.4978f, 5.8905f };
	const float steps24[24] = { 0.0f, 0.2618f, 0.5236f, 0.7854f, 1.0472f, 1.3090f, 1.5708f, 1.8326f, 2.0944f, 2.3562f, 2.6180f, 2.8798f, 3.1416f, 3.4034f, 3.6652f, 3.9270f, 4.1888f, 4.4506f, 4.7124f, 4.9742f, 5.2360f, 5.4978f, 5.7596f, 6.0214f };
	const float steps32[32] = { 0.0f, 0.19635f, 0.3927f, 0.58905f, 0.7854f, 0.98175f, 1.1781f, 1.3744f, 1.5708f, 1.7671f, 1.9635f, 2.1598f, 2.3562f, 2.5525f, 2.7489f, 2.9452f, 3.1416f, 3.3379f, 3.5343f, 3.7306f, 3.927f, 4.1233f, 4.3197f, 4.516f, 4.7124f, 4.9087f, 5.1051f, 5.3014f, 5.4978f, 5.6941f, 5.8905f, 6.0868f };

	// Fill the matrix
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			matrix[i * N + j] = ' ';
		}
	}

	// Draw a circle
	drawCircle(M, N, radius, matrix, init_angle * PI / 180.0f, steps32, 32);

	// Print a circle
	printMatrix(M, N, matrix);

	// Free
	free(matrix);

	// Return zero
	return 0;
}
*/
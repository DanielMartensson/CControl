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
uint64_t lbp(const float X[], const size_t row, const size_t column, const float init_angle, const uint8_t radius, LBP_BIT lbp_bit) {
	/* Constants */
	const float angles8[8] = { 0.0f, 0.7854f, 1.5708f, 2.3562f, 3.1416f, 3.927f, 4.7124f, 5.4978f };
	const float angles16[16] = { 0.0f, 0.3927f, 0.7854f, 1.1781f, 1.5708f, 1.9635f, 2.3562f, 2.7489f, 3.1416f, 3.5343f, 3.927f, 4.3197f, 4.7124f, 5.1051f, 5.4978f, 5.8905f };
	const float angles32[32] = { 0.0f, 0.19635f, 0.3927f, 0.58905f, 0.7854f, 0.98175f, 1.1781f, 1.3744f, 1.5708f, 1.7671f, 1.9635f, 2.1598f, 2.3562f, 2.5525f, 2.7489f, 2.9452f, 3.1416f, 3.3379f, 3.5343f, 3.7306f, 3.927f, 4.1233f, 4.3197f, 4.516f, 4.7124f, 4.9087f, 5.1051f, 5.3014f, 5.4978f, 5.6941f, 5.8905f, 6.0868f };
	const float angles64[64] = { 0.0f, 0.098175f, 0.19635f, 0.29452f, 0.3927f, 0.49087f, 0.58905f, 0.68722f, 0.7854f, 0.88357f, 0.98175f, 1.0799f, 1.1781f, 1.2763f, 1.3744f, 1.4726f, 1.5708f, 1.669f, 1.7671f, 1.8653f, 1.9635f, 2.0617f, 2.1598f, 2.258f, 2.3562f, 2.4544f, 2.5525f, 2.6507f, 2.7489f, 2.8471f, 2.9452f, 3.0434f, 3.1416f, 3.2398f, 3.3379f, 3.4361f, 3.5343f, 3.6325f, 3.7306f, 3.8288f, 3.927f, 4.0252f, 4.1233f, 4.2215f, 4.3197f, 4.4179f, 4.516f, 4.6142f, 4.7124f, 4.8106f, 4.9087f, 5.0069f, 5.1051f, 5.2033f, 5.3014f, 5.3996f, 5.4978f, 5.596f, 5.6941f, 5.7923f, 5.8905f, 5.9887f, 6.0868f, 6.185f };
	
	/* Center coordinates */
	size_t half_row = row / 2.0f;
	size_t half_column = column / 2.0f;

	/* Get the center value */
	const float P = X[half_row * column + half_column];

	/* Initial parameters */
	uint8_t total_angles;
	const float* angles;
	switch (lbp_bit) {
	case LBP_BIT_8:
		total_angles = 8;
		angles = angles8;
		break;
	case LBP_BIT_16:
		total_angles = 16;
		angles = angles16;
		break;
	case LBP_BIT_32:
		total_angles = 32;
		angles = angles32;
		break;
	case LBP_BIT_64:
		total_angles = 64;
		angles = angles64;
		break;
	default:
		total_angles = 8;
		angles = angles8;
		break;
	}

	/* Create the descriptor */
	uint8_t i;
	uint64_t descriptor = 0;
	for (i = 0; i < total_angles; i++) {
		/* Compute the angle */
		const float angle = init_angle + angles[i];

		/* Find the circle coordinates */
		const size_t coordinate_row = roundf(half_row + radius * cosf(angle));
		const size_t coordinate_column = roundf(half_column + radius * sinf(angle));

		/* Check if the coordinate has a ligher color than centrum P */
		descriptor |= (X[coordinate_row * column + coordinate_column] > P) << i;
	}

	/* Return the descriptor */
	return descriptor;
}

/* C-code for drawing the circle
#include <stdio.h>
#include <math.h>
#include <stdint.h>

void drawCircle(const const int M, const int N, const uint8_t radius, char matrix[], const float init_angle, const float angles[], const uint8_t total_angles) {
	size_t centerX = M / 2.0;
	size_t centerY = N / 2.0;

	for (int i = 0; i < total_angles; i++) {
		float angle = init_angle + angles[i];
		int x = roundf(centerX + radius * cosf(angle));
		int y = roundf(centerY + radius * sinf(angle));
		matrix[x*N + y] = '.';
	}
}

void printMatrix(int M, int N, char matrix[]) {
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			printf("%c ", matrix[i*N + j]);
		}
		printf("\n");
	}
}

int main() {
	int M = 40;
	int N = 40;
	float radius = 10;
	int init_angle = 0;
	char matrix[M*N];
	const float steps8[8] = {0.0f, 0.7854f, 1.5708f, 2.3562f, 3.1416f, 3.927f, 4.7124f, 5.4978f};
	const float steps16[16] = {0.0f, 0.3927f, 0.7854f, 1.1781f, 1.5708f, 1.9635f, 2.3562f, 2.7489f, 3.1416f, 3.5343f, 3.927f, 4.3197f, 4.7124f, 5.1051f, 5.4978f, 5.8905f};
	const float steps32[32] = {0.0f, 0.19635f, 0.3927f, 0.58905f, 0.7854f, 0.98175f, 1.1781f, 1.3744f, 1.5708f, 1.7671f, 1.9635f, 2.1598f, 2.3562f, 2.5525f, 2.7489f, 2.9452f, 3.1416f, 3.3379f, 3.5343f, 3.7306f, 3.927f, 4.1233f, 4.3197f, 4.516f, 4.7124f, 4.9087f, 5.1051f, 5.3014f, 5.4978f, 5.6941f, 5.8905f, 6.0868f};
	const float steps64[64] = {0.0f, 0.098175f, 0.19635f, 0.29452f, 0.3927f, 0.49087f, 0.58905f, 0.68722f, 0.7854f, 0.88357f, 0.98175f, 1.0799f, 1.1781f, 1.2763f, 1.3744f, 1.4726f, 1.5708f, 1.669f, 1.7671f, 1.8653f, 1.9635f, 2.0617f, 2.1598f, 2.258f, 2.3562f, 2.4544f, 2.5525f, 2.6507f, 2.7489f, 2.8471f, 2.9452f, 3.0434f, 3.1416f, 3.2398f, 3.3379f, 3.4361f, 3.5343f, 3.6325f, 3.7306f, 3.8288f, 3.927f, 4.0252f, 4.1233f, 4.2215f, 4.3197f, 4.4179f, 4.516f, 4.6142f, 4.7124f, 4.8106f, 4.9087f, 5.0069f, 5.1051f, 5.2033f, 5.3014f, 5.3996f, 5.4978f, 5.596f, 5.6941f, 5.7923f, 5.8905f, 5.9887f, 6.0868f, 6.185f};
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			matrix[i*N + j] = ' ';
		}
	}
	drawCircle(M, N, radius, matrix, init_angle, steps64, 64);
	printMatrix(M, N, matrix);
	return 0;
}
*/
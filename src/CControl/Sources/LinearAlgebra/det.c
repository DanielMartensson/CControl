/*
 * det.c
 *
 *  Created on: 2 mars 2020
 *      Author: hp
 */

#include "../../Headers/Functions.h"

/*
 * Computes the determinant of square matrix A
 * A [m*n]
 * n == m
 * Return: Determinalt value, or -1 for singular matrix
 */
float det(float *A, int row) {

	float determinant = 1.0;
	float LU[row * row];
	int P[row];
	uint8_t status = lup(A, LU, P, row);
	if(status == 0)
		return -1; // matrix is singular

	for (int i = 0; i < row; ++i)
		determinant *= *(LU + row * *(P + i) + i);

	int j = 0;
	for (int i = 0; i < row; ++i)
		if (*(P + i) != i)
			++j;

	if (j && (j - 1) % 2 == 1)
		determinant = -determinant;

	return determinant;
}

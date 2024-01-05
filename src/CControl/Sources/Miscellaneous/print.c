/*
 * print.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "miscellaneous.h"

/*
 * Print matrix or vector - Just for error check
 */
void print(const float A[], const size_t row, const size_t column) {
	size_t i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < column; j++) {
			printf("%0.7f\t", *(A++));
		}
		printf("\n");
	}
	printf("\n");
}

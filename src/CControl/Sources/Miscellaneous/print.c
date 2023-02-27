/*
 * print.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Print matrix or vector - Just for error check
 */
void print(float A[], uint16_t row, uint16_t column) {
	uint16_t i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < column; j++) {
			printf("%0.18f\t", *(A++));
		}
		printf("\n");
	}
	printf("\n");
}

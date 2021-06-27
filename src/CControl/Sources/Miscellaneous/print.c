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
void print(float A[], uint32_t row, uint32_t column) {
	for (uint32_t i = 0; i < row; i++) {
		for (uint32_t j = 0; j < column; j++) {
			printf("%0.18f\t", *(A++));
		}
		printf("\n");
	}
	printf("\n");
}

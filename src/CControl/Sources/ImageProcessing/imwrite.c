/*
 * imwrite.c
 *
 *  Created on: 06 januari 2024
 *      Author: Daniel Mårtensson
 */

#include "imageprocessing.h"

bool imwrite(const uint8_t X[], const char file_path[], const size_t row, const size_t column) {
	/* Check if file_path holds .pgm */
	if (strstr(file_path, ".pgm") == NULL) {
		return false;
	}

	/* Open file */
	FILE* file = fopen(file_path, "wb");
	if (file == NULL) {
		return false;
	}

	/* Write the magic P5 */
	fprintf(file, "%s\n", "P5");

	/* Write the size */
	fprintf(file, "%lu %lu\n", column, row);

	/* Find the max of X */
	size_t i;
	uint8_t max_value = 0;
	const size_t row_column = row * column;
	for (i = 0; i < row_column; i++) {
		if (max_value < X[i]) {
			max_value = X[i];
		}
	}

	/* Print the max value */
	fprintf(file, "%i\n", max_value);

	/* Print the data */
	if (fwrite(X, 1, row_column, file) != row_column) {
		fclose(file);
		return false;
	}

	/* Close */
	fclose(file);

	/* Return true */
	return true;
}
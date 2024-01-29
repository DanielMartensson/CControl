/*
 * saveload.c
 *
 *  Created on: 29 Januari 2024
 *      Author: Daniel Mårtensson
 */

#include "hardware.h"

/*
 * Save or load an array of data
 */
bool saveload(uint8_t data[], const uint32_t data_length, char file_name[], const bool save) {
	if (save) {
		/* Write a file */
		FILE* file = NULL;
		file = fopen(file_name, "wb");
		if (file == NULL) {
			return false;
		}
		fwrite(data, 1, data_length, file);
		fclose(file);
		return true;
	}
	else {
		/* Read a file */
		FILE* file = NULL;
		file = fopen(file_name, "rb");
		if (file == NULL) {
			file = fopen(file_name, "wb");
			if (file == NULL) {
				return false;
			}
		}
		fread(data, 1, data_length, file);
		fclose(file);
		return true;
	}
}
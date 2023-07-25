/*
 * pgm.c
 *
 *  Created on: 24 juli 2023
 *      Author: Daniel M�rtensson
 */

#include "../../Headers/functions.h"

PGM* pgm_read(const char file_path[]) {
	/* Check if file_path holds .pgm */
	if (strstr(file_path, ".pgm") == NULL) {
		return NULL;
	}

	/* Open file */
	FILE* file = fopen(file_path, "rb");
	if (file == NULL) {
		return NULL;
	}

	/* Create image */
	PGM* image = (PGM*)malloc(sizeof(PGM));

	/* Get the magic e.g P2 or P5 */
	char line[256];
	fgets(line, sizeof(line), file);

	/* Check if it's P5 */
	bool isP5 = strstr(line, "P5") != NULL;

	/* Read the comments */
	bool search_comment = true;
	while (search_comment) {
		fgets(line, sizeof(line), file);
		search_comment = line[0] == '#';
	}

	/* Read height and width */
	int w, h;
	sscanf(line, "%d %d", &w, &h);
	image->height = h;
	image->width = w;

	/* Read the maximum value for gray scale */
	int s;
	fgets(line, sizeof(line), file);
	sscanf(line, "%d", &s);
	image->max_gray_value = s;

	/* Read the data */
	const size_t hw = h * w;
	image->pixels = (uint8_t*)malloc(hw);
	if (isP5) {
		fread(image->pixels, 1, hw, file);
	}
	else {
		size_t i;
		for (i = 0; i < hw; i++) {
			fscanf(file, "%d", &s);
			image->pixels[i] = s;
		}
	}

	/* Everything went fine */
	return image;
}

void pgm_free(PGM* image) {
	if (image) {
		free(image->pixels);
		free(image);
	}
}

void pgm_print(PGM* image) {
	if (image) {
		const size_t h = image->height;
		const size_t w = image->width;
		size_t i, j;
		uint8_t* pixels0 = image->pixels;
		for (i = 0; i < h; i++) {
			for (j = 0; j < w; j++) {
				printf("%d ", (*image->pixels++));
			}
			printf("\n");
		}
		image->pixels = pixels0;
		printf("\n");
	}
}
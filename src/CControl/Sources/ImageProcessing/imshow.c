/*
 * imshow.c
 *
 *  Created on: 5 oktober 2023
 *      Author: Daniel Mårtensson
 */

#include "imageprocessing.h"

/* 
 * Print an image
 */
void imshow(PGM* image) {
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
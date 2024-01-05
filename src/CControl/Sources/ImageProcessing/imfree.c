/*
 * imfree.c
 *
 *  Created on: 5 oktober 2023
 *      Author: Daniel Mårtensson
 */

#include "imageprocessing.h"

/*
 * Free an image
 */
void imfree(PGM* image) {
	if (image) {
		free(image->pixels);
		free(image);
	}
}

/*
 * structs.h
 *
 *  Created on: 20 Juli. 2023
 *      Author: Daniel Mårtensson
 */

#ifndef CCONTROL_HEADERS_STRUCTS_H_
#define CCONTROL_HEADERS_STRUCTS_H_

/* For pgm_read.c */
typedef struct {
	size_t width;
	size_t height;
	uint8_t max_gray_value;
	uint8_t* pixels;
}PGM;

/* For fisherfaces.c */
typedef struct {
	size_t row;
	size_t column;
	size_t* class_id;
	float* data;
}FISHER_MODEL;

#endif /* !CCONTROL_HEADERS_STRUCTS_H_ */
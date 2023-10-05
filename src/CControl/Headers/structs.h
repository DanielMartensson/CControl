/*
 * structs.h
 *
 *  Created on: 20 Juli. 2023
 *      Author: Daniel Mårtensson
 */

#ifndef CCONTROL_HEADERS_STRUCTS_H_
#define CCONTROL_HEADERS_STRUCTS_H_

/* For imshow.c and imread.c */
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

/* For fast.c */
typedef struct {
	int x;
	int y;
} fast_corner_xy;


#endif /* !CCONTROL_HEADERS_STRUCTS_H_ */
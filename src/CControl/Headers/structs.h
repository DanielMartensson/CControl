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

/* For fisherfaces.c */
typedef struct {
	/* For reduce the size of the image */
	size_t pooling_size;
	POOLING_METHOD pooling_method;

	/* Source to the data */
	char folder_path[256];

	/* Remove outliers settings */
	bool remove_outliers;
	float epsilon;
	size_t min_pts;

	/* Kernel PCA settings */
	size_t components_pca;
	float kernel_parameters[2];
	KERNEL_METHOD kernel_method;

	/* Neural network settings */
	float C;
	float lambda;

	/* This is the model parameters - The final stage */
	float* model_w;
	float* model_b;
	size_t model_row;
	size_t model_column;
	bool isModelCreated;
	size_t (*activation_function)(float*, size_t);
}FISHER_FACES_SETTINGS;

/* For fast.c */
typedef struct {
	int x;
	int y;
} FAST_XY;

/* For brisk.c */
typedef struct {
	/* Collection of corners from FAST */
	int num_corners;
	FAST_XY* xy;

	/* Binary data [data_row * data_column] */
	uint8_t* data;
	size_t data_row;
	size_t data_column;
}BRISK;

#endif /* !CCONTROL_HEADERS_STRUCTS_H_ */
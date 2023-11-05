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

/* For data_collect.c */
typedef struct {
	/* Model */
	uint8_t total_models;
	float* model_w[10];
	float* model_b[10];
	size_t model_row[10];
	size_t model_column[10];
	bool is_model_created[10];
	ACTIVATION_FUNCTION activation_function[10];

	/* Data */
	size_t input_row;
	size_t column;
	size_t* class_id_original;
	size_t* class_id_k_means;
	size_t classes_original;
	size_t classes_k_means;
	float* input;
}DATA_COLLECT;

typedef struct {
	/* General */
	char folder_path[260];
	COLLECT_TYPE collect_type;
	bool save_model;

	/* Neural network settings */
	float C;
	float lambda;

	/* For Fisherfaces */
	size_t pooling_size;
	POOLING_METHOD pooling_method;

	/* For Fisherfaces */
	bool remove_outliers;
	float epsilon;
	size_t min_pts;

	/* For Fisherfaces */
	size_t components_pca;
	float kernel_parameters[2];
	KERNEL_METHOD kernel_method;

	/* For ORP */
	size_t k_value;
	float sigma1;
	float sigma2;
	uint8_t threshold_sobel;
	uint8_t threshold_fast;
	FAST_METHOD fast_method;
}DATA_SETTINGS;

/* For fast.c */
typedef struct {
	int x;
	int y;
} FAST_XY;

/* For orp.c */
typedef struct {
	/* Collection of corners from FAST */
	int num_corners;
	FAST_XY* xy;

	/* Binary data [data_row * data_column] */
	float* data;
	size_t data_row;
	size_t data_column;
}ORP;

#endif /* !CCONTROL_HEADERS_STRUCTS_H_ */
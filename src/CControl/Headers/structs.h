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

/* For imcollect.c */
typedef struct {
	/* Model */
	uint8_t total_models;
	float* model_w[10];
	float* model_b[10];
	size_t model_row[10];
	size_t model_column[10];
	ACTIVATION_FUNCTION activation_function[10];
}MODEL;

typedef struct {
	/* Models */
	MODEL fisherfaces_models;
	MODEL odorp_models;

	/* Data */
	size_t input_row;
	size_t input_column;
	size_t* class_id_original;
	size_t* class_id_k_means;
	size_t classes_original;
	size_t classes_k_means;
	float* input;
}DATA_COLLECT;

typedef struct {
	/* General */
	char folder_path[260];
	bool save_model;

	/* For nn.c */
	float C;
	float lambda;

	/* For fisherfaces.c */
	size_t pooling_size;
	POOLING_METHOD pooling_method;

	/* For fisherfaces.c */
	bool remove_outliers;
	float epsilon;
	size_t min_pts;

	/* For fisherfaces.c */
	size_t components_pca;
	float kernel_parameters[2];
	KERNEL_METHOD kernel_method;	
}DATA_SETTINGS_FISHERFACES;

/* For odorp.c */
typedef struct {
	/* General */
	char folder_path[260];
	bool save_model;

	/* For nn.c */
	float C;
	float lambda;

	/* For orp.c */
	size_t k_value;
	float sigma1;
	float sigma2;
	uint8_t threshold_sobel;
	uint8_t threshold_fast;
	FAST_METHOD fast_method;
}DATA_SETTINGS_ODORP;

/* For imcollect.c */
typedef struct {
	DATA_SETTINGS_ODORP data_settings_odorp;
	DATA_SETTINGS_FISHERFACES data_settings_fisherfaces;
	DATA_SETTINGS_CHOICE data_settings_choice;
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
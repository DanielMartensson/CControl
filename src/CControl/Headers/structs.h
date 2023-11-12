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

	/* Data */
	size_t input_row;
	size_t input_column;
	size_t* class_id;
	size_t classes;
	float* input;
}MODEL_FISHERFACES;

/* For imcollect.c */
typedef struct {
	/* Model */
	uint64_t* data;
	size_t N;
	uint8_t* class_id;
	uint8_t classes;
}MODEL_ODORP;

typedef struct {
	/* Models */
	MODEL_FISHERFACES fisherfaces_model;
	MODEL_ODORP odorp_model;
	ALGORITHM_CHOICE model_choice;
}MODEL;

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

	/* For orp.c */
	float sigma;
	uint8_t fast_threshold;
	FAST_METHOD fast_method;
}DATA_SETTINGS_ODORP;

/* For imcollect.c */
typedef struct {
	DATA_SETTINGS_ODORP data_settings_odorp;
	DATA_SETTINGS_FISHERFACES data_settings_fisherfaces;
	ALGORITHM_CHOICE model_choice;
}MODEL_SETTINGS;

/* For fast_features.c */
typedef struct {
	int x;
	int y;
} FAST_XY;

/* For orp.c */
typedef struct {
	/* Collection of corners from FAST */
	int num_corners;
	FAST_XY* xy;

	/* Binary data with legnth N */
	uint64_t* data;
	size_t N;
}ORP;

#endif /* !CCONTROL_HEADERS_STRUCTS_H_ */
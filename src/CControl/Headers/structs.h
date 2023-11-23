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
}MODEL_NN;

typedef struct {
	/* Models */
	MODEL_NN fisherfaces_model;
	MODEL_NN sfa_model;
	SETTINGS_CHOICE settings_choice;
}MODEL;

/* For sfa.c */
typedef struct {
	FAST_METHOD fast_method;
	uint8_t fast_threshold;
	uint8_t sobel_threshold;
	uint8_t histogram_size;
	uint8_t points_grid;
	uint8_t length_grid;
}SETTINGS_SFA;

/* For fisherfaces.c */
typedef struct {
	size_t pooling_size;
	POOLING_METHOD pooling_method;
}SETTINGS_FISHERFACES;

/* For all models */
typedef struct {
	/* General */
	char folder_path[260];
	bool save_model;

	/* For nn.c */
	float C;
	float lambda;

	/* For kpda_lda_nn.c */
	bool remove_outliers;
	float epsilon;
	size_t min_pts;

	/* For kpda_lda_nn.c */
	size_t components_pca;
	float kernel_parameters[2];
	KERNEL_METHOD kernel_method;
}SETTINGS_GENERAL;

typedef struct {
	/* General settings */
	SETTINGS_GENERAL settings_general;

	/* Specific Settings */
	SETTINGS_SFA settings_sfa;
	SETTINGS_FISHERFACES settings_fisherfaces;
}DATA_SETTINGS;

/* For imcollect.c */
typedef struct {
	DATA_SETTINGS data_settings_sfa;
	DATA_SETTINGS data_settings_fisherfaces;
	SETTINGS_CHOICE settings_choice;
}MODEL_SETTINGS;

/* For fast_features.c */
typedef struct {
	int x;
	int y;
} FAST_XY;

#endif /* !CCONTROL_HEADERS_STRUCTS_H_ */
/*
 * structs.h
 *
 *  Created on: 20 Juli. 2023
 *      Author: Daniel Mårtensson
 */

#ifndef STRUCTS_H_
#define STRUCTS_H_

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
	float* input;
	size_t* class_id;
	size_t classes;
}MODEL_NN;

typedef struct {
	/* Models */
	MODEL_NN fisherfaces_model;
	SETTINGS_CHOICE settings_choice;
}MODEL;

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

	/* For pooing.c */
	size_t pooling_size;
	POOLING_METHOD pooling_method;
}MODEL_GENERAL_SETTINGS;

/* For imcollect.c */
typedef struct {
	MODEL_GENERAL_SETTINGS settings_fisherfaces;
	SETTINGS_CHOICE settings_choice;
}MODEL_SETTINGS;

/* For featuredetection.c */
typedef struct {
	int x;
	int y;
} FAST_XY;

/* For generalizedhough.c */
typedef struct {
	float alpha;
	float R;
	float shortest;
}GENERALIZED_HOUGH_VOTE;

/* For generalizedhough.c */
typedef struct {
	GENERALIZED_HOUGH_VOTE* vote;
	size_t votes_active;
}GENERALIZED_HOUGH_MODEL;

#endif /* !STRUCTS_H_ */
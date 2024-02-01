/*
 * structs.h
 *
 *  Created on: 20 Juli. 2023
 *      Author: Daniel M�rtensson
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
	uint8_t* class_id;
	uint8_t classes;
}MODEL_NN;

/* Model holder */
typedef struct {
	MODEL_NN fisherfaces_model;
	MODEL_CHOICE model_choice;
}MODEL;

/* For NN models */
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
	uint8_t min_pts;

	/* For kpda_lda_nn.c */
	size_t components_pca;
	float kernel_parameters[2];
	KERNEL_METHOD kernel_method;

	/* For pooing.c */
	uint8_t pooling_size;
	POOLING_METHOD pooling_method;
}MODEL_NN_SETTINGS;

/* For imcollect.c */
typedef struct {
	MODEL_NN_SETTINGS settings_fisherfaces;
	MODEL_CHOICE model_choice;
}MODEL_SETTINGS;

/* For featuredetection.c */
typedef struct {
	int x;
	int y;
} COORDINATE_XY;

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

/* For adaboost.c */
typedef struct {
	float polarity;
	size_t feature_index;
	float threshold;
	float alpha;
}ADABOOST_MODEL;

/* For haarlike.c */
typedef struct {
	HAARLIKE_FEATURE_CHOICE haarlike_feature_choice;
	uint8_t x1, x2, x3, x4, y1, y2, y3, y4;
	int8_t value;
}HAARLIKE_FEATURE;

/* For violajones.c */
typedef struct {
	HAARLIKE_FEATURE haarlike_feature;
	ADABOOST_MODEL adaboost_model;
}VIOLAJONES_MODEL;

#endif /* !STRUCTS_H_ */
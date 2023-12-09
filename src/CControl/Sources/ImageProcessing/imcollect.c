/*
 * imcollect.c
 *
 *  Created on: 28 oktober 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

MODEL* imcollect(const MODEL_SETTINGS* model_settings) {
	/* Each sub folder is a class */
	char** sub_folder_names = NULL;
	size_t sub_folder_count;
	switch (model_settings->settings_choice) {
	case SETTINGS_CHOICE_FISHERFACES:
		sub_folder_count = scan_sub_folder_names(model_settings->data_settings_fisherfaces.settings_general.folder_path, &sub_folder_names);
		break;
	case SETTINGS_CHOICE_SFA:
		sub_folder_count = scan_sub_folder_names(model_settings->data_settings_sfa.settings_general.folder_path, &sub_folder_names);
		break;
	}

	/* Create model */
	MODEL* model = (MODEL*)malloc(sizeof(MODEL));
	memset(model, 0, sizeof(MODEL));

	/* Copy over */
	model->settings_choice = model_settings->settings_choice;

	size_t i, j, k;
	for (i = 0; i < sub_folder_count; i++) {
		/* Get sub folder name */
		const char* sub_folder_name = sub_folder_names[i];

		/* Combine folder name with the folder path */
		char total_folder_path[260];
		switch (model_settings->settings_choice) {
		case SETTINGS_CHOICE_FISHERFACES:
			concatenate_paths(total_folder_path, model_settings->data_settings_fisherfaces.settings_general.folder_path, sub_folder_name);
			break;
		case SETTINGS_CHOICE_SFA:
			concatenate_paths(total_folder_path, model_settings->data_settings_sfa.settings_general.folder_path, sub_folder_name);
			break;
		}

		/* Scan the .pgm files */
		char** file_names = NULL;
		const size_t file_count = scan_file_names(total_folder_path, &file_names, ".pgm");

		/* Iterate the files */
		for (j = 0; j < file_count; j++) {
			/* Get the name of the file */
			const char* file_name = file_names[j];

			/* Combine the total folder path with the file name */
			char total_pgm_path[260];
			concatenate_paths(total_pgm_path, total_folder_path, file_name);

			/* Read the pgm file */
			PGM* image = imread(total_pgm_path);

			/* Check if image is valid */
			if (image) {
				/* Get the total new pixels */
				const size_t image_size = image->height * image->width;
				float* X = (float*)malloc(image_size * sizeof(float));
				for (k = 0; k < image_size; k++) {
					X[k] = (float)image->pixels[k];
				}

				/* Type of detection */
				switch (model_settings->settings_choice) {
				case SETTINGS_CHOICE_FISHERFACES: {
					size_t p = model_settings->data_settings_fisherfaces.settings_fisherfaces.pooling_size;
					if (model_settings->data_settings_fisherfaces.settings_fisherfaces.pooling_method == POOLING_METHOD_NO_POOLING) {
						/* This will cause X will have the same size as new_data */
						p = 1;
					}
					const size_t h = image->height / p;
					const size_t w = image->width / p;
					const size_t new_size = h * w;
					float* new_data = (float*)malloc(new_size * sizeof(float));
					pooling(X, new_data, image->height, image->width, p, model_settings->data_settings_fisherfaces.settings_fisherfaces.pooling_method);

					/* Compute current size */
					const size_t current_size = model->fisherfaces_model.input_row * model->fisherfaces_model.input_column;

					/* Allocate new rows */
					model->fisherfaces_model.input = (float*)realloc(model->fisherfaces_model.input, (current_size + new_size) * sizeof(float));

					/* Column will always be the new_size size */
					model->fisherfaces_model.input_column = new_size;

					/* Fill as row major */
					memcpy(model->fisherfaces_model.input + current_size, new_data, new_size * sizeof(float));

					/* Free new_data */
					free(new_data);

					/* Allocate new element */
					model->fisherfaces_model.class_id = (size_t*)realloc(model->fisherfaces_model.class_id, (model->fisherfaces_model.input_row + 1) * sizeof(size_t));

					/* Add new ID */
					model->fisherfaces_model.class_id[model->fisherfaces_model.input_row] = i;
					
					/* Count rows */
					model->fisherfaces_model.input_row += 1;

					break;
				}
				case SETTINGS_CHOICE_SFA: {
					/* Get SFA histogram */
					uint8_t new_size = model_settings->data_settings_sfa.settings_sfa.histogram_size;
					float* new_data = NULL;// sfa(X, model_settings->data_settings_sfa.settings_sfa.fast_threshold, model_settings->data_settings_sfa.settings_sfa.sobel_threshold, model_settings->data_settings_sfa.settings_sfa.fast_method, new_size, model_settings->data_settings_sfa.settings_sfa.histogram_filter_K, image->height, image->width);

					/* Compute current size */ 
					const size_t current_size = model->sfa_model.input_row * model->sfa_model.input_column;

					/* Allocate new rows */
					model->sfa_model.input = (float*)realloc(model->sfa_model.input, (current_size + new_size) * sizeof(float));
					
					/* Column will always be the new_size size */
					model->sfa_model.input_column = new_size;

					/* Fill as row major */
					memcpy(model->sfa_model.input + current_size, new_data, new_size * sizeof(float));

					/* Free new_data */
					free(new_data);

					/* Allocate new element */
					model->sfa_model.class_id = (size_t*)realloc(model->sfa_model.class_id, (model->sfa_model.input_row + 1) * sizeof(size_t));

					/* Add new ID */
					model->sfa_model.class_id[model->sfa_model.input_row] = i;

					/* Count rows */
					model->sfa_model.input_row += 1;

					break;
				}
				}

				/* Free */
				free(X);
			}

			/* Free the image */
			imfree(image);
		}

		/* Free */
		for (j = 0; j < file_count; j++) {
			free(file_names[j]);
		}
		free(file_names);
	}

	/* Free */
	for (i = 0; i < sub_folder_count; i++) {
		free(sub_folder_names[i]);
	}
	free(sub_folder_names);

	/* Sometimes transpose is necessary */
	switch (model_settings->settings_choice) {
	case SETTINGS_CHOICE_FISHERFACES:
		/* Compute max classes */
		if (model->fisherfaces_model.input_row > 0) {
			model->fisherfaces_model.classes = model->fisherfaces_model.class_id[model->fisherfaces_model.input_row - 1] + 1;
		}
	case SETTINGS_CHOICE_SFA:
		/* Compute max classes */
		if (model->sfa_model.input_row > 0) {
			model->sfa_model.classes = model->sfa_model.class_id[model->sfa_model.input_row - 1] + 1;
		}
		break;
	}

	/* Return model */
	return model;
}

void imcollectfree(MODEL* model) {
	if (model) {
		/* Model choice */
		switch (model->settings_choice) {
		case SETTINGS_CHOICE_FISHERFACES: {
			uint8_t i;
			for (i = 0; i < model->fisherfaces_model.total_models; i++) {
				free(model->fisherfaces_model.model_b[i]);
				free(model->fisherfaces_model.model_w[i]);
			}
			free(model->fisherfaces_model.input);
			free(model->fisherfaces_model.class_id);
			break;
		}
		case SETTINGS_CHOICE_SFA: {
			uint8_t i;
			for (i = 0; i < model->sfa_model.total_models; i++) {
				free(model->sfa_model.model_b[i]);
				free(model->sfa_model.model_w[i]);
			}
			free(model->sfa_model.input);
			free(model->sfa_model.class_id);
			break;
		}
		}

		/* Struct */
		free(model);
	}
}
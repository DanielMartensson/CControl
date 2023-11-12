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
	switch (model_settings->model_choice) {
	case MODEL_CHOICE_FISHERFACES:
		sub_folder_count = scan_sub_folder_names(model_settings->data_settings_fisherfaces.folder_path, &sub_folder_names);
		break;
	case MODEL_CHOICE_ODORP:
		sub_folder_count = scan_sub_folder_names(model_settings->data_settings_odorp.folder_path, &sub_folder_names);
		break;
	}

	/* Create model */
	MODEL* model = (MODEL*)malloc(sizeof(MODEL));
	memset(model, 0, sizeof(MODEL));

	/* Copy over */
	model->model_choice = model_settings->model_choice;

	size_t i, j, k;
	for (i = 0; i < sub_folder_count; i++) {
		/* Get sub folder name */
		const char* sub_folder_name = sub_folder_names[i];

		/* Combine folder name with the folder path */
		char total_folder_path[260];
		switch (model_settings->model_choice) {
		case MODEL_CHOICE_FISHERFACES:
			concatenate_paths(total_folder_path, model_settings->data_settings_fisherfaces.folder_path, sub_folder_name);
			break;
		case MODEL_CHOICE_ODORP:
			concatenate_paths(total_folder_path, model_settings->data_settings_odorp.folder_path, sub_folder_name);
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
				switch (model_settings->model_choice) {
				case MODEL_CHOICE_FISHERFACES: {
					size_t p = model_settings->data_settings_fisherfaces.pooling_size;
					if (model_settings->data_settings_fisherfaces.pooling_method == POOLING_METHOD_NO_POOLING) {
						/* This will cause X will have the same size as new_data */
						p = 1;
					}
					const size_t h = image->height / p;
					const size_t w = image->width / p;
					const size_t new_pixel_size = h * w;
					float* new_data = (float*)malloc(new_pixel_size * sizeof(float));
					pooling(X, new_data, image->height, image->width, p, model_settings->data_settings_fisherfaces.pooling_method);

					/* Compute current pixel size */
					const size_t current_pixel_size = model->fisherfaces_model.input_row * model->fisherfaces_model.input_column;

					/* Allocate new rows */
					model->fisherfaces_model.input = (float*)realloc(model->fisherfaces_model.input, (current_pixel_size + new_pixel_size) * sizeof(float));

					/* Column will always be the new_pixel_size size */
					model->fisherfaces_model.input_column = new_pixel_size;

					/* Fill as row major */
					memcpy(model->fisherfaces_model.input + current_pixel_size, new_data, new_pixel_size * sizeof(float));

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
				case MODEL_CHOICE_ODORP: {
					/* Get orp data */
					ORP* orp_data = orp(X, model_settings->data_settings_odorp.sigma, model_settings->data_settings_odorp.fast_threshold, model_settings->data_settings_odorp.fast_method, image->height, image->width);

					/* Allocate new rows */
					model->odorp_model.data = (uint64_t*)realloc(model->odorp_model.data, (model->odorp_model.N + orp_data->N) * sizeof(uint64_t));

					/* Fill as row major */
					memcpy(model->odorp_model.data + model->odorp_model.N, orp_data->data, orp_data->N * sizeof(uint64_t));

					/* Allocate new elements */
					model->odorp_model.class_id = (uint8_t*)realloc(model->odorp_model.class_id, (model->odorp_model.N + orp_data->N) * sizeof(uint8_t));

					/* Add new IDs */
					for (k = model->odorp_model.N; k < model->odorp_model.N + orp_data->N; k++) {
						model->odorp_model.class_id[k] = (uint8_t)i;
					}

					/* Count rows */
					model->odorp_model.N += orp_data->N;

					/* Free */
					orpfree(orp_data);

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
	switch (model_settings->model_choice) {
	case MODEL_CHOICE_FISHERFACES:
		/* Transpose becase it's much better to have row > column */
		tran(model->fisherfaces_model.input, model->fisherfaces_model.input_row, model->fisherfaces_model.input_column);
		k = model->fisherfaces_model.input_row;
		model->fisherfaces_model.input_row = model->fisherfaces_model.input_column;
		model->fisherfaces_model.input_column = k;

		/* Compute max classes */
		if (model->fisherfaces_model.input_column > 0) {
			model->fisherfaces_model.classes = model->fisherfaces_model.class_id[model->fisherfaces_model.input_column - 1] + 1;
		}
	case MODEL_CHOICE_ODORP:
		/* Compute max classes */
		if (model->odorp_model.N > 0) {
			model->odorp_model.classes = model->odorp_model.class_id[model->odorp_model.N - 1] + 1;
		}
		break;
	}

	/* Return model */
	return model;
}

void imcollectfree(MODEL* model) {
	if (model) {
		/* Model choice */
		switch (model->model_choice) {
		case MODEL_CHOICE_FISHERFACES: {
			uint8_t i;
			for (i = 0; i < model->fisherfaces_model.total_models; i++) {
				free(model->fisherfaces_model.model_b[i]);
				free(model->fisherfaces_model.model_w[i]);
			}
			free(model->fisherfaces_model.input);
			free(model->fisherfaces_model.class_id);
			break;

		}
		case MODEL_CHOICE_ODORP:
			free(model->odorp_model.data);
			free(model->odorp_model.class_id);
		}

		/* Struct */
		free(model);
	}
}
/*
 * imcollect.c
 *
 *  Created on: 28 oktober 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

DATA_COLLECT* imcollect(const DATA_SETTINGS* data_settings) {
	/* Each sub folder is a class */
	char** sub_folder_names = NULL;
	size_t sub_folder_count;
	switch (data_settings->data_settings_choice) {
	case DATA_SETTINGS_CHOICE_FISHERFACES:
		sub_folder_count = scan_sub_folder_names(data_settings->data_settings_fisherfaces.folder_path, &sub_folder_names);
		break;
	case DATA_SETTINGS_CHOICE_ODORP:
		sub_folder_count = scan_sub_folder_names(data_settings->data_settings_odorp.folder_path, &sub_folder_names);
		break;
	}
	
	/* Data collect */
	DATA_COLLECT* data_collect = (DATA_COLLECT*)malloc(sizeof(DATA_COLLECT));
	memset(data_collect, 0, sizeof(DATA_COLLECT));

	size_t i, j, k, current_pixel_size = 0;
	for (i = 0; i < sub_folder_count; i++) {
		/* Get sub folder name */
		const char* sub_folder_name = sub_folder_names[i];

		/* Combine folder name with the folder path */
		char total_folder_path[260];
		switch (data_settings->data_settings_choice) {
		case DATA_SETTINGS_CHOICE_FISHERFACES:
			concatenate_paths(total_folder_path, data_settings->data_settings_fisherfaces.folder_path, sub_folder_name);
			break;
		case DATA_SETTINGS_CHOICE_ODORP:
			concatenate_paths(total_folder_path, data_settings->data_settings_odorp.folder_path, sub_folder_name);
			break;
		}

		/* Scan the files */
		char** file_names = NULL;
		const size_t file_count = scan_file_names(total_folder_path, &file_names);

		/* Iterate the files */
		for (j = 0; j < file_count; j++) {
			/* Get the name of the file */
			const char* file_name = file_names[j];

			/* Check if the file is .pgm */
			if (!strstr(file_name, ".pgm")) {
				/*
				 * Parameters for collecting data
				 * I recommend generate_pgm.m file for generating the .pgm files
				 */
				continue;
			}

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
				float* new_data = NULL;
				size_t new_pixel_size;
				size_t row;
				switch (data_settings->data_settings_choice) {
				case DATA_SETTINGS_CHOICE_FISHERFACES: {
					size_t p = data_settings->data_settings_fisherfaces.pooling_size;
 					if (data_settings->data_settings_fisherfaces.pooling_method == POOLING_METHOD_NO_POOLING) {
						/* This will cause X will have the same size as new_data */
						p = 1;
					}
					const size_t h = image->height / p;
					const size_t w = image->width / p;
					new_pixel_size = h * w;
					row = 1U;
					new_data = (float*)malloc(new_pixel_size * sizeof(float));
					pooling(X, new_data, image->height, image->width, p, data_settings->data_settings_fisherfaces.pooling_method);
					
					/* Column will always be the new_pixel_size size */
					data_collect->input_column = new_pixel_size;
					break;
				}
				case DATA_SETTINGS_CHOICE_ODORP: {
					/* Get orp data */
					ORP* orp_data = orp(X, data_settings->data_settings_odorp.sigma1, data_settings->data_settings_odorp.sigma2, data_settings->data_settings_odorp.threshold_sobel, data_settings->data_settings_odorp.threshold_fast, data_settings->data_settings_odorp.fast_method, image->height, image->width);
					
					/* Compute new size */
					row = orp_data->data_row;
					new_pixel_size = orp_data->data_row * orp_data->data_column;

					/* Allocate new memory */
					new_data = (float*)malloc(new_pixel_size * sizeof(float));
					
					/* Copy over */
					memcpy(new_data, orp_data->data, new_pixel_size * sizeof(float));

					/* Column will always be the data_column size */
					data_collect->input_column = orp_data->data_column;
	
					/* Free */
					orpfree(orp_data);
				}
				}

				/* Free */
				free(X);

				/* Allocate new rows */
				data_collect->input = (float*)realloc(data_collect->input, (current_pixel_size + new_pixel_size) * sizeof(float));

				/* Remember */
				float* data0 = data_collect->input;

				/* Jump */
				data_collect->input += current_pixel_size;

				/* Fill as row major */
				memcpy(data_collect->input, new_data, new_pixel_size * sizeof(float));

				/* Go back to index 0 */
				data_collect->input = data0;

				/* Free new_data */
				free(new_data);

				/* Add current size */
				current_pixel_size += new_pixel_size;

				/* Allocate new element */
				data_collect->class_id_original = (size_t*)realloc(data_collect->class_id_original, (data_collect->input_row + row) * sizeof(size_t));
				
				/* Add new ID */
				for (k = data_collect->input_row; k < data_collect->input_row + row; k++) {
					data_collect->class_id_original[k] = i;
				}

				/* Count rows */
				data_collect->input_row += row;
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
	switch (data_settings->data_settings_choice) {
	case DATA_SETTINGS_CHOICE_FISHERFACES:
		/* Transpose becase it's much better to have row > column */
		tran(data_collect->input, data_collect->input_row, data_collect->input_column);
		k = data_collect->input_row;
		data_collect->input_row = data_collect->input_column;
		data_collect->input_column = k;

		/* Compute max classes */
		data_collect->classes_original = data_collect->class_id_original[data_collect->input_column - 1] + 1;
		break;
	case DATA_SETTINGS_CHOICE_ODORP:
		/* Compute max classes */
		data_collect->classes_original = data_collect->class_id_original[data_collect->input_row - 1] + 1;
		break;
	}

	/* Return data collection */
	return data_collect;
}

void imcollect_free(DATA_COLLECT* data_collect) {
	if (data_collect) {
		/* Data */
		free(data_collect->class_id_original);
		free(data_collect->class_id_k_means);
		free(data_collect->input);

		/* Models for fisherfaces */
		uint8_t i;
		for (i = 0; i < data_collect->fisherfaces_models.total_models; i++) {
			free(data_collect->fisherfaces_models.model_b[i]);
			free(data_collect->fisherfaces_models.model_w[i]);
		}

		/* Models for ODORP */
		for (i = 0; i < data_collect->odorp_models.total_models; i++) {
			free(data_collect->odorp_models.model_b[i]);
			free(data_collect->odorp_models.model_w[i]);
		}

		/* Struct */
		free(data_collect);
	}
}
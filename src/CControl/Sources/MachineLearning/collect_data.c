/*
 * collect_data.c
 *
 *  Created on: 28 oktober 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

DATA_COLLECT* collect_data(const DATA_COLLECT_SETTINGS* data_collect_settings) {
	/* Each sub folder is a class */
	char** sub_folder_names = NULL;
	const size_t sub_folder_count = scan_sub_folder_names(data_collect_settings->folder_path, &sub_folder_names);

	/* Fischer model */
	DATA_COLLECT* data_collect = (DATA_COLLECT*)malloc(sizeof(DATA_COLLECT));
	memset(data_collect, 0, sizeof(DATA_COLLECT));

	size_t i, j, k, current_pixel_size = 0;
	for (i = 0; i < sub_folder_count; i++) {
		/* Get sub folder name */
		const char* sub_folder_name = sub_folder_names[i];

		/* Combine folder name with the folder path */
		char total_folder_path[260];
		concatenate_paths(total_folder_path, data_collect_settings->folder_path, sub_folder_name);

		/* Scan the files */
		char** file_names = NULL;
		const size_t file_count = scan_file_names(total_folder_path, &file_names);

		/* Iterate the files */
		for (j = 0; j < file_count; j++) {
			/* Get the name of the file */
			const char* file_name = file_names[j];

			/* Check if the file is .pgm */
			if (!strstr(file_name, ".pgm")) {
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
				BRISK* brisk_data;
				size_t row;
				switch (data_collect_settings->collect_type) {
				case COLLECT_TYPE_FISHERFACES: {
					size_t p = data_collect_settings->pooling_size;
 					if (data_collect_settings->pooling_method == POOLING_METHOD_NO_POOLING) {
						/* This will cause X will be the same size as new_data */
						p = 1;
					}
					const size_t h = image->height / p;
					const size_t w = image->width / p;
					new_pixel_size = h * w;
					row = 1U;
					new_data = (float*)malloc(new_pixel_size * sizeof(float));
					pooling(X, new_data, image->height, image->width, p, data_collect_settings->pooling_method);
					
					/* Column will always be the new_pixel_size size */
					data_collect->column = new_pixel_size;
					break;
				}
				case COLLECT_TYPE_ODBRISK: {
					/* Get brisk data */
					BRISK* brisk_data = brisk(X, data_collect_settings->sigma1, data_collect_settings->sigma2, data_collect_settings->threshold_sobel, data_collect_settings->threshold_fast, data_collect_settings->fast_method, image->height, image->width);
					
					/* Compute new size */
					row = brisk_data->data_row;
					new_pixel_size = brisk_data->data_row * brisk_data->data_column;

					/* Allocate new memory */
					new_data = (float*)malloc(new_pixel_size * sizeof(float));
					
					/* Copy over */
					memcpy(new_data, brisk_data->data, new_pixel_size * sizeof(float));

					/* Column will always be the data_column size */
					data_collect->column = brisk_data->data_column;
	
					/* Free */
					briskfree(brisk_data);
				}
				}

				/* Free */
				free(X);

				/* Allocate new rows */
				data_collect->data = (float*)realloc(data_collect->data, (current_pixel_size + new_pixel_size) * sizeof(float));

				/* Remember */
				float* data0 = data_collect->data;

				/* Jump */
				data_collect->data += current_pixel_size;

				/* Fill as row major */
				memcpy(data_collect->data, new_data, new_pixel_size * sizeof(float));

				/* Free new_data */
				free(new_data);

				/* Add current size */
				current_pixel_size += new_pixel_size;

				/* Go back to index 0 */
				data_collect->data = data0;

				/* Allocate new element */
				data_collect->class_id = (size_t*)realloc(data_collect->class_id, (data_collect->row + row) * sizeof(size_t));

				/* Add new ID */
				for (k = data_collect->row; k < data_collect->row + row; k++) {
					data_collect->class_id[k] = i;
				}

				/* Count rows */
				data_collect->row += row;
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
	switch (data_collect_settings->collect_type) {
	case COLLECT_TYPE_FISHERFACES:
		/* Transpose becase it's much better to have row > column */
		tran(data_collect->data, data_collect->row, data_collect->column);
		k = data_collect->row;
		data_collect->row = data_collect->column;
		data_collect->column = k;
		break;
	}

	/* Return model */
	return data_collect;
}
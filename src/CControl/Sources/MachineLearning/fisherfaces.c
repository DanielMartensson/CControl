/*
 * fisherfaces.c
 *
 *  Created on: 27 mars 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

void fisherfaces_remove_outliers(FISHER_MODEL* fisher_model, float epsilon, size_t min_pts) {
	if (fisher_model) {
		/* Get rows, columns and data */
		size_t row = fisher_model->row;
		const size_t column = fisher_model->column;
		float* data = fisher_model->data;

		/* Create the new indexes */
		size_t* idx = (size_t*)malloc(row * sizeof(size_t));
		dbscan(data, idx, epsilon, min_pts, row, column);

		/* The rule is simple. Any index that holds 0 is noise and should be removed */
		size_t i, j, row_new = 0, current_size = 0;
		float* data0 = data;
		float* data_clean = NULL;
		for (i = 0; i < row; i++) {
			/* Check if it's noise */
			if (idx[i] != 0) {
				/* Allocate new row */
				data_clean = (float*)realloc(data_clean, (current_size + column) * sizeof(float));
				
				/* Remember orignal position */
				float* data_clean0 = data_clean;

				/* Jump */
				data_clean += current_size;

				/* Fill */
				for (j = 0; j < column; j++) {
					data_clean[j] = data[j];
				}

				/* Add current size */
				current_size += column;

				/* Count new row */
				row_new++;

				/* Go back to index 0 */
				data_clean = data_clean0;
			}

			/* Jump */
			data += column;
		}

		/* Go back to index 0 */
		data = data0;

		/* Free */
		free(data);
		free(idx);

		/* Replace data with data_clean */
		fisher_model->row = row_new;
		fisher_model->data = data_clean;
	}
}

FISHER_MODEL* fisherfaces_train_projection_matrix(FISHER_MODEL* fisher_model, size_t number_components, float kernel_parameters[], KERNEL_METHOD kernel_method) {
	/* Get row, column, ID */
	const size_t row = fisher_model->row;
	const size_t column = fisher_model->column;
	const size_t* class_id = fisher_model->class_id;

	/* Compute number of components for KPCA */
	const size_t cpca = column - class_id[column-1];

	/* Compute number of components for LDA */
	const size_t clda = cpca - 1 < number_components ? cpca - 1 : number_components;

	/* Do KPCA on the model */
	//kpca(fisher_model->data, Wpca, Ppca, Kpca, mu, cpca, row, column, kernel_parameters, kernel_method);

	/* Do LDA on P */
	//lda(Ppca, class_id, Wlda, Plda, clda, row, column);


}

FISHER_MODEL* fisherfaces_collect_data(const char folder_path[]) {
	/* Each sub folder is a class */
	char** sub_folder_names = NULL;
	const size_t sub_folder_count = scan_sub_folder_names(folder_path, &sub_folder_names);

	/* Fischer model */
	FISHER_MODEL* fisher_model = (FISHER_MODEL*)malloc(sizeof(FISHER_MODEL));
	memset(fisher_model, 0, sizeof(FISHER_MODEL));

	size_t i, j, k, current_pixel_size = 0;
	for (i = 0; i < sub_folder_count; i++) {
		/* Get sub folder name */
		const char* sub_folder_name = sub_folder_names[i];

		/* Combine folder name with the folder path */
		char total_folder_path[260];
		concatenate_paths(total_folder_path, folder_path, sub_folder_name);

		/* Scan the files */
		char** file_names = NULL;
		const size_t file_count = scan_file_names(total_folder_path, &file_names);

		/* Iterate the files */
		for (j = 0; j < file_count; j++) {
			/* Get the name of the file */
			const char* file_name = file_names[j];

			/* Combine the total folder path with the file name */
			char total_pgm_path[260];
			concatenate_paths(total_pgm_path, total_folder_path, file_name);

			/* Read the pgm file */
			PGM* image = pgm_read(total_pgm_path);

			/* Check if image is valid */
			if (image) {
				/* Get the total new pixels */
				const size_t new_pixel_size = image->height * image->width;

				/* Allocate new rows */
				fisher_model->data = (float*)realloc(fisher_model->data, (current_pixel_size + new_pixel_size) * sizeof(float));

				/* Remember */
				float* data0 = fisher_model->data;

				/* Jump */
				fisher_model->data += current_pixel_size;

				/* Fill as row major */
				for (k = 0; k < new_pixel_size; k++) {
					fisher_model->data[k] = (float)image->pixels[k];
				}
				
				/* Add current size */
				current_pixel_size += new_pixel_size;

				/* Go back to index 0 */
				fisher_model->data = data0;
				
				/* Allocate new element */
				fisher_model->class_id = (size_t*)realloc(fisher_model->class_id, (fisher_model->row + 1) * sizeof(size_t));
				
				/* Add new ID */
				fisher_model->class_id[fisher_model->row] = i;

				/* Column will always be the total pixel size */
				fisher_model->column = new_pixel_size;

				/* Count columns */
				fisher_model->row++;
			}

			/* Free the image */
			pgm_free(image);
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

	/* Transpose becase it's mutch better to have row > column */
	tran(fisher_model->data, fisher_model->row, fisher_model->column);
	k = fisher_model->row;
	fisher_model->row = fisher_model->column;
	fisher_model->column = k;

	/* Return model */
	return fisher_model;
}

void fisherfaces_free_model(FISHER_MODEL* fisher_model) {
	if (fisher_model) {
		free(fisher_model->class_id);
		free(fisher_model->data);
		free(fisher_model);
	}
}

void fisherfaces_print_model(FISHER_MODEL* fisher_model) {
	if (fisher_model) {
		const size_t r = fisher_model->row;
		const size_t c = fisher_model->column;
		printf("Rows %i and columns %i\n", r, c);
		size_t i, j;
		float* data0 = fisher_model->data;
		for (i = 0; i < r; i++) {
			if (i == 0) {
				for (j = 0; j < c; j++) {
					printf("%i\t", fisher_model->class_id[j]);
				}
				printf("\n");
				for (j = 0; j < c; j++) {
					printf("_\t");
				}
				printf("\n");
			}
			for (j = 0; j < c; j++) {
				printf("%i\t", (int32_t)(*fisher_model->data++));
			}
			printf("\n");
		}
		fisher_model->data = data0;
		printf("\n");
	}
}

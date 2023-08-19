/*
 * fisherfaces.c
 *
 *  Created on: 27 mars 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

 /* Return the index of the largest value of vector x */
static size_t activation_function(float x[], size_t length) {
	size_t max_index;
	amax(x, &max_index, length);
	return max_index;
}

static FISHER_MODEL* fisherfaces_collect_data(const char folder_path[], size_t p, POOLING_METHOD pooling_method);
static void fisherfaces_free_model(FISHER_MODEL* fisher_model);
static void fisherfaces_print_model(FISHER_MODEL* fisher_model);

void fisherfaces(const char folder_path[], bool remove_outliers, const float espilon, const size_t min_pts, const size_t pooling_size, POOLING_METHOD pooling_method, const size_t components_pca, const float kernel_parameters[], KERNEL_METHOD kernel_method, const float C, const float lambda) {
	/* 
	 * Parameters for collecting data
	 * I recommend fisherfaces.m file in MataveID for selecting the pooling size and method
	 * Example:
	 * const char folder_path[] = "C:\\Users\\dmn\\Downloads\\yale\\GIF";
	 * const size_t pooling_size = 8;
	 * POOLING_METHOD pooling_method = POOLING_METHOD_AVERAGE;
	 */
	printf("\t\t\t\tFisherfaces\n");

	/* Collect data */
	printf("1: Collecting data. Reading the .pgm files in row-major. PGM format P2 or P5 format.\n");
	FISHER_MODEL* fisher_model = fisherfaces_collect_data(folder_path, pooling_size, pooling_method);
	const size_t row = fisher_model->row;
	const size_t column = fisher_model->column;
	size_t* class_id = (size_t*)malloc(column * sizeof(size_t));
	memcpy(class_id, fisher_model->class_id, column * sizeof(size_t));
	const size_t classes = class_id[column - 1] + 1;

	/* Remove outliers */
	if (remove_outliers) {
		printf("\tOutliers removed: %i\n", cluster_filter(fisher_model->data, row, column, espilon, min_pts));
	}

	/*
	 * Parameters for KPCA:
	 * components_pca is a user defined tuning parameter
	 * Example:
	 * const size_t components_pca = 100;
	 * float kernel_parameters[2] = { 0.0000001f, 3.1 };
	 * KERNEL_METHOD kernel_method = KERNEL_METHOD_RBF;
	 */
	printf("2: Do Kernel Principal Component Analysis for creating a linear projection for nonlinear data.\n");
	float* Wpca = (float*)malloc(row * components_pca * sizeof(float));
	float* Ppca = (float*)malloc(components_pca * column * sizeof(float));
	kpca(fisher_model->data, Wpca, Ppca, components_pca, row, column, kernel_parameters, kernel_method);

	/*
	 * Parametets for LDA:
	 * components_lda must be classes - 1 because we want to avoid zero eigenvalues
	 */
	printf("3: Do Linear Discriminant Analysis for creating a linear projection for separation of class data.\n");
	const size_t components_lda = classes - 1;
	float* Wlda = (float*)malloc(components_pca * components_lda * sizeof(float));
	float* Plda = (float*)malloc(components_lda * column * sizeof(float));
	lda(Ppca, class_id, Wlda, Plda, components_lda, components_pca, column);

	/* Multiply W = Wlda'*Wpca' */
	printf("4: Combine Kernel Principal Component Analysis projection with Linear Discriminant projection.\n");
	float* W = (float*)malloc(components_lda * row * sizeof(float));
	tran(Wlda, components_pca, components_lda);
	tran(Wpca, row, components_pca);
	mul(Wlda, Wpca, W, components_lda, components_pca, row);

	/* Free some matrices that are not needed any longer */
	free(Wpca);
	free(Ppca);
	free(Wlda);
	free(Plda);

	/* Find the total projection */
	printf("5: Find the total projection of the nonlinear data.\n");
	float* P = (float*)malloc(components_lda * column * sizeof(float));
	mul(W, fisher_model->data, P, components_lda, row, column);

	/*
	 * Train Neural Network model of the total projection
	 * C is a tuning parameter
	 * lambda is a tuning parameter
	 * Important to turn P into transpose because we are going to use that as vectors ontop on each other
	 * Example:
	 * float C = 1;
	 * float lambda = 2.5;
	 */
	printf("6: Create a Neural Network for a linear model that can handle nonlinear data.\n");
	tran(P, components_lda, column);
	float* accuracy = (float*)malloc(classes * sizeof(float));
	bool* status = (bool*)malloc(classes * sizeof(bool));
	float* weight = (float*)malloc(classes * components_lda * sizeof(float));
	float* bias = (float*)malloc(classes * sizeof(float));
	float* labels = (float*)malloc(column * sizeof(float));
	nn(P, class_id, weight, bias, status, accuracy, column, components_lda, C, lambda);

	/* Free */
	free(status);
	free(accuracy);

	/*
	 * Multiply model_w = weight * W
	 * y = model_w*imagevector + model_b is our model.
	 * The index of the largest value of vector y is the class ID of imagevector
	 */
	printf("7: Creating the model for nonlinear data.\n");
	float* model_w = (float*)malloc(classes * row * sizeof(float));
	mul(weight, W, model_w, classes, components_lda, row);

	/* Check the accuracy of the model */
	size_t i, score = 0;
	float* x = (float*)malloc(classes * column * sizeof(float));
	mul(model_w, fisher_model->data, x, classes, row, column);
	tran(x, classes, column); /* Column major */
	float* x0 = x;
	size_t j;
	for (i = 0; i < column; i++) {
		for (j = 0; j < classes; j++) {
			x[j] += bias[j];
		}
		x += classes;
	}
	x = x0;

	for (i = 0; i < classes; i++) {
		/* Use activation function to determine the class ID */
		if (class_id[i] == activation_function(x, classes)) {
			score++;
		}
		x += classes;
	}
	x = x0;
	free(x);
	fisherfaces_free_model(fisher_model);

	/* Print status */
	printf("\tThe accuracy of the neural network is: %f\n", ((float)score) / ((float)classes) * 100.0f);

	/* Save wW and b as a function */
	printf("8: Saving the model to a .h file.\n");
	char model_path[260];
	concatenate_paths(model_path, folder_path, "ccontrol_model.h");
	FILE* file = fopen(model_path, "w");
	if (file) {
		fprintf(file, "/*\n");
		fprintf(file, " * ccontrol_model.h\n");
		fprintf(file, " *\n");
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		fprintf(file, " * Created on : %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		fprintf(file, " * Author: <Your name>\n");
		fprintf(file, " * Purpose: Classify <your things>\n");
		fprintf(file, " * Model ID: <ID of you model>\n");
		fprintf(file, " */\n\n");
		fprintf(file, "#ifndef CCONTROL_MODEL_H_\n");
		fprintf(file, "#define CCONTROL_MODEL_H_\n\n");
		fprintf(file, "/*\n");
		fprintf(file, " * This is Fisherfaces - A technique that can classify raw data.\n");
		fprintf(file, " * Most used for images, but you can use it at any data you want as long the data is stored into .pgm files in format P2 or P5.\n");
		fprintf(file, " * This file contains one matrix called 'model_w' and one vector called 'model_b'.\n");
		fprintf(file, " * To use this header file, your data that must be a vector of size 'model_column' in row-major.\n");
		fprintf(file, " * For example, if you have an image with size m * n, where m * n = 'model_column', then you need to turn it into a vector in row-wise, not in column-wise.\n");
		fprintf(file, " * Multiply your vector together with the matrix 'model_w' and add vector 'model_b'. Then you will get a new vector called y with the size 'model_row'.\n");
		fprintf(file, " * y = model_w*your_data_vector + model_b\n");
		fprintf(file, " * The index of the largest number inside the vector y is the class ID of your_data_vector.\n");
		fprintf(file, " * If you get wrong class ID, then you need to use take account to noise or use better dimension reduction.\n");
		fprintf(file, " */\n\n");
		fprintf(file, "#define model_row %i\n", classes);
		fprintf(file, "#define model_column %i\n", row);
		fprintf(file, "\n");
		fprintf(file, "const static float model_w[model_row * model_column] = { ");
		size_t count = 0, max_index_model_w = classes * row - 1;
		for (i = 0; i < classes; i++) {
			for (j = 0; j < row; j++) {
				if (count < max_index_model_w) {
					fprintf(file, "%0.9ef, ", model_w[i * row + j]);
				}
				else {
					fprintf(file, "%0.9ef };\n", model_w[i * row + j]);
				}
				count++;
			}
			fprintf(file, "\n");
		}
		fprintf(file, "const static float model_b[model_row] = { ");
		for (i = 0; i < classes; i++) {
			if (i < classes - 1) {
				fprintf(file, "%0.9ef, ", bias[i]);
			}
			else {
				fprintf(file, "%0.9ef };\n", bias[i]);
			}
		}
		fprintf(file, "\n");
		fprintf(file, "#endif /* !CCONTROL_MODEL_H_ */\n");
		fclose(file);
		printf("\tModel as been saved: %s\n", model_path);
	}
	else {
		printf("\tFile ccontrol_model.h could not be open. Check your writing rights at the folder %s.\n", folder_path);
	}

	/* Free */
	printf("9: Free allocated memory.\n");
	free(weight);
	free(bias);
	free(W);
	free(P);
	free(labels);
	free(model_w);
	free(class_id);
	printf("10: Everything is done...\n");
}

static FISHER_MODEL* fisherfaces_collect_data(const char folder_path[], size_t p, POOLING_METHOD pooling_method) {
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

			/* Check if the file is .pgm */
			if (!strstr(file_name, ".pgm")) {
				continue;
			}

			/* Combine the total folder path with the file name */
			char total_pgm_path[260];
			concatenate_paths(total_pgm_path, total_folder_path, file_name);

			/* Read the pgm file */
			PGM* image = pgm_read(total_pgm_path);

			/* Check if image is valid */
			if (image) {
				/* Get the total new pixels */
				const size_t image_size = image->height * image->width;

				/* Pooling */
				const size_t h = image->height / p;
				const size_t w = image->width / p;
				const size_t pooling_size = h*w;
				float* P = (float*)malloc(pooling_size * sizeof(float));
				float* A = (float*)malloc(image_size * sizeof(float));
				for (k = 0; k < image_size; k++) {
					A[k] = (float)image->pixels[k];
				}
				pooling(A, P, image->height, image->width, p, pooling_method);
				free(A);

				/* Allocate new rows */
				fisher_model->data = (float*)realloc(fisher_model->data, (current_pixel_size + pooling_size) * sizeof(float));

				/* Remember */
				float* data0 = fisher_model->data;

				/* Jump */
				fisher_model->data += current_pixel_size;

				/* Fill as row major */
				for (k = 0; k < pooling_size; k++) {
					fisher_model->data[k] = P[k];
				}

				/* Free pooling image */
				free(P);
				
				/* Add current size */
				current_pixel_size += pooling_size;

				/* Go back to index 0 */
				fisher_model->data = data0;
				
				/* Allocate new element */
				fisher_model->class_id = (size_t*)realloc(fisher_model->class_id, (fisher_model->row + 1) * sizeof(size_t));
				
				/* Add new ID */
				fisher_model->class_id[fisher_model->row] = i;

				/* Column will always be the pooling size */
				fisher_model->column = pooling_size;

				/* Count rows */
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

	/* Transpose becase it's much better to have row > column */
	tran(fisher_model->data, fisher_model->row, fisher_model->column);
	k = fisher_model->row;
	fisher_model->row = fisher_model->column;
	fisher_model->column = k;

	/* Return model */
	return fisher_model;
}

static void fisherfaces_free_model(FISHER_MODEL* fisher_model) {
	if (fisher_model) {
		free(fisher_model->class_id);
		free(fisher_model->data);
		free(fisher_model);
	}
}

static void fisherfaces_print_model(FISHER_MODEL* fisher_model) {
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
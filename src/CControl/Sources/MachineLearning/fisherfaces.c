/*
 * fisherfaces.c
 *
 *  Created on: 27 mars 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/* Private functions */
static void fisherfaces_free_model(DATA_COLLECT* fisher_model);
static void fisherfaces_print_model(DATA_COLLECT* fisher_model);

/*
 * This is an algorithm that was created in the year 1997 by Peter N. Belhumeur, Joao P. Hespanha, and David J. Kriegman.
 * But I have made som improvements to this algorithm.
 * Instead of regular PCA, I have added Kernel PCA.
 * Instead of K-nn, I have included SVM instead.
 * This algorithm has a very high accuracy for classifying images e.g faces or other types of objects.
 * Notice that this algorithm can only classify one object at the time. It has no localization feature.
 * Pros:
 *  - Classify general objects
 *  - Fast
 * Cons:
 *  - Does not offer localization
 *  - Only one object at the time
 */
void fisherfaces(FISHERFACES_SETTINGS* fisherfaces_settings) {
	/*
	 * Parameters for collecting data
	 * I recommend fisherfaces.m file in MataveID for selecting generating the .pgm files
	 */
	printf("\t\t\t\tFisherfaces\n");
	
	/* Check if we need to remove the allocated model */
	if (fisherfaces_settings->is_model_created) {
		printf("0: Deallocate model because it's already existing.\n");
		free(fisherfaces_settings->model_w);
		free(fisherfaces_settings->model_b);
		fisherfaces_settings->model_row = 0;
		fisherfaces_settings->model_column = 0;
		fisherfaces_settings->is_model_created = false;
	}

	/* Collect data */
	printf("1: Collecting data. Reading the .pgm files in row-major. PGM format P2 or P5 format.\n");
	fisherfaces_settings->collect_data_settings.collect_type = COLLECT_TYPE_FISHERFACES;
	DATA_COLLECT* fisher_data = collect_data(&fisherfaces_settings->collect_data_settings);
	const size_t classes = fisher_data->class_id[fisher_data->column - 1] + 1;

	/* Remove outliers */
	if (fisherfaces_settings->remove_outliers) {
		printf("\tOutliers removed: %i\n", cluster_filter(fisher_data->data, fisher_data->row, fisher_data->column, fisherfaces_settings->epsilon, fisherfaces_settings->min_pts));
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
	float* Wpca = (float*)malloc(fisher_data->row * fisherfaces_settings->components_pca * sizeof(float));
	float* Ppca = (float*)malloc(fisherfaces_settings->components_pca * fisher_data->column * sizeof(float));
	kpca(fisher_data->data, Wpca, Ppca, fisherfaces_settings->components_pca, fisher_data->row, fisher_data->column, fisherfaces_settings->kernel_parameters, fisherfaces_settings->kernel_method);

	/*
	 * Parametets for LDA:
	 * components_lda must be classes - 1 because we want to avoid zero eigenvalues
	 */
	printf("3: Do Linear Discriminant Analysis for creating a linear projection for separation of class data.\n");
	const size_t components_lda = classes - 1;
	float* Wlda = (float*)malloc(fisherfaces_settings->components_pca * components_lda * sizeof(float));
	float* Plda = (float*)malloc(components_lda * fisher_data->column * sizeof(float));
	lda(Ppca, fisher_data->class_id, Wlda, Plda, components_lda, fisherfaces_settings->components_pca, fisher_data->column);

	/* Multiply W = Wlda'*Wpca' */
	printf("4: Combine Kernel Principal Component Analysis projection with Linear Discriminant projection.\n");
	float* W = (float*)malloc(components_lda * fisher_data->row * sizeof(float));
	tran(Wlda, fisherfaces_settings->components_pca, components_lda);
	tran(Wpca, fisher_data->row, fisherfaces_settings->components_pca);
	mul(Wlda, Wpca, W, components_lda, fisherfaces_settings->components_pca, fisher_data->row);

	/* Free some matrices that are not needed any longer */
	free(Wpca);
	free(Ppca);
	free(Wlda);
	free(Plda);

	/* Find the total projection */
	printf("5: Find the total projection of the nonlinear data.\n");
	float* P = (float*)malloc(components_lda * fisher_data->column * sizeof(float));
	mul(W, fisher_data->data, P, components_lda, fisher_data->row, fisher_data->column);

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
	tran(P, components_lda, fisher_data->column);
	float* accuracy = (float*)malloc(classes * sizeof(float));
	bool* status = (bool*)malloc(classes * sizeof(bool));
	float* weight = (float*)malloc(classes * components_lda * sizeof(float));
	fisherfaces_settings->model_b = (float*)malloc(classes * sizeof(float));
	float* model_b = fisherfaces_settings->model_b;
	nn_train(P, fisher_data->class_id, weight, model_b, status, accuracy, fisher_data->column, components_lda, fisherfaces_settings->C, fisherfaces_settings->lambda);

	/* Free */
	free(status);
	free(P);
	free(accuracy);

	/*
	 * Multiply model_w = weight * W
	 * y = model_w*imagevector + model_b is our model.
	 * The index of the largest value of vector y is the class ID of imagevector
	 */
	printf("7: Creating the model for nonlinear data.\n");
	fisherfaces_settings->model_w = (float*)malloc(classes * fisher_data->row * sizeof(float));
	float* model_w = fisherfaces_settings->model_w;
	mul(weight, W, model_w, classes, components_lda, fisher_data->row);

	/* Free */
	free(W);
	free(weight);

	/* Save the row and column parameters */
	fisherfaces_settings->model_row = classes;
	fisherfaces_settings->model_column = fisher_data->row;
	fisherfaces_settings->is_model_created = true;

	/* Check the accuracy of the model */
	tran(fisher_data->data, fisher_data->row, fisher_data->column);
	nn_eval(model_w, model_b, fisher_data->data, fisher_data->class_id, classes, fisher_data->row, fisher_data->column);

	/* Save wW and b as a function */
	printf("8: Saving the model to a .h file.\n");
	if (fisherfaces_settings->save_model) {
		char model_path[260];
		char model_name[100];
		char mode_name_h[100];
		printf("Enter a proper model name e.g my_model_6 or MyModel(max 100 letters): ");
		scanf("%s", model_name);
		sprintf(mode_name_h, "%s.h", model_name);
		concatenate_paths(model_path, fisherfaces_settings->collect_data_settings.folder_path, mode_name_h);
		nn_save(fisherfaces_settings->model_w, fisherfaces_settings->model_b, model_path, model_name, classes, fisher_data->row);
	}
	else {
		printf("\tNo...\n");
	}

	/* Free - The model is not needed any more */
	fisherfaces_free_model(fisher_data);

	/* End */
	printf("9: Everything is done...\n");
}

static void fisherfaces_free_model(DATA_COLLECT* fisher_model) {
	if (fisher_model) {
		free(fisher_model->class_id);
		free(fisher_model->data);
		free(fisher_model);
	}
}

static void fisherfaces_print_model(DATA_COLLECT* fisher_model) {
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
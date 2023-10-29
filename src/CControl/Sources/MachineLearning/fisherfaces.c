/*
 * fisherfaces.c
 *
 *  Created on: 27 mars 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

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
DATA_COLLECT* fisherfaces(DATA_SETTINGS* settings) {
	/* Header */
	printf("\t\t\t\tFisherfaces\n");
	
	/* Collect data */
	printf("1: Collecting data. Reading the .pgm files in row-major. PGM format P2 or P5 format.\n");
	settings->collect_type = COLLECT_TYPE_FISHERFACES;
	DATA_COLLECT* data_collect = collect_data(settings);

	/* Remove outliers */
	if (settings->remove_outliers) {
		printf("\tOutliers removed: %i\n", cluster_filter(data_collect->input, data_collect->row_input, data_collect->column, settings->epsilon, settings->min_pts));
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
	float* Wpca = (float*)malloc(data_collect->row_input * settings->components_pca * sizeof(float));
	float* Ppca = (float*)malloc(settings->components_pca * data_collect->column * sizeof(float));
	kpca(data_collect->input, Wpca, Ppca, settings->components_pca, data_collect->row_input, data_collect->column, settings->kernel_parameters, settings->kernel_method);

	/*
	 * Parametets for LDA:
	 * components_lda must be classes - 1 because we want to avoid zero eigenvalues
	 */
	printf("3: Do Linear Discriminant Analysis for creating a linear projection for separation of class data.\n");
	const size_t components_lda = data_collect->classes - 1;
	float* Wlda = (float*)malloc(settings->components_pca * components_lda * sizeof(float));
	float* Plda = (float*)malloc(components_lda * data_collect->column * sizeof(float));
	lda(Ppca, data_collect->class_id, Wlda, Plda, components_lda, settings->components_pca, data_collect->column);

	/* Multiply W = Wlda'*Wpca' */
	printf("4: Combine Kernel Principal Component Analysis projection with Linear Discriminant projection.\n");
	float* W = (float*)malloc(components_lda * data_collect->row_input * sizeof(float));
	tran(Wlda, settings->components_pca, components_lda);
	tran(Wpca, data_collect->row_input, settings->components_pca);
	mul(Wlda, Wpca, W, components_lda, settings->components_pca, data_collect->row_input);

	/* Free some matrices that are not needed any longer */
	free(Wpca);
	free(Ppca);
	free(Wlda);
	free(Plda);

	/* Find the total projection */
	printf("5: Find the total projection of the nonlinear data.\n");
	float* P = (float*)malloc(components_lda * data_collect->column * sizeof(float));
	mul(W, data_collect->input, P, components_lda, data_collect->row_input, data_collect->column);

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
	tran(P, components_lda, data_collect->column);
	float* accuracy = (float*)malloc(data_collect->classes * sizeof(float));
	bool* status = (bool*)malloc(data_collect->classes * sizeof(bool));
	float* weight = (float*)malloc(data_collect->classes * components_lda * sizeof(float));
	data_collect->model_b = (float*)malloc(data_collect->classes * sizeof(float));
	nn_train(P, data_collect->class_id, weight, data_collect->model_b, status, accuracy, data_collect->column, components_lda, settings->C, settings->lambda);

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
	data_collect->model_w = (float*)malloc(data_collect->classes * data_collect->row_input * sizeof(float));
	float* model_w = data_collect->model_w;
	mul(weight, W, model_w, data_collect->classes, components_lda, data_collect->row_input);

	/* Free */
	free(W);
	free(weight);

	/* Save the row and column parameters */
	data_collect->model_row = data_collect->classes;
	data_collect->model_column = data_collect->row_input;
	data_collect->is_model_created = true;

	/* Check the accuracy of the model */
	tran(data_collect->input, data_collect->row_input, data_collect->column);
	nn_eval(model_w, data_collect->model_b, data_collect->input, data_collect->output, data_collect->class_id, data_collect->classes, data_collect->row_input, data_collect->column);

	/* Save wW and b as a function */
	printf("8: Saving the model to a .h file.\n");
	if (settings->save_model) {
		char model_path[260];
		char model_name[100];
		char mode_name_h[100];
		printf("Enter a proper model name e.g my_model_6 or MyModel(max 100 letters): ");
		scanf("%s", model_name);
		sprintf(mode_name_h, "%s.h", model_name);
		concatenate_paths(model_path, settings->folder_path, mode_name_h);
		nn_save(data_collect->model_w, data_collect->model_b, model_path, model_name, data_collect->classes, data_collect->row_input);
	}
	else {
		printf("\tNo...\n");
	}

	/* End */
	printf("9: Everything is done...\n");

	/* Return data */
	return data_collect;
}
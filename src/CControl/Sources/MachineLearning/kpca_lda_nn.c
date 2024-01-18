/*
 * kpca_lda_nn.c
 *
 *  Created on: 19 November 2023
 *      Author: Daniel Mårtensson
 */

#include "machinelearning.h"

 /*
  * This is an algorithm that was created in the year 1997 by Peter N. Belhumeur, Joao P. Hespanha, and David J. Kriegman.
  * But I have made som improvements to this algorithm.
  * Instead of regular PCA, I have added Kernel PCA.
  * Instead of K-nn, I have included SVM instead.
  * This algorithm has a very high accuracy for classifying images e.g faces or other types of objects.
  * Notice that this algorithm can only classify one object at the time. It has no localization feature.
  * Pros:
  *  - Classify general objects
  *  - Fast (Only linear algebra)
  * Cons:
  *  - Does not offer localization
  *  - Only one object at the time
  * 
  * NOTICE: The input data must be in row-major
  */
void kpca_lda_nn(MODEL_NN* model_nn, const MODEL_SETTINGS* model_settings) {
	/* Extract */
	MODEL_NN_SETTINGS* general_settings = NULL;
	switch (model_settings->model_choice) {
	case MODEL_CHOICE_FISHERFACES:
		general_settings = &model_settings->settings_fisherfaces;
		break;
	}

	/* Transpose becase it's much better to have row > column */
	tran(model_nn->input, model_nn->input_row, model_nn->input_column);
	size_t k = model_nn->input_row;
	model_nn->input_row = model_nn->input_column;
	model_nn->input_column = k;

	/* Remove outliers */
	if (general_settings->remove_outliers) {
		printf("\tOutliers removed: %i\n", clusterfilter(model_nn->input, model_nn->input_row, model_nn->input_column, general_settings->epsilon, general_settings->min_pts));
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
	float* Wpca = (float*)malloc(model_nn->input_row * general_settings->components_pca * sizeof(float));
	float* Ppca = (float*)malloc(general_settings->components_pca * model_nn->input_column * sizeof(float));
	kpca(model_nn->input, Wpca, Ppca, general_settings->components_pca, model_nn->input_row, model_nn->input_column, general_settings->kernel_parameters, general_settings->kernel_method);

	/*
	 * Parametets for LDA:
	 * components_lda must be classes - 1 because we want to avoid zero eigenvalues
	 */
	printf("3: Do Linear Discriminant Analysis for creating a linear projection for separation of class data.\n");
	const uint8_t components_lda = model_nn->classes - 1;
	float* Wlda = (float*)malloc(general_settings->components_pca * components_lda * sizeof(float));
	float* Plda = (float*)malloc(components_lda * model_nn->input_column * sizeof(float));
	lda(Ppca, model_nn->class_id, Wlda, Plda, components_lda, general_settings->components_pca, model_nn->input_column);

	/* Multiply W = Wlda'*Wpca' */
	printf("4: Combine Kernel Principal Component Analysis projection with Linear Discriminant projection.\n");
	float* W = (float*)malloc(components_lda * model_nn->input_row * sizeof(float));
	tran(Wlda, general_settings->components_pca, components_lda);
	tran(Wpca, model_nn->input_row, general_settings->components_pca);
	mul(Wlda, Wpca, W, components_lda, general_settings->components_pca, model_nn->input_row);

	/* Free some matrices that are not needed any longer */
	free(Wpca);
	free(Ppca);
	free(Wlda);
	free(Plda);

	/* Find the total projection */
	printf("5: Find the total projection of the nonlinear data.\n");
	float* P = (float*)malloc(components_lda * model_nn->input_column * sizeof(float));
	mul(W, model_nn->input, P, components_lda, model_nn->input_row, model_nn->input_column);

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
	tran(P, components_lda, model_nn->input_column);
	float* accuracy = (float*)malloc(model_nn->classes * sizeof(float));
	bool* status = (bool*)malloc(model_nn->classes * sizeof(bool));
	float* weight = (float*)malloc(model_nn->classes * components_lda * sizeof(float));
	model_nn->model_b[0] = (float*)malloc(model_nn->classes * sizeof(float));
	nn_train(P, model_nn->class_id, weight, model_nn->model_b[0], status, accuracy, model_nn->input_column, components_lda, model_nn->classes, general_settings->C, general_settings->lambda);

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
	model_nn->model_w[0] = (float*)malloc(model_nn->classes * model_nn->input_row * sizeof(float));
	mul(weight, W, model_nn->model_w[0], model_nn->classes, components_lda, model_nn->input_row);

	/* Free */
	free(W);
	free(weight);

	/* Save the row and column parameters */
	model_nn->model_row[0] = model_nn->classes;
	model_nn->model_column[0] = model_nn->input_row;
	model_nn->activation_function[0] = ACTIVATION_FUNCTION_HIGHEST_VALUE_INDEX;
	model_nn->total_models = 1;

	/* Check the accuracy of the model */
	tran(model_nn->input, model_nn->input_row, model_nn->input_column);
	float* y = (float*)malloc(model_nn->input_column * model_nn->classes * sizeof(float));
	nn_eval(model_nn->model_w[0], model_nn->model_b[0], model_nn->input, y, model_nn->class_id, model_nn->classes, model_nn->input_row, model_nn->input_column, ACTIVATION_FUNCTION_HIGHEST_VALUE_INDEX);
	free(y);

	/* Save wW and b as a function */
	printf("8: Saving the model to a .h file.\n");
	if (general_settings->save_model) {
		char model_path[260];
		char model_name[100];
		char model_name_h[100];
		char model_name_text[100];
		printf("Enter a model name: ");
		scanf("%s", model_name);
		uint8_t i;
		for (i = 0; i < model_nn->total_models; i++) {
			sprintf(model_name_h, "%s_%i.h", model_name, i);
			sprintf(model_name_text, "%s_%i", model_name, i);
			concatenate_paths(model_path, general_settings->folder_path, model_name_h);
			nn_save(model_nn->model_w[i], model_nn->model_b[i], model_nn->activation_function[i], model_path, model_name_text, model_nn->model_row[i], model_nn->model_column[i]);
		}
	}
	else {
		printf("\tNo...\n");
	}

	/* End */
	printf("9: Everything is done...\n");
}
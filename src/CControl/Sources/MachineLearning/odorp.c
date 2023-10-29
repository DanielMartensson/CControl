/*
 * odorp.c
 *
 *  Created on: 28 oktober 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/*
 * This algorithm is created by Daniel Mårtensson in the year 2023.
 * This algorithm uses one linear neural network for classifying descriptors from interest points.
 * The purpose with this algorithm is to 
 * * Pros:
 *  - Offer localization
 *  - Multiple object at the time
 *  - Fast
 * Cons:
 *  - Classify specific objects
 */
DATA_COLLECT* odorp(DATA_SETTINGS* settings) {
	/* Header */
	printf("\t\t\t\tObject Detection Oriented FAST Rotated Pattern\n");

	/* Collect data */
	settings->collect_type = COLLECT_TYPE_ORP;
	printf("1: Collecting data. Reading the .pgm files in row-major. PGM format P2 or P5 format.\n");
	DATA_COLLECT* data_collect = collect_data(settings);
	
	/*
	 * Train Neural Network model of the total projection
	 * C is a tuning parameter
	 * lambda is a tuning parameter
	 * Important to turn P into transpose because we are going to use that as vectors ontop on each other
	 * Example:
	 * float C = 1;
	 * float lambda = 2.5;
	 */
	printf("2: Create a Neural Network for a linear model.\n");
	float* accuracy = (float*)malloc(data_collect->classes * sizeof(float));
	bool* status = (bool*)malloc(data_collect->classes * sizeof(bool));
	data_collect->model_w = (float*)malloc(data_collect->classes * data_collect->column * sizeof(float));
	data_collect->model_b = (float*)malloc(data_collect->classes * sizeof(float));
	nn_train(data_collect->input, data_collect->class_id, data_collect->model_w, data_collect->model_b, status, accuracy, data_collect->row_input, data_collect->column, settings->C, settings->lambda);

	/* Free */
	free(status);
	free(accuracy);

	/* Save the row and column parameters */
	data_collect->model_row = data_collect->classes;
	data_collect->model_column = data_collect->column;
	data_collect->is_model_created = true;

	/* Check the accuracy of the model */
	nn_eval(data_collect->model_w, data_collect->model_b, data_collect->input, data_collect->output, data_collect->class_id, data_collect->classes, data_collect->column, data_collect->row_input);

	/* Save model */
	printf("3: Saving the model to a .h file.\n");
	if (settings->save_model) {
		char model_path[260];
		char model_name[100];
		char mode_name_h[100];
		printf("Enter a proper model name e.g my_model_6 or MyModel(max 100 letters): ");
		scanf("%s", model_name);
		sprintf(mode_name_h, "%s.h", model_name);
		concatenate_paths(model_path, settings->folder_path, mode_name_h);
		nn_save(data_collect->model_w, data_collect->model_b, model_path, model_name, data_collect->classes, data_collect->column);
	}
	else {
		printf("\tNo...\n");
	}

	/* End */
	printf("4: Everything is done...\n");

	/* Return data */
	return data_collect;
}
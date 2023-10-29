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
void odorp(ODORP_SETTINGS* odorp_settings) {
	/* Header */
	printf("\t\t\t\tObject Detection Oriented FAST Rotated Pattern\n");

	/* Check if we need to remove the allocated model */
	if (odorp_settings->is_model_created) {
		printf("0: Deallocate model because it's already existing.\n");
		free(odorp_settings->model_w);
		free(odorp_settings->model_b);
		odorp_settings->model_row = 0;
		odorp_settings->model_column = 0;
		odorp_settings->is_model_created = false;
	}

	/* Collect data */
	printf("1: Collecting data. Reading the .pgm files in row-major. PGM format P2 or P5 format.\n");
	odorp_settings->collect_data_settings.collect_type = COLLECT_TYPE_ORP;
	DATA_COLLECT* odorp_data = collect_data(&odorp_settings->collect_data_settings);
	const size_t classes = odorp_data->class_id[odorp_data->row - 1] + 1;

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
	float* accuracy = (float*)malloc(classes * sizeof(float));
	bool* status = (bool*)malloc(classes * sizeof(bool));
	odorp_settings->model_w = (float*)malloc(classes * odorp_data->column * sizeof(float));
	odorp_settings->model_b = (float*)malloc(classes * sizeof(float));
	nn_train(odorp_data->data, odorp_data->class_id, odorp_settings->model_w, odorp_settings->model_b, status, accuracy, odorp_data->row, odorp_data->column, odorp_settings->C, odorp_settings->lambda);

	/* Free */
	free(status);
	free(accuracy);

	/* Save the row and column parameters */
	odorp_settings->model_row = classes;
	odorp_settings->model_column = odorp_data->column;
	odorp_settings->is_model_created = true;

	/* Check the accuracy of the model */
	nn_eval(odorp_settings->model_w, odorp_settings->model_b, odorp_data->data, odorp_data->class_id, classes, odorp_data->column, odorp_data->row);

	/* Save wW and b as a function */
	printf("8: Saving the model to a .h file.\n");
	if (odorp_settings->save_model) {
		char model_path[260];
		char model_name[100];
		char mode_name_h[100];
		printf("Enter a proper model name e.g my_model_6 or MyModel(max 100 letters): ");
		scanf("%s", model_name);
		sprintf(mode_name_h, "%s.h", model_name);
		concatenate_paths(model_path, odorp_settings->collect_data_settings.folder_path, mode_name_h);
		nn_save(odorp_settings->model_w, odorp_settings->model_b, model_path, model_name, classes, odorp_data->column);
	}
	else {
		printf("\tNo...\n");
	}

	/* Free - The model is not needed any more */
	collect_data_free(odorp_data);

	/* End */
	printf("9: Everything is done...\n");
}
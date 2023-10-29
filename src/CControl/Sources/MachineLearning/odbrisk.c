/*
 * odbrisk.c
 *
 *  Created on: 28 oktober 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/* Private functions */
static void odbrisk_free_model(DATA_COLLECT* fisher_model);

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
void odbrisk(ODBRISK_SETTINGS* odbrisk_settings) {
	/*
	 * Parameters for collecting data
	 * I recommend odbrisk.m file in MataveID for selecting generating the .pgm files
	 */
	printf("\t\t\t\tObject Detection BRISK\n");

	/* Check if we need to remove the allocated model */
	if (odbrisk_settings->is_model_created) {
		printf("0: Deallocate model because it's already existing.\n");
		free(odbrisk_settings->model_w);
		free(odbrisk_settings->model_b);
		odbrisk_settings->model_row = 0;
		odbrisk_settings->model_column = 0;
		odbrisk_settings->is_model_created = false;
	}

	/* Collect data */
	printf("1: Collecting data. Reading the .pgm files in row-major. PGM format P2 or P5 format.\n");
	odbrisk_settings->collect_data_settings.collect_type = COLLECT_TYPE_ODBRISK;
	DATA_COLLECT* odbrisk_data = collect_data(&odbrisk_settings->collect_data_settings);
	const size_t classes = odbrisk_data->class_id[odbrisk_data->row - 1] + 1;

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
	odbrisk_settings->model_w = (float*)malloc(classes * odbrisk_data->column * sizeof(float));
	odbrisk_settings->model_b = (float*)malloc(classes * sizeof(float));
	nn_train(odbrisk_data->data, odbrisk_data->class_id, odbrisk_settings->model_w, odbrisk_settings->model_b, status, accuracy, odbrisk_data->row, odbrisk_data->column, odbrisk_settings->C, odbrisk_settings->lambda);

	/* Free */
	free(status);
	free(accuracy);

	/* Save the row and column parameters */
	odbrisk_settings->model_row = classes;
	odbrisk_settings->model_column = odbrisk_data->column;
	odbrisk_settings->is_model_created = true;

	/* Check the accuracy of the model */
	nn_eval(odbrisk_settings->model_w, odbrisk_settings->model_b, odbrisk_data->data, odbrisk_data->class_id, classes, odbrisk_data->column, odbrisk_data->row);

	/* Save wW and b as a function */
	printf("8: Saving the model to a .h file.\n");
	if (odbrisk_settings->save_model) {
		char model_path[260];
		char model_name[100];
		char mode_name_h[100];
		printf("Enter a proper model name e.g my_model_6 or MyModel(max 100 letters): ");
		scanf("%s", model_name);
		sprintf(mode_name_h, "%s.h", model_name);
		concatenate_paths(model_path, odbrisk_settings->collect_data_settings.folder_path, mode_name_h);
		nn_save(odbrisk_settings->model_w, odbrisk_settings->model_b, model_path, model_name, classes, odbrisk_data->column);
	}
	else {
		printf("\tNo...\n");
	}

	/* Free - The model is not needed any more */
	odbrisk_free_model(odbrisk_data);

	/* End */
	printf("9: Everything is done...\n");

}

static void odbrisk_free_model(DATA_COLLECT* odbrisk_model) {
	if (odbrisk_model) {
		free(odbrisk_model->class_id);
		free(odbrisk_model->data);
		free(odbrisk_model);
	}
}
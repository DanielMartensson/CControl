/*
 * odorp.c
 *
 *  Created on: 28 oktober 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/* Private functions */
static void odorp_save(const uint64_t data[], const uint8_t class_id[], const uint8_t classes, const char model_path[], const char model_name[], const size_t N);

 /* Private variable */
static bool has_been_called = false;

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
MODEL* odorp(MODEL_SETTINGS* model_settings) {
	/* Header */
	printf("\t\t\t\tObject Detection Oriented FAST Rotated Pattern\n");

	/* Collect data */
	printf("1: Collecting data. Reading the .pgm files in row-major. PGM format P2 or P5 format.\n");
	model_settings->model_choice = MODEL_CHOICE_ODORP;
	MODEL* model = imcollect(model_settings);

	/* Print information */
	printf("2: Print information\n");
	printf("\tTotal classes: %i\n", model->odorp_model.classes);
	printf("\tHow many descriptors: %lli\n", model->odorp_model.N);
	size_t i, counter = 0;
	for (i = 0; i < model->odorp_model.N; i++) {
		if (i > 1) {
			if (model->odorp_model.class_id[i] - model->odorp_model.class_id[i - 1] > 0) {
				printf("\tDescriptors for class %i are: %lli\n", model->odorp_model.class_id[i - 1], counter);
				counter = 0;
			}
		}
		counter++;
	}
	printf("\tDescriptors for class %i are: %lli\n", model->odorp_model.class_id[i - 1], counter);

	/* Save matrix */
	printf("3: Save matrix\n");
	if (model_settings->data_settings_odorp.save_model) {
		char model_path[260];
		char model_name[100];
		char model_name_h[100];
		printf("Enter a model name: ");
		scanf("%s", model_name);
		sprintf(model_name_h, "%s.h", model_name);
		concatenate_paths(model_path, model_settings->data_settings_odorp.folder_path, model_name_h);
		odorp_save(model->odorp_model.data, model->odorp_model.class_id, model->odorp_model.classes, model_path, model_name, model->odorp_model.N);
	}
	else {
		printf("\tNo...\n");
	}

	/* End */
	printf("4: Everything is done...\n");

	/* Return model */
	return model;
}

/*
 * Save a matrix
 */
static void odorp_save(const uint64_t data[], const uint8_t class_id[], const uint8_t classes, const char model_path[], const char model_name[], const size_t N) {
	/* Get time */
	time_t t = time(NULL);

	/* Flag for the random number generator */
	if (!has_been_called) {
		srand(t);
		has_been_called = true;
	}

	FILE* file = fopen(model_path, "w");
	if (file) {
		fprintf(file, "/*\n");
		fprintf(file, " * %s.h\n", model_name);
		fprintf(file, " *\n");
		struct tm tm = *localtime(&t);
		fprintf(file, " * Created on : %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		fprintf(file, " * Author: <Your name>\n");
		fprintf(file, " * Purpose: <Classify <your things>\n");
		fprintf(file, " * Model ID: %i\n", rand());
		fprintf(file, " */\n\n");
		fprintf(file, "#ifndef %s_H_\n", model_name);
		fprintf(file, "#define %s_H_\n\n", model_name);
		fprintf(file, "/* Add the CControl header file */\n");
		fprintf(file, "#include \"ccontrol.h\"\n\n");
		fprintf(file, "/*\n");
		fprintf(file, " * This is a K-nearest neighbor - A model that can classify raw data.\n");
		fprintf(file, " * Most used for descriptors, but you can use it at any data you want as long the data is stored into .pgm files in format P2 or P5.\n");
		fprintf(file, " * This file contains two vectors called '%s_data' and '%s_class_id'.\n", model_name, model_name);
		fprintf(file, " * To use this header file, your data that must be a vector of size '%s_N'\n", model_name);
		fprintf(file, " * For example, if you have an descriptors with length m, where m = '%s_N', then you can XOR-multiply the data with your data.\n", model_name);
		fprintf(file, " * The smallest number is the class ID\n");
		fprintf(file, " */\n\n");
		fprintf(file, "#define %s_N %i\n", model_name, (int)N);
		fprintf(file, "#define %s_classes %i\n", model_name, (int)classes);
		fprintf(file, "\n");
		fprintf(file, "const static uint64_t %s_data[%s_N] = { ", model_name, model_name);
		size_t i, count = 0, max_index_model_w = N - 1;
		for (i = 0; i < N; i++) {
			if (count < max_index_model_w) {
				fprintf(file, "0x%llXU, ", data[i]);
			}
			else {
				fprintf(file, "0x%llXU };\n", data[i]);
			}
			count++;
			fprintf(file, "\n");
		}
		fprintf(file, "const static uint8_t %s_class_id[%s_N] = { ", model_name, model_name);
		count = 0;
		for (i = 0; i < N; i++) {
			if (count < max_index_model_w) {
				fprintf(file, "%iU,", (int)class_id[i]);
			}
			else {
				fprintf(file, "%iU };\n", (int)class_id[i]);
			}
			count++;
			fprintf(file, "\n");
		}
		
		fprintf(file, "#endif /* !%s_H_ */\n", model_name);
		fclose(file);
		printf("\tModel as been saved: %s\n", model_path);
	}
	else {
		printf("\tFile %s.h could not be open. Check your writing rights at the file %s.\n", model_name, model_path);
	}
}
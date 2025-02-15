/*
 * nn.c
 *
 *  Created on: 19 Augusti 2023
 *      Author: Daniel Mårtensson
 */

#include "machinelearning.h"

/* Return the index of the closest to +1 value of vector x */
static size_t closest_value_index(float x[], size_t length, bool* class_id_found) {
	size_t i, max_index = 0;
	float t, s = FLT_MAX;
	*class_id_found = false;
	for (i = 0; i < length; i++) {
		t = fabsf(x[i] - 1.0f);
		if (t < s && t <= 1.0f) {
			s = t;
			max_index = i;
			*class_id_found = true;
		}
	}
	return max_index;
}

/* Return the index of the largest value of vector x */
static size_t highest_value_index(float x[], size_t length, bool* class_id_found) {
	size_t max_index;
	*class_id_found = amax(x, &max_index, length) >= 0.0f ? true : false;
	return max_index;
}

/* Private fields */
static bool has_been_called = false;

/*
 * This is a neural network. It's not a deep neural network.
 * If you want to use Deep Learning, just use multiple neural networks.
 * This neural network is special. Here I'm using Support Vector Machine
 * to train a neural network. The reason why is that Support Vector Machine
 * is using convex optimization a.k.a Quadratic Programming, and it gives a very good result
 * with small data. For example, with this technique, Ronald Fisher's Iris dataset, can give a
 * result of 96.7% accuracy with just one neural network with a nonlinear activation function.
 * Even if the Iris dataset (contains three classes), have two classes that are clustred together.
 *
 * The neural network is:
 * y = weight*x + bias
 *
 * Where the index of the maximum value of vector y, is the class ID for unknown data vector x
 *
 * X[m*n]
 * class_id[m]
 * weight[classes*n]
 * bias[classes]
 * status[classes]
 * accuracy[classes]
 */
void nn_train(const float X[], const uint8_t class_id[], float weight[], float bias[], STATUS_CODES status[], float accuracy[], const size_t row, const size_t column, const uint8_t classes, const float C, const float lambda) {
	size_t i, j;
	float* labels = (float*)malloc(row * sizeof(float));
	for (i = 0; i < classes; i++) {
		/* Tune in class ID */
		for (j = 0; j < row; j++) {
			/* Get the class ID */
			if (i == class_id[j]) {
				labels[j] = 1.0f;
			}
			else {
				labels[j] = -1.0f;
			}
		}
		/* Compute the model parameters w and bias b */
		status[i] = svm(X, labels, weight + column * i, &bias[i], &accuracy[i], C, lambda, row, column);

		/* Print the result */
		printf("Class %i: Success: %s. The accuracy = %f\n", i, status[i] == STATUS_OK ? "yes" : "no", accuracy[i]*100.0f);
	}

	/* Free*/
	free(labels);
}

/* 
 * Predict the class id
 * model_w[row_w * column_w]
 * model_b[row_w]
 * x[column_w]
 * y[row_w]
 * Return the class ID = sigma(model_w*x + model_b)
 */
uint8_t nn_predict(const float model_w[], const float model_b[], const float x[], float y[], const size_t row_w, const size_t column_w, bool* class_id_found, const ACTIVATION_FUNCTION activation_function) {
	/* Compute y = W*x + b */
	mul(model_w, x, y, row_w, column_w, 1);
	size_t i;
	for (i = 0; i < row_w; i++) {
		y[i] += model_b[i];
	}

	/* Get the index of the largest value of y */
	switch (activation_function) {
	case ACTIVATION_FUNCTION_HIGHEST_VALUE_INDEX:
		return highest_value_index(y, row_w, class_id_found);
	case ACTIVATION_FUNCTION_CLOSEST_VALUE_INDEX:
		return closest_value_index(y, row_w, class_id_found);
	default:
		*class_id_found = false;
		return 0U;
	}
}

/*
 * Evaluate the model 
 * model_w[row_w * column_w]
 * model_b[row_w]
 * X[row_x * column_w]
 * Y[row_x * row_w]
 * class_id[row_x]
 */
void nn_eval(const float model_w[], const float model_b[], const float X[], float Y[], const uint8_t class_id[], const size_t row_w, const size_t column_w, const size_t row_x, const ACTIVATION_FUNCTION activation_function) {
	size_t i, predicted_correctly_score = 0, predicted_wrongy_score = 0, unpredicted_score = 0;
	bool class_id_found;
	for (i = 0; i < row_x; i++) {
		uint8_t class_id_predicted = nn_predict(model_w, model_b, X + column_w * i, Y + row_w * i, row_w, column_w, &class_id_found, activation_function);
		if (class_id_found) {
			if (class_id[i] == class_id_predicted) {
				predicted_correctly_score++;
			}
			else {
				predicted_wrongy_score++;
			}
		}
		else {
			unpredicted_score++;
		}
	}

	/* Print status */
	printf("The correctly predicted accuracy of the neural network is: %f (%i of %i)\n", ((float)predicted_correctly_score) / ((float)row_x) * 100.0f, predicted_correctly_score, row_x);
	printf("The wrongly predicted accuracy of the neural network is: %f (%i of %i)\n", ((float)predicted_wrongy_score) / ((float)row_x) * 100.0f, predicted_wrongy_score, row_x);
	printf("The non-predicted accuracy of the neural network is: %f (%i of %i)\n", ((float)unpredicted_score) / ((float)row_x) * 100.0f, unpredicted_score, row_x);
	printf("The totalt accuracy of the neural network is: %f (%i of %i)\n", ((float)predicted_correctly_score + unpredicted_score) / ((float)row_x) * 100.0f, predicted_correctly_score + unpredicted_score, row_x);
}

/*
 * Save a neural network into a .h file
 * model_w[m*n]
 * model_b[m]
 */
void nn_save(const float model_w[], const float model_b[], const ACTIVATION_FUNCTION activation_function, const char model_path[], const char model_name[], const size_t row, const size_t column) {
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
		fprintf(file, " * This is a Neural Network - A model that can classify raw data.\n");
		fprintf(file, " * Most used for images, but you can use it at any data you want as long the data is stored into .pgm files in format P2 or P5.\n");
		fprintf(file, " * This file contains one matrix called '%s_w' and one vector called '%s_b'.\n", model_name, model_name);
		fprintf(file, " * To use this header file, your data that must be a vector of size '%s_column' in row-major.\n", model_name);
		fprintf(file, " * For example, if you have an image with size m * n, where m * n = '%s_column', then you need to turn it into a vector in row-wise, not in column-wise.\n", model_name);
		fprintf(file, " * Multiply your vector together with the matrix '%s_w' and add vector '%s_b'. Then you will get a new vector called y with the size '%s_row'.\n", model_name, model_name, model_name);
		fprintf(file, " * y = %s_w*your_data_vector + %s_b\n", model_name, model_name);
		fprintf(file, " * The index of the largest number inside the vector y is the class ID of your_data_vector.\n");
		fprintf(file, " * If you get wrong class ID, then you need to use take account to noise or use better dimension reduction.\n");
		fprintf(file, " */\n\n");
		fprintf(file, "#define %s_row %i\n", model_name, (int)row);
		fprintf(file, "#define %s_column %i\n", model_name, (int)column);
		fprintf(file, "\n");
		fprintf(file, "const static float %s_w[%s_row * %s_column] = { ", model_name, model_name, model_name);
		size_t i, j, count = 0, max_index_model_w = row * column - 1;
		for (i = 0; i < row; i++) {
			for (j = 0; j < column; j++) {
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
		fprintf(file, "const static float %s_b[%s_row] = { ", model_name, model_name);
		for (i = 0; i < row; i++) {
			if (i < row - 1) {
				fprintf(file, "%0.9ef, ", model_b[i]);
			}
			else {
				fprintf(file, "%0.9ef };\n", model_b[i]);
			}
		}
		fprintf(file, "\n");
		fprintf(file, "const static ACTIVATION_FUNCTION %s_activtion_function = ", model_name);
		switch (activation_function) {
		case ACTIVATION_FUNCTION_HIGHEST_VALUE_INDEX:
			fprintf(file, "ACTIVATION_FUNCTION_HIGHEST_VALUE_INDEX;\n\n");
			break;
		case ACTIVATION_FUNCTION_CLOSEST_VALUE_INDEX:
			fprintf(file, "ACTIVATION_FUNCTION_CLOSEST_VALUE_INDEX;\n\n");
			break;
		}
		fprintf(file, "#endif /* !%s_H_ */\n", model_name);
		fclose(file);
		printf("\tModel as been saved: %s\n", model_path);
	}
	else {
		printf("\tFile %s.h could not be open. Check your writing rights at the file %s.\n", model_name, model_path);
	}
}
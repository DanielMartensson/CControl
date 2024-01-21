/*
 * adaboost.c
 *
 *  Created on: 20 Januari 2024
 *      Author: Daniel Mårtensson
 */

#include "machinelearning.h"

 /*
  * Adaptive Boosting
  * X[m*n] - Numerical variables - Input/Output
  * index[m] - Index of the X - Output
  * labels[m] - Lables - Input
  * is_numerical[n] - Flags if the columns in X is numerical or binary 1/0
  */
bool adaboost(const float X[], const bool is_numerical[], const bool labels[], const size_t row, const size_t column) {	
	/* Create vectors */
	float* x = (float*)malloc(row * sizeof(float));
	float* weights = (float*)malloc(row * sizeof(float));

	/* This will only reset the srand seed */
	randn(x, row, 0.0f, 1.0f);

	/* Create a row tracker */
	size_t* row_correct_tracker = (size_t*)malloc(row * sizeof(size_t));
	size_t* row_incorrect_tracker = (size_t*)malloc(row * sizeof(size_t));

	/* Copy X */
	float* X_copy = (float*)malloc(row * column * sizeof(float));
	memcpy(X_copy, X, row * column * sizeof(float));

	/* Create holders */
	float* X_new = (float*)malloc(row * column * sizeof(float));

	/* The sum of amount of say will determine if it's positive classified or not */
	float sum_amout_of_say_positive = 0.0f;
	float sum_amout_of_say_negative = 0.0f;

	/* Do AdaBoost */
	size_t iteration;
	for (iteration = 0; iteration < row; iteration++) {
		/* Give the samples the same weight */
		size_t i, j = 0;
		const float same_weight = 1.0f / ((float)row);
		for (i = 0; i < row; i++) {
			weights[i] = same_weight;
		}

		/* Find gini index that separate most */
		float lowest_gini_index = 1.0f;
		float numerical_threshold = 0.0f;
		for (i = 0; i < column; i++) {
			/* Extract one column of X */
			cut(X_copy, column, x, 0, row - 1, i, i);

			/* Compute gini index */
			float gini_index = giniimpurity(x, is_numerical[i], labels, &numerical_threshold, row);
			if (gini_index < lowest_gini_index) {
				lowest_gini_index = gini_index;
				j = i;
			}
		}

		/* Extract the column column of X that has the lowest gini index */
		cut(X_copy, column, x, 0, row - 1, j, j);

		/* Find the total error */
		float true_correct = 0.0f;
		float true_incorrect = 0.0f;
		float false_correct = 0.0f;
		float false_incorrect = 0.0f;
		const float threshold = is_numerical[j] ? numerical_threshold : 0.5f;
		size_t counter_correct = 0, counter_incorrect = 0;
		for (i = 0; i < row; i++) {
			if (x[i] > threshold) {
				if (labels[i]) {
					true_correct++;
					row_correct_tracker[counter_correct++] = i;
				}
				else {
					true_incorrect++;
					row_incorrect_tracker[counter_incorrect++] = i;
				}
			}
			else {
				if (!labels[i]) {
					false_correct++;
					row_correct_tracker[counter_correct++] = i;
				}
				else {
					false_incorrect++;
					row_incorrect_tracker[counter_incorrect++] = i;
				}
			}
		}

		/* Compute the total error */
		float total_error = (true_incorrect + false_incorrect) / (true_incorrect + false_incorrect + true_correct + false_correct);

		/* Check if total error is equal to 0 */
		if (fabsf(total_error) < FLT_EPSILON) {
			total_error = FLT_EPSILON;
		}

		/* Check if total error is equal to 1 */
		if (fabsf(total_error - 1.0f) < FLT_EPSILON) {
			total_error = 1.0f + FLT_EPSILON;
		}

		/* Compute the amount of say */
		const float amount_of_say = 0.5f * logf((1.0f - total_error) / total_error);

		/* Add the sum of amout of say */
		if (labels[iteration]) {
			sum_amout_of_say_positive += amount_of_say;
		}
		else {
			sum_amout_of_say_negative += amount_of_say;
		}

		/* Compute the new weights for the incorrects */
		for (i = 0; i < counter_incorrect; i++) {
			weights[row_incorrect_tracker[i]] = weights[row_incorrect_tracker[i]] * expf(amount_of_say);
		}

		/* Compute the new weights for the corrects */
		for (i = 0; i < counter_correct; i++) {
			weights[row_correct_tracker[i]] = weights[row_correct_tracker[i]] * expf(-amount_of_say);
		}

		/* Normalize */
		normalize(weights, row, NORMALIZE_METHOD_TOTAL_SUM_1);

		/* Create buckets of the weights */
		for (i = 1; i < row; i++) {
			weights[i] += weights[i - 1];
		}

		/* Update the X matrix */
		for (i = 0; i < row; i++) {
			/* Pick a random number */
			const float random_number = ((float)rand()) / ((float)RAND_MAX);

			/* Check where random_number lies in between */
			float smallest = 1.0f;
			size_t k = 0;
			for (j = 0; j < row; j++) {
				float small = fabsf(weights[j] - random_number);
				if (small < smallest) {
					smallest = small;
					k = j;
				}
			}

			memcpy(X_new + i * column, X_copy + k * column, column * sizeof(float));
		}

		/* Copy over */
		memcpy(X_copy, X_new, row * column * sizeof(float));
	}

	/* Free */
	free(x);
	free(row_correct_tracker);
	free(row_incorrect_tracker);
	free(X_new);
	free(weights);
	free(X_copy);

	/* Check if the classification was positive or negative */
	return sum_amout_of_say_positive > sum_amout_of_say_negative;
}
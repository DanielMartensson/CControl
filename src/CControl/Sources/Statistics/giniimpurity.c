/*
 * giniimpurity.c
 *
 *  Created on: 20 Janiary 2024
 *      Author: Daniel Mårtensson
 */

#include "statistics.h"

 /*
  * Compute the Gini impurity
  * x[m]
  * labels[m]
  *
  * This can be used for both binary and numerical ways
  */
float giniimpurity(float x[], const bool is_numerical, const bool labels[], float* threshold, const size_t row) {
	/* Declare lowest gini index */
	float lowest_gini_index = 1.0f;

	if (is_numerical) {
		/* Sort it at column wise */
		size_t* index = (size_t*)malloc(row * sizeof(size_t));
		sort(x, index, 1, row, SORT_MODE_COLUMN_DIRECTION_ASCEND);

		/* Find the lowest gini index */
		size_t i, j;
		for (i = 0; i < row - 1; i++) {
			float yes_below = 0.0f;
			float no_below = 0.0f;
			float yes_above = 0.0f;
			float no_above = 0.0f;
			float average = (x[i] + x[i + 1]) / 2.0f;
			for (j = 0; j < row; j++) {
				if (x[j] < average) {
					if (labels[index[j]]) {
						yes_below++;
					}
					else {
						no_below++;
					}
				}
				else {
					if (labels[index[j]]) {
						yes_above++;
					}
					else {
						no_above++;
					}
				}
			}


			/* Calucalte the gini value for the below */
			float propability_yes = yes_below / (yes_below + no_below);
			float propability_no = no_below / (yes_below + no_below);
			const float gini_below = 1.0f - propability_yes * propability_yes - propability_no * propability_no;

			/* Calucalte the gini value for the above */
			propability_yes = yes_above / (yes_above + no_above);
			propability_no = no_above / (yes_above + no_above);
			const float gini_above = 1.0f - propability_yes * propability_yes - propability_no * propability_no;

			/* Calculate the total gini value */
			const float total_yes_no = yes_below + no_below + yes_above + no_above;
			float gini_index = (yes_below + no_below) / total_yes_no * gini_below + (yes_above + no_above) / total_yes_no * gini_above;
			if (gini_index < lowest_gini_index) {
				lowest_gini_index = gini_index;
				*threshold = average;
			}
		}

		/* Free */
		free(index);
	}
	else {
		/* Find the lowest gini index */
		size_t i, j;
		for (i = 0; i < row; i++) {
			float yes_true = 0.0f;
			float no_true = 0.0f;
			float yes_false = 0.0f;
			float no_false = 0.0f;
			for (j = 0; j < row; j++) {
				if (x[j] < 0.5f) {
					if (labels[j]) {
						yes_true++;
					}
					else {
						no_true++;
					}
				}
				else {
					if (labels[j]) {
						yes_false++;
					}
					else {
						no_false++;
					}
				}
			}

			/* Calucalte the gini value for the below */
			float propability_yes = yes_true / (yes_true + no_true);
			float propability_no = no_true / (yes_true + no_true);
			const float gini_below = 1.0f - propability_yes * propability_yes - propability_no * propability_no;

			/* Calucalte the gini value for the above */
			propability_yes = yes_false / (yes_false + no_false);
			propability_no = no_false / (yes_false + no_false);
			const float gini_above = 1.0f - propability_yes * propability_yes - propability_no * propability_no;

			/* Calculate the total gini value */
			const float total_yes_no = yes_true + no_true + yes_false + no_false;
			float gini_index = (yes_true + no_true) / total_yes_no * gini_below + (yes_false + no_false) / total_yes_no * gini_above;
			if (gini_index < lowest_gini_index) {
				lowest_gini_index = gini_index;
			}
		}
	}

	/* Return index */
	return lowest_gini_index;
}

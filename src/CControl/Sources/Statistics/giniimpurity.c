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
float giniimpurity(const float x[], const float threshold, const bool labels[], const size_t row) {
	float yes_below = 0;
	float no_below = 0;
	float yes_above = 0;
	float no_above = 0;
	size_t i;
	for (i = 0; i < row; i++) {
		if (x[i] < threshold) {
			if (labels[i]) {
				yes_below++;
			}
			else {
				no_below++;
			}
		}
		else {
			if (labels[i]) {
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
	return (yes_below + no_below) / total_yes_no * gini_below + (yes_above + no_above) / total_yes_no * gini_above;
}
/*
 * normalize.c
 *
 *  Created on: 20 Januari 2024
 *      Author: Daniel Mårtensson
 */

#include "miscellaneous.h"

/*
 * Normalize a vector x
 * x[m]
 */
void normalize(float x[], const size_t row, const NORMALIZE_METHOD normalize_method) {
	switch (normalize_method) {
	case NORMALIZE_METHOD_UNIT_CIRCLE:
		scalar(x, 1.0f / norm(x, row, 1, NORM_METHOD_L2), row);
		break;
	case NORMALIZE_METHOD_TOTAL_SUM_1: {
		float y;
		sum(x, &y, row, 1, true);
		scalar(x, 1.0f / y, row);
		break;
	}
	}
}
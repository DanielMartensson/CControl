/*
 * generalizedhough.c
 *
 *  Created on: 24 december 2023
 *      Author: Daniel Mårtensson
 */

#include "computervision.h"

/*
 * Create data by using corner detection
 * Return a matrix C with size [max_classes*2]
 */
float* generalizedhough_collect(const uint8_t X[], const size_t row, const size_t column, const uint8_t fast_threshold, const FAST_METHOD fast_method, const float epsilon, const size_t min_pts, size_t* max_classes) {
	/* Landmark detection */
	int N;
	COORDINATE_XY* xy = featuredetection(X, &N, fast_threshold, fast_method, row, column);

	/* Convert to float for using with DBSCAN */
	float* xy_float = (float*)malloc(N * 2 * sizeof(float));
	size_t i;
	for (i = 0; i < N; i++) {
		xy_float[i * 2] = (float)xy[i].x;
		xy_float[i * 2 + 1] = (float)xy[i].y;
	}

	/* Use DBSCAN - When idx[i] is zero, then it contains noise */
	size_t* idx = (size_t*)malloc(N * sizeof(size_t));
	dbscan(xy_float, idx, epsilon, min_pts, N, 2);

	/* Find max classes */
	*max_classes = 0;
	for (i = 0; i < N; i++) {
		if (*max_classes < idx[i]) {
			*max_classes = idx[i];
		}
	}

	/* Find centroid C for each class */
	size_t j;
	float total;
	float* C = (float*)malloc(*max_classes * 2 * sizeof(float));
	memset(C, 0, *max_classes * 2 * sizeof(float));
	for (i = 0; i < *max_classes; i++) {
		total = 0;
		for (j = 0; j < N; j++) {
			if (idx[j] == i + 1) {
				C[i * 2] += xy[j].x;
				C[i * 2 + 1] += xy[j].y;
				total++;
			}
		}

		/* Average */
		C[i * 2] /= total;
		C[i * 2 + 1] /= total;
	}

	/* Free */
	free(xy);
	free(idx);
	free(xy_float);

	/* Return C */
	return C;
}

/* 
 * Generalized Hough Transform with Mårtensson Method
 * X[m*2] - Contains feature points from an image e.g corner detection
 * Return back the generalized hough model
 */
GENERALIZED_HOUGH_MODEL* generalizedhough_train(const float X[], const size_t row) {
	/* Constants and variables */
	const size_t column = 2;
	const size_t column_bytes = column * sizeof(float);
	size_t i, j, k, xc, yc;
	float alpha, s;

	/* Create vectors */
	float* vector_A = (float*)malloc(column_bytes);
	float* vector_B = (float*)malloc(column_bytes);
	float* vector_C = (float*)malloc(column_bytes);
	float* C = (float*)malloc(column_bytes);
	float* unit_vector = (float*)malloc(column_bytes);
	float* xc_yc = (float*)malloc(column_bytes);

	/* Find centroid */
	centroid(X, C, row, column);

	/* Find the distances */
	float* D = (float*)malloc(row * row * sizeof(float));
	pdist2(X, X, D, row, column, row, PDIST2_METRIC_EUCLIDEAN);

	/* Sort distances and also the index */
	size_t* index = (size_t*)malloc(row * row * sizeof(size_t));
	sort(D, index, row, row, SORT_MODE_COLUMN_DIRECTION_ASCEND);

	/* Create generalized hough model */
	GENERALIZED_HOUGH_MODEL* model = (GENERALIZED_HOUGH_MODEL*)malloc(181 * sizeof(GENERALIZED_HOUGH_MODEL));
	
	/* Important to set all to zero */
	memset(model, 0, 181 * sizeof(GENERALIZED_HOUGH_MODEL));

	/* Create the accumulator array */
	size_t pm = 0;
	size_t pn = 0;
	for (i = 0; i < row; i++) {
		if (pm < (size_t)X[i * column]) {
			pm = (size_t)X[i * column];
		}
		if (pn < (size_t)X[i * column + 1]) {
			pn = (size_t)X[i * column + 1];
		}
	}
	float* accumulator = (float*)malloc(pm * pn * sizeof(float));
	memset(accumulator, 0, pm * pn * sizeof(float));

	/* Find the angles between vectors from each cluster point */
	for (i = 0; i < row; i++) {
		/* Status 
		printf("Training: %i of %i points left\n", i, row); */

		/* Select the target vector */
		float* target = &X[index[i * row] * column];

		/* Get the closest point */
		memcpy(vector_A, &X[index[i * row + 1] * column], column_bytes);

		/* Get the second closest point */
		memcpy(vector_B, &X[index[i * row + 2] * column], column_bytes);

		/* Center */
		for (j = 0; j < column; j++) {
			vector_A[j] -= target[j];
			vector_B[j] -= target[j];
			vector_C[j] = C[j] - target[j];
		}

		/* Compute the angle between vectors */
		const uint8_t theta = (uint8_t)(anglevector(vector_A, vector_B, column));
		const float alpha1 = anglevector(vector_A, vector_C, column);
		const float alpha2 = anglevector(vector_B, vector_C, column);

		/* Compute the euclidean distance */
		const float RA = norm(vector_A, 1, column, NORM_METHOD_L2);
		const const RB = norm(vector_B, 1, column, NORM_METHOD_L2);
		const float RC = norm(vector_C, 1, column, NORM_METHOD_L2);

		/* Allocate new vote */
		const size_t votes_active = model[theta].votes_active;
		model[theta].vote = (GENERALIZED_HOUGH_VOTE*)realloc(model[theta].vote, (votes_active + 1) * sizeof(GENERALIZED_HOUGH_VOTE));

		/* Save shortest of RA and RB - This is used for scaling */
		if (RA < RB){
			model[theta].vote[votes_active].shortest = RA;
			model[theta].vote[votes_active].alpha = alpha1;
		}
		else {
			model[theta].vote[votes_active].shortest = RB;
			model[theta].vote[votes_active].alpha = alpha2;
		}

		/* Compute the euclidean distance between centroid and target */
		model[theta].vote[votes_active].R = RC;

		/* Count */
		model[theta].votes_active++;

		/* Estimate the centroid */
		for (j = 0; j < 2; j++) {
			/* Once again, select the unit vector because in the estimation, the vector_C is unknown */
			if (RA < RB) {
				memcpy(unit_vector, vector_A, column_bytes);
			}
			else {
				memcpy(unit_vector, vector_B, column_bytes);
			}

			/* Check direction */
			if (j == 0) {
				alpha = 360 - model[theta].vote[votes_active].alpha;
			}
			else {
				alpha = model[theta].vote[votes_active].alpha;
			}

			/* Create the unit vector by using L2-norm */
			s = norm(unit_vector, 1, column, NORM_METHOD_L2);
			for (k = 0; k < column; k++) {
				unit_vector[k] /= s;
			}

			/* Rotate the unit vector */
			rot2d(unit_vector, deg2rad(alpha));
			scalar(unit_vector, model[theta].vote[votes_active].R, column);

			/* Compute the xc and xy */
			for (k = 0; k < column; k++) {
				xc_yc[k] = unit_vector[k] + target[k];
			}

			xc = (size_t)xc_yc[0];
			yc = (size_t)xc_yc[1];

			/* Save in the accumulator array */
			if (xc >= 0 && yc >= 0, xc < pm && yc < pn) {
				accumulator[xc * pn + yc]++;
			}
		}
	}

	/* Find the largest value inside the accumulator array */
	size_t max_index;
	amax(accumulator, &max_index, pm * pn);
	ind2sub(max_index, pn, &xc, &yc);

	/* Display */
	printf("The centroid was[%f, %f].\nThe estimated centroid was[%i, %i]\n", C[0], C[1], (int)xc, (int)yc);

	/* Free */
	free(vector_A);
	free(vector_B);
	free(vector_C);
	free(C);
	free(D);
	free(index);
	free(unit_vector);
	free(xc_yc);
	free(accumulator);

	/* Return */
	return model;
}

/*
 * Evaluate the Generalized Hough Model with Mårtensson method
 * X[m*2] - Contains feature points from an image e.g corner detection
 * xc and yc are center coordinates for the detected object
 */
void generalizedhough_eval(const float X[], const GENERALIZED_HOUGH_MODEL model[], const float smoothing_accumulator, float* max_value_accumulator, size_t* xc, size_t* yc, const size_t row) {
	/* Constants and variables */
	const size_t column = 2;
	const size_t column_bytes = column * sizeof(float);
	size_t i, j, k, l;
	float ratio, R, alpha, s;

	/* Create vectors */
	float* vector_A = (float*)malloc(column_bytes);
	float* vector_B = (float*)malloc(column_bytes);
	float* unit_vector = (float*)malloc(column_bytes);
	float* xc_yc = (float*)malloc(column_bytes);

	/* Create the accumulator array */
	size_t pm = 0;
	size_t pn = 0;
	float* X0 = X;
	for (i = 0; i < row; i++) {
		if (pm < (size_t)X0[0]) {
			pm = (size_t)X0[0];
		}
		if (pn < (size_t)X0[1]) {
			pn = (size_t)X0[1];
		}
		X0 += column;
	}
	float* accumulator = (float*)malloc(pm * pn * sizeof(float));
	memset(accumulator, 0, pm * pn * sizeof(float));

	/* Find the distances */
	float* D = (float*)malloc(row * row * sizeof(float));
	pdist2(X, X, D, row, column, row, PDIST2_METRIC_EUCLIDEAN);

	/* Sort distances and also the index */
	size_t* index = (size_t*)malloc(row * row * sizeof(size_t));
	sort(D, index, row, row, SORT_MODE_COLUMN_DIRECTION_ASCEND);

	/* Find the angles between vectors from each cluster point */
	for (i = 0; i < row; i++) {
		/* Status 
		printf("Evaluate: %i of %i points left\n", i, row); */

		/* Select the target vector */
		float* target = &X[index[i * row] * column];

		/* Get the closest point */
		memcpy(vector_A, &X[index[i * row + 1] * column], column_bytes);

		/* Get the second closest point */
		memcpy(vector_B, &X[index[i * row + 2] * column], column_bytes);

		/* Center */
		for (j = 0; j < column; j++) {
			vector_A[j] -= target[j];
			vector_B[j] -= target[j];
		}

		/* Compute the angle between vectors */
		const uint8_t theta = (uint8_t)(anglevector(vector_A, vector_B, column));

		/* Compute the euclidean distance */
		const float RA = norm(vector_A, 1, column, NORM_METHOD_L2);
		const const RB = norm(vector_B, 1, column, NORM_METHOD_L2);

		/* Get the shortest distance */
		const float shortest = RA < RB ? RA : RB;
		
		/* Compare votes */
		for (j = 0; j < model[theta].votes_active; j++) {
			/* Display 
			printf("Evaluate: %i of %i votes left at theta %i\n", j, model[theta].votes_active, theta); */

			/* Compute the ratio for scaling */
			ratio = shortest / model[theta].vote[j].shortest;

			/* Compute the R distance */
			R = ratio * model[theta].vote[j].R;

			/* Estimate the centroid */
			for (k = 0; k < 2; k++) {
				/* Once again, select the unit vector because in the estimation, the vector_C is unknown */
				if (RA < RB) {
					memcpy(unit_vector, vector_A, column_bytes);
				}
				else {
					memcpy(unit_vector, vector_B, column_bytes);
				}

				/* Check direction */
				if (k == 0) {
					alpha = 360 - model[theta].vote[j].alpha;
				}
				else {
					alpha = model[theta].vote[j].alpha;
				}

				/* Create the unit vector by using L2-norm */
				s = norm(unit_vector, 1, column, NORM_METHOD_L2);
				scalar(unit_vector, 1 / s, column);

				/* Rotate the unit vector */
				rot2d(unit_vector, deg2rad(alpha));
				scalar(unit_vector, R, column);

				/* Compute the xc and xy */
				for (l = 0; l < column; l++) {
					xc_yc[l] = unit_vector[l] + target[l];
				}
				*xc = (size_t)xc_yc[0];
				*yc = (size_t)xc_yc[1];

				/* Save in the accumulator array */
				if ((*xc) >= 0 && (*yc) >= 0, (*xc) < pm && (*yc) < pn) {
					accumulator[(*xc) * pn + (*yc)]++;
				}
			}
		}
	}

	/* Do gaussian smoothing on the peaks */
	if (smoothing_accumulator > 0) {
		imgaussfilt(accumulator, smoothing_accumulator, pm, pn);
	}

	/* Find the largest value inside the accumulator array */
	size_t max_index;
	*max_value_accumulator = amax(accumulator, &max_index, pm * pn);
	ind2sub(max_index, pn, xc, yc);

	/* Free */
	free(vector_A);
	free(vector_B);
	free(unit_vector);
	free(accumulator);
	free(index);
	free(D);
	free(xc_yc);
}

/*
 * Free the generalized hough model
 */
void generalizedhough_free(const GENERALIZED_HOUGH_MODEL model[]) {
	uint8_t i;
	for (i = 0; i < 181; i++) {
		if (model[i].votes_active > 0) {
			/* Free */
			free(model[i].vote);
		}
	}

	/* Free */
	free(model);
}
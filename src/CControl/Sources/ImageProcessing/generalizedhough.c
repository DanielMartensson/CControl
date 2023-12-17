/*
 * generalizedhough.c
 *
 *  Created on: 12 december 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/* 
 * Generalized Hough Transform - With rotation invariant and scale invariant
 * X[m*2] - Contains an image of coordinates
 * Return back the generalized hough model
 */
GENERALIZED_HOUGH_MODEL* generalizedhough_create_model(const float X[], const size_t row) {
	/* Constants and variables */
	const size_t column = 2;
	const size_t column_bytes = column * sizeof(float);
	size_t i, j, k, l;

	/* Create vectors */
	float* vector_A = (float*)malloc(column_bytes);
	float* vector_B = (float*)malloc(column_bytes);
	float* vector_C = (float*)malloc(column_bytes);
	float* C = (float*)malloc(column_bytes);

	/* Find centroid */
	centroid(X, C, row, column);

	/* Create generalized hough model */
	GENERALIZED_HOUGH_MODEL* generalized_hough_model = (GENERALIZED_HOUGH_MODEL*)malloc(180 * sizeof(GENERALIZED_HOUGH_MODEL));
	
	/* Important to set all to zero */
	memset(generalized_hough_model, 0, 180 * sizeof(GENERALIZED_HOUGH_MODEL));

	/* Find the angles between vectors from each cluster point */
	for (i = 0; i < row; i++) {
		/* Select the target vector */
		float* target = &X[i * column];

		/* Center */
		for (l = 0; l < column; l++) {
			vector_C[l] = C[l] - target[l];
		}

		/* Iterate the rest of the points */
		for (j = 0; j < row; j++) {
			/* We cannot select the target vector */
			if (j != i) {
				/* Select peripheral vector A */
				memcpy(vector_A, &X[j * column], column_bytes);

				/* Center */
				for (l = 0; l < column; l++) {
					vector_A[l] -= target[l];
				}

				for (k = 0; k < row; k++) {
					/* We cannot select the same peripheral vector */
					if (k != j) {
						/* Select peripheral vector B */
						memcpy(vector_B, &X[k * column], column_bytes);

						/* Center */
						for (l = 0; l < column; l++) {
							vector_B[l] -= target[l];
						}

						/* Compute the angle between vectors */
						int theta = (int)anglevector(vector_A, vector_B, column);
						if (theta == 180) {
							theta--;
						}
						float alpha1 = anglevector(vector_A, vector_C, column);
						float alpha2 = anglevector(vector_B, vector_C, column);

						/* Compute the euclidean distance */
						float RA = norm(vector_A, column, 1, NORM_METHOD_L2);
						float RB = norm(vector_B, column, 1, NORM_METHOD_L2);
						float RC = norm(vector_C, column, 1, NORM_METHOD_L2);

						/* Allocate new vote */
						size_t votes_active = generalized_hough_model[theta].votes_active;
						generalized_hough_model[theta].vote = (GENERALIZED_HOUGH_VOTE*)realloc(generalized_hough_model[theta].vote, (votes_active + 1) * sizeof(GENERALIZED_HOUGH_VOTE));

						/* Save shortest of RA and RB - This is used for scaling */
						generalized_hough_model[theta].vote[votes_active].shortest = RA < RB ? RA : RB;

						/* Compute the euclidean distance between centroid and target */
						generalized_hough_model[theta].vote[votes_active].R = RC;

						/* Select the right alpha */
						generalized_hough_model[theta].vote[votes_active].alpha = RA < RB ? alpha1 : alpha2;

						/* Check direction */
						generalized_hough_model[theta].vote[votes_active].clock_wise_direction = alpha2 > alpha1;
						
						/* Count */
						generalized_hough_model[theta].votes_active++;
					}
				}
			}	
		}
	}

	/* Free */
	free(vector_A);
	free(vector_B);
	free(vector_C);
	free(C);

	/* Return */
	return generalized_hough_model;
}

/*
 * Evaluate the generalized hough model
 * X[m*2] - Contains an image of coordinates
 * xc and yc are center coordinates for the detected object
 */
void generalizedhough_eval_votes(const float X[], const GENERALIZED_HOUGH_MODEL model[], size_t* xc, size_t* yc, const size_t row) {
	/* Constants and variables */
	const size_t column = 2;
	const size_t column_bytes = column * sizeof(float);
	size_t i, j, k, l;

	/* Create vectors */
	float* vector_A = (float*)malloc(column_bytes);
	float* vector_B = (float*)malloc(column_bytes);
	float* unit_vector = (float*)malloc(column_bytes);

	/* Find the maximum of X(:, 1) and X(:, 2) */
	size_t width = 0; 
	size_t height = 0;
	float* X0 = X;
	for (i = 0; i < row; i++) {
		/* This is MATLAB X(:, 1) */
		if (X[0] > width) {
			width = (size_t)X[0];
		}

		/* This is MATLAB X(:, 2) */
		if (X[1] > height) {
			height = (size_t)X[1];
		}

		/* Next row */
		X += column;
	}

	/* Reset */
	X = X0;

	/* Create accumulator array */
	size_t* accumulator = (size_t*)malloc(height * width * sizeof(size_t));
	size_t* accumulator0 = accumulator;
	memset(accumulator, 0, height * width * sizeof(size_t));

	/* Find the angles between vectors from each cluster point */
	for (i = 0; i < row; i++) {
		/* Select the target vector */
		float* target = &X[i * column];

		/* Iterate the rest of the points */
		for (j = 0; j < row; j++) {
			/* We cannot select the target vector */
			if (j != i) {
				/* Select peripheral vector A */
				memcpy(vector_A, &X[j * column], column_bytes);

				/* Center */
				for (l = 0; l < column; l++) {
					vector_A[l] -= target[l];
				}

				for (k = 0; k < row; k++) {
					/* We cannot select the same peripheral vector */
					if (k != j) {
						/* Select peripheral vector B */
						memcpy(vector_B, &X[k * column], column_bytes);

						/* Center */
						for (l = 0; l < column; l++) {
							vector_B[l] -= target[l];
						}

						/* Compute the angle between vector A and B - Here we don't know alpha1 and alpha2 */
						int theta = (int)anglevector(vector_A, vector_B, column);
						if (theta == 180) {
							theta--;
						}

						/* Compute the euclidean distance */
						float RA = norm(vector_A, column, 1, NORM_METHOD_L2);
						float RB = norm(vector_B, column, 1, NORM_METHOD_L2);

						/* Get the shortest distance */
						float shortest = RA < RB ? RA : RB;
						
						/* Compare votes */
						for (l = 0; l < model[theta].votes_active; l++) {
							/* Compute the ratio for scaling */
							float ratio = shortest / model[theta].vote[l].shortest;

							/* Compute the R distance */
							float R = ratio * model[theta].vote[l].R;

							/* Copy over the shortest vector */
							if (RA < RB) {
								memcpy(unit_vector, vector_A, column_bytes);
							}
							else {
								memcpy(unit_vector, vector_B, column_bytes);
							}

							/* Create the unit vector*/
							unit(unit_vector, column);

							/* Rotate the unit vector */
							const float angle = model[theta].vote[l].alpha;
							const float radians = model[theta].vote[l].clock_wise_direction ? deg2rad(360.0f - angle) : deg2rad(angle);
							rot2d(unit_vector, radians);

							/* Scale with R */
							scalar(unit_vector, R, column);

							/* Compute the xc and yc coordinates */
							size_t center_x = (size_t) (target[0] + unit_vector[0]);
							size_t center_y = (size_t) (target[1] + unit_vector[1]);

							/* Count */
							accumulator[center_y * height + center_x] += 1;
						}
					}
				}
			}
		}
	}

	/* Find the largest value int the accumulator array */
	k = 0;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			if (k < accumulator[j]) {
				/* Remember the largest value */
				k = accumulator[j];

				/* Get the coordinates */
				*yc = i;
				*xc = j;
			}
		}

		/* Next row */
		accumulator += width;
	}

	/* Reset */
	accumulator = accumulator0;

	/* Free */
	free(vector_A);
	free(vector_B);
	free(unit_vector);
	free(accumulator);
}

/*
 * Free the generalized hough model
 */
void generalizedhough_free_model(const GENERALIZED_HOUGH_MODEL model[]) {
	uint8_t i;
	for (i = 0; i < 180; i++) {
		if (model[i].votes_active > 0) {
			/* Free */
			free(model[i].vote);
		}
	}

	/* Free */
	free(model);
}
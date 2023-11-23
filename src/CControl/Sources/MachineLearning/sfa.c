/*
 * sfa.c
 *
 *  Created on: 17 oktober 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/* Private functions */
static void find_longest_distance(const int* num_corners, int* current_x, int* current_y, const FAST_XY* xy, bool used[]);

 /*
  * Sobel Fast Angle
  * X[m*n]
  * Return a histogram of angles with length histogram_size
  */
float* sfa(float X[], const uint8_t fast_threshold, const uint8_t sobel_threshold, const FAST_METHOD fast_method, const uint8_t histogram_size, const uint8_t length_grid, const uint8_t points_grid, const size_t row, const size_t column) {
	/* Sobel operator */
	const size_t row_column = row * column;
	float* G = (float*)malloc(row_column * sizeof(float));
	sobel(X, G, NULL, row, column, SOBEL_METHOD_GRADIENT);

	/* Create uint8_t matrix of the float matrix */
	size_t i;
	uint8_t* Xuint8 = (uint8_t*)malloc(row_column);
	memset(Xuint8, 0U, row_column);
	for (i = 0; i < row_column; i++) {
		if (G[i] >= sobel_threshold) {
			Xuint8[i] = (float)G[i];
		}
	}

	/* Free */
	free(G);

	/* Apply FAST */
	int num_corners;
	FAST_XY* xy = fast_features(Xuint8, row, column, fast_threshold, &num_corners, fast_method, length_grid, points_grid);

	/* Free */
	free(Xuint8);

	/* Create a boolean array of flags */
	bool* used = (bool*)malloc(num_corners * sizeof(bool));
	memset(used, false, num_corners * sizeof(bool));

	/* Create histogram */
	float* histogram = (float*)malloc(histogram_size * sizeof(float));
	memset(histogram, 0, histogram_size * sizeof(float));

	/* Holders */
	int x = 0, y = 0;
	int current_x[3];
	int current_y[3];

	/* Pre-find */
	if (num_corners > 3) {
		/* Save */
		current_x[0] = x;
		current_y[0] = y;

		/* Search for the longest distance */
		find_longest_distance(&num_corners, &x, &y, xy, used);

		/* Save */
		current_x[1] = x;
		current_y[1] = y;

		/* Search for the longest distance */
		find_longest_distance(&num_corners, &x, &y, xy, used);

		/* Save */
		current_x[2] = x;
		current_y[2] = y;

		for (i = 3; i < num_corners; i++) {
			/* Create two vectors in 2D. The middle array value is the middle coordinate */
			const float a[2] = { current_x[0] - current_x[1], current_y[0] - current_y[1] };
			const float b[2] = { current_x[2] - current_x[1], current_y[2] - current_x[1] };

			/* Compute the angle between two vectors */	
			int angle = anglevector(a, b, 2) * ((float)histogram_size / 180.0f) - 1;

			/* Set the angle */
			histogram[angle]++;

			/* Shift backwards */
			current_x[0] = current_x[1];
			current_y[0] = current_y[1];
			current_x[1] = current_x[2];
			current_y[1] = current_y[2];

			/* Search for the longest distance */
			find_longest_distance(&num_corners, &x, &y, xy, used);

			/* Update */
			current_x[2] = x;
			current_y[2] = y;
		}
	}

	/* Free */
	free(xy);
	free(used);

	/* Return histogram */
	return histogram;
}

static void find_longest_distance(const int* num_corners, int* current_x, int* current_y, const FAST_XY* xy, bool used[]) {
	float max_distance = 0.0f;
	int i, index = 0;
	for (i = 0; i < *num_corners; i++) {
		/* Compute the difference */
		const float x_diff = (float)*current_x - xy[i].x;
		const float y_diff = (float)*current_y - xy[i].y;

		/* Check with L2-norm */
		const float distance = sqrtf(x_diff * x_diff + y_diff * y_diff);

		/* Compare the distance and find the maximal distance */
		if (max_distance < distance && !used[i]) {
			max_distance = distance;
			index = i;
		}
	}

	/* Save */
	used[index] = true;
	*current_x = xy[index].x;
	*current_y = xy[index].y;
}
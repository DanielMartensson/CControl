/*
 * clusterdistance.c
 *
 *  Created on: 4 december 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/* 
 * Compare the average distance between the clostest points between two clusters X and Y
 * X[row_x * column]
 * Y[row_y * column]
 * Return: The average of clostest points between clusters X and Y
 */
float clusterdistance(const float X[], const float Y[], const size_t row_x, const size_t row_y, const size_t column, const CLUSTER_DISTANCE_METHOD cluster_distance_method) {
	/* Find all the distances D(i, j) between X(i, :) and Y(j, :) */
	float* D = (float*)malloc(row_x * row_y * sizeof(float));
	float* D0 = D;
	switch (cluster_distance_method) {
	case CLUSTER_DISTANCE_METHOD_R_AXIS: {
		pdist2(X, Y, D, row_x, column, row_y, PDIST2_METRIC_EUCLIDEAN);
		break;
	}case CLUSTER_DISTANCE_METHOD_X_AXIS: {
		/* Cut first column from both X and Y */
		float* x1 = (float*)malloc(row_x * sizeof(float));
		float* y1 = (float*)malloc(row_y * sizeof(float));
		cut(X, column, x1, 0, row_x - 1, 0, 0);
		cut(Y, column, y1, 0, row_y - 1, 0, 0);
		pdist2(x1, y1, D, row_x, 1, row_y, PDIST2_METRIC_EUCLIDEAN);
		free(x1);
		free(y1);
		break;
	}case CLUSTER_DISTANCE_METHOD_Y_AXIS: {
		/* Cut second column from both X and Y */
		float* x2 = (float*)malloc(row_x * sizeof(float));
		float* y2 = (float*)malloc(row_y * sizeof(float));
		cut(X, column, x2, 0, row_x - 1, 1, 1);
		cut(Y, column, y2, 0, row_y - 1, 1, 1);
		pdist2(x2, y2, D, row_x, 1, row_y, PDIST2_METRIC_EUCLIDEAN);
		free(x2);
		free(y2);
		break;
	}
	}

	/* Find the minimal values */
	float* min_distances_clostest_points = (float*)malloc(row_x * sizeof(float));
	size_t i, min_index;
	for (i = 0; i < row_x; i++) {
		/* Find the minimum value at D(i, :)
		 * This is the shortest distance for the closest points between X and Y 
		 * MATLAB: min_distances_clostest_points = min(D, [], 2);
		 */
		min_distances_clostest_points[i] = amin(D, &min_index, row_y);
		
		/* Next row */
		D += row_y;
	}

	/* Compute the average of the distance between the clostest points */
	float average_distance_clostest_points = mean(min_distances_clostest_points, row_x);

	/* Free */
	free(D0);
	free(min_distances_clostest_points);

	/* Return */
	return average_distance_clostest_points;
}
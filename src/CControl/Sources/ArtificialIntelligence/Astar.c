/*
 * Astar.c
 *
 *  Created on: 23 Feb. 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

static void heuristic_map(int32_t map[], size_t x_stop, size_t y_stop, size_t height, size_t width, ASTAR_MODE astar_mode);

/*
 * This is A* algorithm. An AI-algorithm in other words.
 * It find the optimal(at least try too) path from your source to your destination.
 * See working example how to use.
 * I wrote this C code because I don't like calloc, malloc and recalloc in embedded.
 */
void Astar(int32_t map[], int32_t path_x[], int32_t path_y[], size_t x_start, size_t y_start, size_t x_stop, size_t y_stop, size_t height, size_t width, ASTAR_MODE astar_mode, size_t* steps) {
	/* Clear first our path */
	memset(path_x, -1, height * width * sizeof(int32_t));
	memset(path_y, -1, height * width * sizeof(int32_t));

	/* Create map with a norm */
	heuristic_map(map, x_stop, y_stop, height, width, astar_mode);

	/* Set some initial parameters */
	int32_t direction[4];
	const int32_t x_directions[4] = { -1, 1, 0, 0 };
	const int32_t y_directions[4] = { 0, 0, -1, 1 };
	int32_t position = 0;
	int32_t minValue = 0;
	size_t x = x_start;
	size_t y = y_start;

	/* print32_tf("Current start coordinate: x = %d, y = %d\n", x, y); */
	size_t k, index, i, j;

	for (k = 0; k < height * width; k++) {
		/* Look to the left, right, up and down */
		for (index = 0; index < 4; index++) {
			direction[index] = map[(y + y_directions[index]) * width + x + x_directions[index]];
		}
		/* Take the decision where to go by looking at direction array */
		minValue = direction[0];
		position = 0;
		for (index = 0; index < 4; index++) {
			/* If our minimal value is -1, change it */
			if (minValue == -1) {
				minValue = direction[index];
				position = index;
			} else {
				/* Check if we have found the smallest number now */
				if (direction[index] != -1 && direction[index] < minValue) {
					minValue = direction[index];
					position = index;
				}
			}
		}
		/* Prevent repeating by increasing the weights */
		map[y * width + x] += map[y * width + x];

		/* Save path */
		path_x[k] = x;
		path_y[k] = y;

		/* Where to go - position variable tells */
		if (position == 0) x--; /* Go one step left */
		if (position == 1) x++; /* Go one step right */
		if (position == 2) y--; /* Go one step up */
		if (position == 3) y++; /* Go one step down */

		/* Check if we are at our goal */
		if (*(map + y * width + x) == 0) {
			path_x[k + 1] = x;
			path_y[k + 1] = y;
			break;
		}

	}

	/* Filter the path_x and path_y from duplicates */
	for (i = 0; i < height * width; i++) {
		position = 0;
		x = path_x[i];
		y = path_y[i];
		if (x != -1 && y != -1) {
			/* Search for the last same x and y in path_x and path_y */
			for (j = i + 1; j < height * width; j++) {
				if (x == path_x[j] && y == path_y[j]) {
					position = j; /* Remember this position */
				}
			}
			/* If we got duplicates - delete them */
			if (position > 0) {
				memset(path_x + i, -1, (position - i) * sizeof(int32_t));
				memset(path_y + i, -1, (position - i) * sizeof(int32_t));
				i = position; /* Jump */
			}
		} else {
			position = i; /* Remember that too */
			break; /* The rest is just -1's */
		}
	}

	/* Pack path_x and path_y together now */
	for (i = 0; i < position; i++) {
		/* If we have a deleted coordinate */
		if (path_x[i] == -1 && path_y[i] == -1) {
			/* Move them up */
			for (j = i; j < height * width; j++) {
				if (path_x[j] != -1 && path_y[j] != -1) {
					path_x[i] = path_x[j];
					path_x[j] = -1;
					path_y[i] = path_y[j];
					path_y[j] = -1;
					break;
				}
			}
		}
	}

	/* Filter the path_x and path_y from zigzag */
	*steps = 0;
	for (i = 0; i < height * width; i++) {
		position = 0;
		x = path_x[i];
		y = path_y[i];
		if (x != -1 && y != -1) {
			/* Search for a jump */
			for (index = 0; index < 4; index++) {
				for (j = i + 1; j < height * width; j++) {
					if (x + x_directions[index] == path_x[j] && y + y_directions[index] == path_y[j]) {
						if (j > position)
							position = j; /* We want to have the largest "gap", which is the zigzag */
					}
				}
				/* If we got zigzag. We need to have + 1 because we cannot accept a neighbor step as zigzag */
				if (position > i + 1) {
					memset(path_x + i + 1, -1, (position - i - 1) * sizeof(int32_t));
					memset(path_y + i + 1, -1, (position - i - 1) * sizeof(int32_t));
					i = position; /* Jump */
					*steps += 1;
				}
			}
			*steps += 1;
		} else {
			position = i; /* Remember that too */
			break; /* The rest is just -1's */
		}
	}

	/* Pack path_x and path_y together again */
	for (i = 0; i < position; i++) {
		/* If we have a deleted coordinate */
		if (path_x[i] == -1 && path_y[i] == -1) {
			/* Move them up */
			for (j = i; j < height * width; j++) {
				if (path_x[j] != -1 && path_y[j] != -1) {
					path_x[i] = path_x[j];
					path_x[j] = -1;
					path_y[i] = path_y[j];
					path_y[j] = -1;
					break;
				}
			}
		}
	}
}

static void heuristic_map(int32_t map[], size_t x_stop, size_t y_stop, size_t height, size_t width, ASTAR_MODE astar_mode) {
	size_t i, j;
	int32_t dx, dy;
	for (i = 0; i < width; i++) {
		for (j = 0; j < height; j++) {
			if (map[j * width + i] != -1) {
				dx = (j + x_stop) - (j + i); 				/* Distance we want to go in x-axis */
				dy = (y_stop + i) - (j + i);  				/* Distance we want to go in y-axis */
				switch (astar_mode) {
				case ASTAR_MODE_L1:
					map[j * width + i] = abs(dx) + abs(dy);			/* L1-Norm */
					break;
				case ASTAR_MODE_L2:
					map[j * width + i] = dx * dx + dy * dy; /* L2-Norm (without square root, not needed due to integers) */
					break;
				}
				/* You can add more heuristic here! */
			}
		}
	}
}

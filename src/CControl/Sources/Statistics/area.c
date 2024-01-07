/*
 * area.c
 *
 *  Created on: Oktober 21, 2023
 *      Author: Daniel Mårtensson
 */

#include "statistics.h"

/*
 * Compute the average of all values inside a circle
 * X[m*n]
 * m == n
 */
float area(const float X[], const size_t row, size_t* total_elements, const AREA_METHOD area_method) {
	/* Define area variable*/
	float area_value = 0.0f;
	*total_elements = 0U;

	/* Select which type of area we want to compute */
	switch (area_method) {
	case AREA_METHOD_CIRCLE: {
		/* Compute the radius */
		const size_t radius = row / 2;

		/* Total iterations */
		size_t i, j;
		float coordinate[2];
		float distance;
		for (i = 0; i < row; i++) {
			for (j = 0; j < row; j++) {
				/* Compute the distance by giving the row and column coordinates for L2-norm */
				coordinate[0] = (float)radius - i;
				coordinate[1] = (float)radius - j;
				distance = norm(coordinate, 2, 1, NORM_METHOD_L2);

				/* Check if distance is equal or less */
				if (distance <= radius) {
					area_value += X[j];
					(*total_elements)++;
				}
			}

			/* New row */
			X += row;
		}

		break;
	}case AREA_METHOD_SQURE: {
		/* Total iterations */
		size_t i;
		const size_t row_row = row * row;
		for (i = 0; i < row_row; i++) {
			area_value += X[i];
			(*total_elements)++;
		}
		break;
	}
	}

	/* Return area */
	return area_value;
}

/* GNU Octave code:

function avg = circleaverage(X)
  % Compute row
  [m, n] = size(X);

  % Compute the radius
  radius = m / 2;

  % Total iterations
  counter = 0;
  s = 0.0;
  coordinate = zeros(1, 2);
  for i = 1:m
	for j = 1:n
	  % Compute the distance by giving the row and column coordinates for L2-norm
	  coordinate(1) = radius - i + 1;
	  coordinate(2) = radius - j + 1;
	  distance = norm(coordinate, 2);

	  % Check if distance is equal or less
	  if distance <= radius
		s = s + X(i, j);
		counter = counter + 1;
	  end
	end
  end

  % Compute the average
  avg = s / counter;
end

*/
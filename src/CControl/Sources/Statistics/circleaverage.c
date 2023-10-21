/*
 * circleaverage.c
 *
 *  Created on: Oktober 21, 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/*
 * Compute the average of all values inside a circle
 * X[m*n]
 * m == n
 */
float circleaverage(float X[], size_t row) {
	/* Compute the radius */
	const size_t radius = row / 2;

	/* Total iterations */
	size_t i, j, counter = 0;
	float s = 0.0f;
	float coordinate[2];
	float distance;
	for (i = 0; i < row; i++) {
		for (j = 0; j < row; j++) {
			/* Compute the distance by giving the row and column coordinates for L2-norm */
			coordinate[0] = (float) radius - i;
			coordinate[1] = (float) radius - j;
			distance = norm(coordinate, 2, 1, NORM_METHOD_L2);

			/* Check if distance is equal or less */
			if (distance <= radius) {
				s += X[0];
				counter++;
			}

			/* Shift index one step */
			X += 1;
		}
	}

	/* Compute the average */
	return s / ((float)counter);
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
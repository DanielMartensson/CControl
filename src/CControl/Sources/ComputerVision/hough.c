/*
 * hough.c
 *
 *  Created on: 16 september 2023
 *      Author: Daniel Mårtensson
 */

#include "computervision.h"

static float* hough_scores(const float X[], const float p, size_t* r_half, const size_t row, const size_t column);
static size_t hough_cluster(const float P[], float* x[], float* y[], float* z[], size_t* index[], size_t* L, const float epsilon, const size_t min_pts, const size_t r_half);
static void hough_lines(const float* x[], const float* y[], const float* z[], const size_t L, const size_t N, const size_t* index[], float* K[], float* M[], const size_t r_half);

/*
 * Hough Transform - Line detection of an edge image 
 * X[m*n] = Data matrix of an edge image
 * K[N] = Slope for the line
 * M[N] = Bias for the line
 * p = Line length threshold in precent
 * epsilon = Minimum radius for hough cluster
 * min_pts = Minimum points for hough cluster
 * 
 * Hough Transform will define the matricies K and M and the size of them (N)
 */
size_t hough(const float X[], float* K[], float* M[], const float p, const float epsilon, const size_t min_pts, const size_t row, const size_t column) {
	/* Compute scores for the lines */
	size_t r_half;
	float* P = hough_scores(X, p, &r_half, row, column);

	/* Turn the scores matrix into a hough cluster */
	float* x = NULL;
	float* y = NULL;
	float* z = NULL;
	float* index = NULL;
	size_t L;
	size_t N = hough_cluster(P, &x, &y, &z, &index, &L, epsilon, min_pts, r_half);

	/* Free */
	free(P);

	/* Compute lines */
	hough_lines(&x, &y, &z, L, N, &index, K, M, r_half);

	/* Free */
	free(x);
	free(y);
	free(z);
	free(index);

	/* Return the size of K and M */
	return N;
}

static void hough_lines(const float* x[], const float* y[], const float* z[], const size_t L, const size_t N, const size_t* index[], float* K[], float* M[], const size_t r_half) {
	/* Create K and M - They are holders for the output */
	*K = (float*)malloc(N * sizeof(float));
	*M = (float*)malloc(N * sizeof(float));

	/* Fill */
	size_t i, j, max_index;
	float value, max_value, angle, r;
	for (i = 1; i <= N; i++) {
		/* Find the maximum value at z-axis of a specific class ID */
		max_value = 0;
		max_index = 0;
		for (j = 0; j < L; j++) {
			if (i == (*index)[j]) {
				value = (*z)[j];
				if (value >= max_value) {
					max_value = value;
					max_index = j;
				}
			}
		}

		/* Get the angles and take -1 because we did +1 above */
		angle = (*x)[max_index] - 1.0f;

		/* Important to take -1 because indexes = sub2ind(size(P), angles, r) (a function that been used before) cannot accept r = 0, but indexes = r*180 + angles; can that */
		r = (*y)[max_index] - 1.0f;

		/* This is the trick to make sure r pointing at the right direction */
		if (r > r_half) {
			r = r - r_half;
		}else{
			r = -r;
		}

		/* Make sure that the angle is not deliberately, purposefully and exactly 90 */
		if (fabsf(90.0f - angle) < 1e-5f) {
			angle = angle + 1e-5f;
		}

		/* y = k*x + m can be expressed as x*sin(angle) + y*cos(angle) = r */
		angle = deg2rad(angle);
		(*K)[i-1] = sinf(angle) / -cosf(angle);
		(*M)[i-1] = -r / -cosf(angle);
	}
}

static size_t hough_cluster(const float P[], float* x[], float* y[], float* z[], size_t* index[], size_t* L, const float epsilon, const size_t min_pts, const size_t r_half) {
	/* Turn matrix P into 3 vectors - This is the same as [x, y, z] = find(P) in Matlab */
	const size_t r_max = 2 * r_half;
	size_t i, j, bytes, k = 0;
	for (i = 0; i < 180; i++) {
		for (j = 0; j < r_max; j++) {
			if (P[j] > 1.0f) {
				/* Add new coordinate */
				bytes = (k + 1) * sizeof(float);
				*x = (float*)realloc(*x, bytes);
				*y = (float*)realloc(*y, bytes);
				*z = (float*)realloc(*z, bytes);
				(*x)[k] = i + 1;
				(*y)[k] = j + 1;
				(*z)[k] = P[j];
				k += 1;
			}
		}
		P += r_max;
	}

	/* Sort so the longest line comes first */
	size_t* sorted_indexes = (size_t*)malloc(k * sizeof(size_t));
	sort(*z, sorted_indexes, k, 1, SORT_MODE_ROW_DIRECTION_DESCEND);
	
	/* Sort the rest */
	float* y_sorted = (float*)malloc(k * sizeof(float));
	float* x_sorted = (float*)malloc(k * sizeof(float));
	for (i = 0; i < k; i++) {
		x_sorted[i] = (*x)[sorted_indexes[i]];
		y_sorted[i] = (*y)[sorted_indexes[i]];
	}
	for (i = 0; i < k; i++) {
		(*x)[i] = x_sorted[i];
		(*y)[i] = y_sorted[i];
	}

	/* Free */
	free(sorted_indexes);
	free(x_sorted);
	free(y_sorted);
	
	/* Do dbscan - All idx values that are 0, are noise */
	float* C = (float*)malloc(k * 2 * sizeof(float));
	cat(false, *x, *y, C, k, 1, k, 1, k, 2);
	*index = (size_t*)malloc(k * sizeof(size_t));
	dbscan(C, *index, epsilon, min_pts, k, 2);

	/* Find the amount of clusters */
	size_t N = (*index)[0];
	for (i = 1; i < k; i++) {
		j = (*index)[i];
		if (j > N) {
			N = j;
		}
	}

	/* Free */
	free(C);

	/* Return N */
	*L = k;
	return N;
}

static float* hough_scores(const float X[], const float p, size_t* r_half, const size_t row, const size_t column) {
	/* Choose angles between -90 and 90 in radians with 1 degree in step */
	float K[181];
	size_t i;
	for (i = 0; i < 181; i++) {
		K[i] = -90.0f + i;
	}

	/* Compute slope K for every angle */
	K[0] = -90 - 1e5f;
	K[180] = 90 + 1e5f;
	for (i = 0; i < 181; i++) {
		K[i] = tanf(deg2rad(K[i]));
	}

	/* Compute the r_half */
	*r_half = sqrtf(row * row + column * column);

	/* Maximum r value */
	const size_t r_max = 2 * (*r_half);

	/* Maximum index */
	const size_t max_index = 180 * r_max;

	/* Create points holder P */
	const size_t P_total_size = 180 * r_max;
	float* P = (float*)malloc(P_total_size * sizeof(float));
	memset(P, 0, P_total_size * sizeof(float));

	/* Collect the points for the most common lines */
	size_t j, k, r, angle_int, c, d, index;
	float M, x, y, angle;
	size_t indexes[181];
	for (i = 0; i < row; i++) {
		for (j = 0; j < column; j++) {
			/* Check if the coordinate X(i, j) belongs to an edge */
			if (X[j] <= FLT_EPSILON) {
				/* No edge here */
				continue;
			}

			/* Iterate all angles */
			c = 0;
			for (k = 0; k < 181; k++) {
				/* Compute M from straight line equation */
				M = (j + 1.0f) - K[k] * (i + 1.0f);

				/* Find x that finds a minimal r */
				x = -K[k] * M / (1 + K[k] * K[k]);

				/* Compute y */
				y = K[k] * x + M;

				/* Compute r and make it to an integer */
				r = roundf(sqrtf(x * x + y * y));

				/* This is a special case when we want to track the direction of r */
				if (y > 0.0f) {
					r = r + *r_half;
				}

				/* Compute the angle */
				angle = atan2f(y, x);

				/* Make sure that the angles are not negative */
				if (angle < 0.0f) {
					angle += PI;
				}

				/* Turn them into degrees */
				angle_int = roundf(rad2deg(angle));

				/* Compute index */
				index = angle_int * r_max + r;

				/* Avoid indexes that are larger than max_index */
				if (index < max_index) {
					/* Save the indexes in row major */
					indexes[c++] = index;
				}
			}

			/* Check if there are duplicates */
			for (k = 0; k < c; k++) {
				for (d = 0; d < k; d++) {
					if (indexes[k] == indexes[d]) {
						break;
					}
				}
				if (d == k) {
					P[indexes[d]] += 1;
				}
			}
		}

		/* Jump to next row */
		X += column;
	}

	/* p is precent variable that describes the threshold for a line definition - Small lines avoid */
	float threshold = amax(P, &i, P_total_size) * p;
	for (i = 0; i < P_total_size; i++) {
		if (P[i] < threshold) {
			P[i] = 0.0f;
		} 
	}

	/* Return P */
	return P;
}

/*
 GNU Octave code

% Hough transform - Line detection algorthm by using hough transform and DBscan
% Input: X(Data matrix of an edge image), p(Line length threshold in precent), epsilon(Minimum radius for hough cluster), min_pts(Minimum points for hough cluster)
% Output: N(Number of lines), K(Slopes for the lines), M(Bias for the lines)
% Example 1: [N, K, M] = mi.hough(X, p, epsilon, min_pts);
% Author: Daniel Mårtensson, 14 September 2023

function [N, K, M] = hough(varargin)
  % Check if there is any input
  if(isempty(varargin))
	error('Missing inputs')
  end

  % Get data matrix X
  if(length(varargin) >= 1)
	X = varargin{1};
  else
	error('Missing data X')
  end
  % Get amount of sloped lines
  if(length(varargin) >= 2)
	p = varargin{2};
	if(p <= 0)
	  error('Line length threshold in precent needs to be above 0');
	end
	if(p > 1)
	  p = 1;
	end
  else
	error('Missing line length threshold in precent');
  end

  % Get minimum radius for hough cluster
  if(length(varargin) >= 3)
	epsilon = varargin{3};
  else
	error('Missing minimum radius for hough cluster');
  end

  % Get minimum points for hough cluster
  if(length(varargin) >= 4)
	min_pts = varargin{4};
  else
	error('Missing minimum points for hough cluster');
  end

  % Compute scores for the lines
  [P, r_half] = hough_scores(X, p);

  % Turn the scores matrix into a hough cluster
  [x, y, z, N, index] = hough_cluster(P, epsilon, min_pts);

  % Compute lines
  [K, M] = hough_lines(x, y, z, N, index, r_half);
end

function [x, y, z, N, index] = hough_cluster(P, epsilon, min_pts)
  % Turn matrix P into 3 vectors
  [x, y, z] = find(P);

  % Sort so the longest line comes first
  [z, sorted_indexes] = sort(z, 'descend');

  % Sort the rest
  x = x(sorted_indexes);
  y = y(sorted_indexes);

  % Do dbscatn - All idx values that are 0, are noise
  C = cat(2, x, y);
  index = mi.dbscan(C, epsilon, min_pts);

  % Find the amount of clusters
  N = max(index);

  % Uncomment for test
  %for i = 0:N
  %  scatter3(x(index == i), y(index == i), z(index == i));
  %  hold on
  %end
  %figure
end

function [P, r_half] = hough_scores(X, p)
  % Get the size of X
  [m, n] = size(X);

  % Choose angles between -90 and 90 in radians with 1 degree in step
  angles = -90:1:90;

  % Compute slope K for every angle
  angles(1) = -90 - 1e5;
  angles(181) = 90 + 1e5;
  K = tan(deg2rad(angles));

  % Get length of K
  K_length = length(K);

  % Compute the r_half
  r_half = floor(sqrt(m^2 + n^2));

  % Maximum r value
  r_max = 2*r_half;

  % Create points holder P
  P = zeros(180, r_max);

  % Create max index
  max_index = 180*r_max;

  % Collect the points for the most common lines
  for i = 1:m
	for j = 1:n
	  % Check if the coordinate X(i, j) belongs to an edge
	  if(X(i, j) <= eps)
		% No edge here
		continue
	  end

	  % Compute M from straight line equation
	  M = j - K*i;

	  % Find x that finds a minimal r
	  x = -K.*M./(1 + K.^2);

	  % Compute y
	  y = K.*x + M;

	  % Compute r and make it to an integer
	  r = round(sqrt(x.^2 + y.^2));

	  % This is a special case when we want to track the direction of r
	  r(y > 0) = r(y > 0) + r_half;

	  % Compute the angles
	  angles = atan2(y, x);

	  % Make sure that the angles are not negative
	  angles(angles < 0) = angles(angles < 0) + pi;

	  % Turn them into degrees and add +1 because of the indexing
	  angles = round(rad2deg(angles)) + 1;

	  % Save the indexes in column major
	  indexes = r*180 + angles;

	  % Avoid indexes that are larger than max index
	  indexes(indexes > max_index) = [];

	  % Check if there are duplicates
	  P(indexes) = P(indexes) + 1;
	end
  end

  % p is precent variable that describes the threshold for a line definition - Small lines avoid
  threshold = max(P(:)) * p;
  P(P < threshold) = 0;
end

function [K, M] = hough_lines(x, y, z, N, index, r_half)
  % Create K and M - They are holders for the output
  K = zeros(1, N);
  M = zeros(1, N);

  % Fill
  for i = 1:N
	% Find the maximum value at z-axis of a specific class ID
	[~, max_index] = max(z(index == i));

	% Get the angles and take -1 because we did +1 above
	angle = x(index == i)(max_index) - 1;

	% Important to take -1 because indexes = sub2ind(size(P), angles, r) (a function that been used before) cannot accept r = 0, but indexes = r*180 + angles; can that
	r = y(index == i)(max_index) - 1;

	% This is the trick to make sure r pointing at the right direction
	if(r > r_half)
	  r = r - r_half;
	else
	  r = -r;
	end

	% Make sure that the angle is not deliberately, purposefully and exactly 90
	if(abs(90 - angle) <= 1e-05)
	  angle = angle + 1e-05;
	end

	% y = k*x + m can be expressed as x*sin(angle) + y*cos(angle) = r
	angle = deg2rad(angle);
	K(i) = sin(angle)/-cos(angle);
	M(i) = - r/-cos(angle);
  end
end
 
*/
/*
* kmeans.c
*
*  Created on: 23 oktober 2023
*      Author: Daniel Mårtensson
*/

#include "../../Headers/functions.h"

/*
 * K-means clustering for gaussian clustering
 * X[m*n] - Data
 * idx[m] - Index of each data point
 * C[k*n] - Coordinates of centrum
 * k - Number of expected gaussian clusters 
 */
void kmeans(float X[], size_t idx[], float C[], size_t k, size_t row, size_t column) {
	/* Find a random initial start */
	size_t* x = (float*)malloc(k * sizeof(size_t));
	randperm(x, row, k);
	const size_t k_column = k * column;
	float* C0 = C;
	float* X0 = X;
	size_t i, j, l, m, counter;
	for (i = 0; i < k; i++) {
		memcpy(C, X + x[i] * column, column * sizeof(float));
		C += column;
	}
	C = C0;

	/* Free */
	free(x);

	/* Copy X to X_copy */
	float* X_copy = (float*)malloc(row * column * sizeof(float));
	memcpy(X_copy, X, row * column * sizeof(float));
	tran(X_copy, row, column);
	float* X_copy0 = X_copy;

	/* Iterate the solution */
	float old_value = 0.0f, min_value, new_value, difference;
	float* D = (float*)malloc(row * k * sizeof(float));
	float* D0 = D;
	float s;
	for (i = 0; i < MAX_ITERATIONS; i++) {
		/* Compute euclidean distance */
		pdist2(X, C, D, row, column, k, PDIST2_METRIC_SQEUCLIDEAN);
		
		/* Create points to nearest cluster */
		for (j = 0; j < row; j++) {
			/* Initial value */
			min_value = D[0];
			idx[j] = 0;

			/* Scan and see if they can be replaced */
			for (l = 1; l < k; l++) {
				if (D[l] < min_value) {
					min_value = D[l];
					idx[j] = l;
				}
			}
			D += k;
		}
		D = D0;

		/* Update the center of the cluster coordinates */
		for (j = 0; j < k; j++) {
			X_copy = X_copy0;
			for (l = 0; l < column; l++) {
				/* Initial */
				s = 0.0f;
				counter = 0;
				/* Find C(j, :) = mean(X(idx == j, :), 1) */
				for(m = 0; m < row; m++){
					if (idx[m] == j) {
						s += X_copy[m];
						counter++;
					}
				}

				/* Average */
				C[l] = s / ((float)counter);

				/* Next column - Becausie it's transposed */
				X_copy += row;
			}

			/* Next row */
			C += column;
		}
		C = C0;

		/* Check if we are going to break the iteration */
		new_value = 0.0f;
		for (j = 0; j < k_column; j++) {
			new_value += C[j];
		}
		difference = fabsf(old_value - new_value);
		if (difference < MIN_VALUE) {
			break;
		}
		else {
			old_value = new_value;
		}
	}

	/* Reset */
	X_copy = X_copy0;

	/* Free */
	free(D);
	free(X_copy);
}

/*
 GNU octave code

 % K-means clustering
% Input: X(data), k(amount of clusters)
% Output: idx(row index of class ID), C(coordinates of the centers of the cluster coordinates)
% Example 1: [idx, C] = mi.kmeans(X, k)
% Author: Daniel Mårtensson, April 2023

function [idx, C] = kmeans(X, k)
  % Get size of X
  [m, n] = size(X);

  % Find a random initial start
  t = [292   691   467];
  C = X(t, :);


  % Iterate the solution
  max_iter = 100;
  old_value = 0;
  for i = 1:max_iter
	  % Compute euclidean distance
	  D = euclidean_distance(X, C);

	  % Create points to nearest cluster
	  [~, idx] = min(D, [], 2);

	  % Update the center of the cluster coordinates
	  for j = 1:k
		  C(j,:) = mean(X(idx == j, :), 1);
	  end

	  % Check if we are going to break the iteration
	  new_value = sum(C(:));
	  C
	  difference = abs(old_value - new_value)
	  if(difference < 1e-11)
		break;
	  end
	  old_value = new_value;
  end
end

function D = euclidean_distance(X, Y)
  Yt = Y';
  XX = sum(X.*X,2);
  YY = sum(Yt.*Yt,1);
  D = XX + YY - 2*X*Yt;
end
*/
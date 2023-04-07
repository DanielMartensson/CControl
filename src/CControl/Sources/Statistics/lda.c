/*
 * lda.c
 *
 *  Created on: 25 mars 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

static void average_vector(float A[], float mu[], uint16_t row, uint16_t column);
static void center_data(float X[], float mu[], uint16_t row, uint16_t column);

/*
  * Linear Discriminant Analysis (LDA)
  * This compute the LDA of X and return W and P
  * X[m*n]
  * y[n]
  * W[m*components]
  * P[components*n]
  */

void lda(float X[], uint8_t y[], float W[], float P[], uint8_t components, uint16_t row, uint16_t column) {
	/* Create average vector mu_X = mean(X, 2) */
	float* mu_X = (float*)malloc(row * sizeof(float));
	average_vector(X, mu_X, row, column);

	/* Count classes */
	uint8_t amount_of_classes = y[column - 1] + 1;

	/* Create scatter matrices Sw and Sb */
	uint32_t scatter_matrix_size = row * row;
	float* Sw = (float*)malloc(scatter_matrix_size * sizeof(float));
	memset(Sw, 0, scatter_matrix_size * sizeof(float));
	float* Sb = (float*)malloc(scatter_matrix_size * sizeof(float));
	float* Sb0 = Sb;
	memset(Sb, 0, scatter_matrix_size * sizeof(float));

	/* How many samples of each class */
	uint8_t i;
	uint8_t* samples_of_each_class = (uint8_t*)malloc(amount_of_classes);
	memset(samples_of_each_class, 0, amount_of_classes);
	for (i = 0; i < column; i++) {
		samples_of_each_class[y[i]]++;
	}

	/* Iterate all classes */
	float* Xi, *XiT, *Xi0, *X0 = X;
	float *XiXiT = (float*)malloc(row * row * sizeof(float));
	float *mu_Xi = (float*)malloc(row * sizeof(float));
	float *diff = (float*)malloc(row * sizeof(float));
	uint8_t j, samples_of_class;
	uint16_t k;
	uint32_t l;
	for (i = 0; i < amount_of_classes; i++) {
		/* Get samples of each class */
		samples_of_class = samples_of_each_class[i];

		/* Create a part of X that we calling Xi */
		Xi = (float*)malloc(row * samples_of_class * sizeof(float));
		Xi0 = Xi;

		/* Initial shift */
		X = X0;
		for (j = 0; j < i; j++) {
			X += samples_of_each_class[j];
		}

		/* Copy a class to Xi from X */
		for (k = 0; k < row; k++) {
			memcpy(Xi, X, samples_of_class * sizeof(float));

			/* Shift to next row */
			Xi += samples_of_class;
			X += column;
		}

		/* Reset Xi to zero index */
		Xi = Xi0; 

		/* Get the average of Xi */
		average_vector(Xi, mu_Xi, row, samples_of_class);

		/* Center Xi */
		center_data(Xi, mu_Xi, row, samples_of_class);
		
		/* Copy Xi and transpose Xi to XiT and turn XiT into transpose */
		XiT = (float*)malloc(row * samples_of_class * sizeof(float));
		memcpy(XiT, Xi, row * samples_of_class * sizeof(float));
		tran(XiT, row, samples_of_class);

		/* Create XiXiT = Xi*Xi' */
		mul(Xi, XiT, XiXiT, row, samples_of_class, row);

		/* Add to Sw scatter matrix */
		for (l = 0; l < scatter_matrix_size; l++) {
			Sw[l] += XiXiT[l];
		}

		/* Calculate difference */
		for (k = 0; k < row; k++) {
			diff[k] = mu_Xi[k] - mu_X[k];
		}

		/* Borrow this matrix and do XiXiT = diff*diff' */
		mul(diff, diff, XiXiT, row, 1, row);
		
		/* Add to Sb scatter matrix - Important to multiply XiXiT with samples of class */
		for (l = 0; l < scatter_matrix_size; l++) {
			Sb[l] += XiXiT[l] * samples_of_class; 
		}

		/* Free */
		free(Xi);
		free(XiT);
	}

	/* Do Generalized Eigenvalue Problem */
	float* d = (float*)malloc(row * sizeof(float));
	eig_sym_generalized(Sb, Sw, row, d);

	/* Copy over eigenvectors from Sb to W */
	float* W0 = W;
	for (l = 0; l < row; l++) {
		for (k = 0; k < components; k++) {
			W[k] = Sb[row - 1 - k]; /* Sb are in an ascending order */
		}
		
		Sb += row;
		W += components;
	}

	/* Project P = W'*X */
	X = X0;
	W = W0;
	tran(W, row, components);
	mul(W, X, P, components, row, column);
	tran(W, components, row);

	/* Reset */
	Sb = Sb0;

	/* Free */
	free(mu_X);
	free(Sw);
	free(Sb);
	free(samples_of_each_class);
	free(mu_Xi);
	free(XiXiT);
	free(diff);
	free(d);
}

static void average_vector(float X[], float mu[], uint16_t row, uint16_t column) {
	uint16_t i;
	for (i = 0; i < row; i++) {
		/* Average data mu = mean(X, 2) */
		mu[i] = mean(X, column);

		/* Jump to next row */
		X += column;
	}
}

static void center_data(float X[], float mu[], uint16_t row, uint16_t column) {
	uint16_t i, j;
	for (i = 0; i < row; i++) {
		/* Center the data X = X - mu */
		for (j = 0; j < column; j++) {
			X[j] -= mu[i];
		}

		/* Jump to next row */
		X += column;
	}
}

/*
 * GNU Octave code:
 * 
    % First clear
	clear all
	clc

	% Set size
	column = 15;
	row = 7;
	components = 2;

	% Data
	X = [1.2728,     -0.36547,      0.68505,      -1.0574,      -1.2045,     -0.92432,       1.4365,     -0.16106,     -0.48166,  -1.8685,       -0.694,     0.070631,      0.64477,     -0.68641,     0.089715,
	   -0.43468,     -0.79101,     0.026709,      0.93476,      -1.0388,      -1.3911,       -1.952,      0.55584,      -1.0549, -0.72454,       1.2319,      -1.5877,      0.83961,     -0.13554,     -0.50264,
	   -0.58425,     -0.28557,       -1.126,      -1.2017,      0.62607,     -0.68838,      0.12606,      0.64138,      0.86003, -0.16475,       1.3645,      0.20788,      0.49795,      -1.4394,      0.33117,
	  -0.027078,      0.82297,       1.0295,        1.865,      0.63335,    -0.020842,     -0.97425,     -0.74584,      -1.3266, -0.29814,       1.2731,     -0.74892,      -1.8926,    -0.082449,     -0.36409,
	    0.63059,      0.85787,     -0.78536,       2.2746,     -0.76871,     -0.74203,      0.65182,    -0.012599,       1.4966, -0.84699,     -0.23234,      0.23088,     -0.48266,     -0.21415,      -0.4839,
	    0.32812,      0.22213,     -0.18081,      -2.7175,      0.17749,      0.16662,       1.5695,     -0.50224,     0.055364, 0.087841,      0.52972,      -1.5707,      0.25717,      -1.1804,     -0.41628,
	    -0.3845,      0.84373,     -0.26776,     -0.41406,      0.86366,       1.0458,      0.92923,     -0.36558,      0.17761,   1.2917,     -0.74526,      0.83441,      -1.1806,     -0.39902,      -1.4924];

	% Class ID
	y = [0,0,0,0,1,1,1,1,2,2,3,3,3,3,4];

	% Create average vector mu_X = mean(X, 2)
	mu_X = mean(X, 2);

	% Count classes
	amount_of_classes = y(end) + 1;

	% Create scatter matrices Sw and Sb
	Sw = zeros(row, row);
	Sb = zeros(row, row);

	% How many samples of each class
	samples_of_each_class = zeros(1, amount_of_classes);
	for i = 1:column
	  samples_of_each_class(y(i) + 1) = samples_of_each_class(y(i) + 1) + 1; % Remove +1 if you are using C
	end

	% Iterate all classes
	shift = 1;
	for i = 1:amount_of_classes
	  % Get samples of each class
	  samples_of_class = samples_of_each_class(i);

	  % Copy a class to Xi from X
	  Xi = X(:, shift:shift+samples_of_class - 1);

	  % Shift
	  shift = shift + samples_of_class;

	  % Get average of Xi
	  mu_Xi = mean(Xi, 2);

	  % Center Xi
	  Xi = Xi - mu_Xi;

	  % Copy Xi and transpose Xi to XiT and turn XiT into transpose
	  XiT = Xi';

	  % Create XiXiT = Xi*Xi'
	  XiXiT = Xi*XiT;

	  % Add to Sw scatter matrix
	  Sw = Sw + XiXiT;

	  % Calculate difference
	  diff = mu_Xi - mu_X;

	  % Borrow this matrix and do XiXiT = diff*diff'
	  XiXiT = diff*diff';

	  % Add to Sb scatter matrix - Important to multiply XiXiT with samples of class
	  Sb = Sb + XiXiT*samples_of_class;
	end

	% Find the eigenvectors - by solving the generalized eigenvalue problem: Sb*v = Sw*v*lambda
	L = chol(Sw, 'lower');
	Y = linsolve(L, Sb);
	Z = Y*inv(L');
	[V, D] = eig(Z);

	% Sort eigenvectors descending by eigenvalue
	[D, idx] = sort(diag(D), 1, 'descend');
	V = V(:,idx);

	% Get components W and project the matrix P
	W = V(:, 1:components)
	P = W'*X
 */
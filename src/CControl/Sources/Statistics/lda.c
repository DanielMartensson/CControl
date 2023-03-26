/*
 * lda.c
 *
 *  Created on: 25 mars 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

static void average_vector(float A[], float mu[], uint16_t row, uint16_t column);
static void center_data(float X[], float mu[], uint16_t row, uint16_t column);

void lda(float X[], uint8_t y[], uint8_t components, uint16_t row, uint16_t column) {
	/* Create average vector mu_X = mean(X, 2) */
	float* mu_X = (float*)malloc(row * sizeof(float));
	average_vector(X, mu_X, row, column);

	/* Count classes */
	uint8_t amount_of_classes = y[column - 1] + 1;

	/* Create scatter matrices Sw and Sb */
	uint32_t scatter_matrix_size = column * column;
	float* Sw = (float*)malloc(scatter_matrix_size * sizeof(float));
	memset(Sw, 0, scatter_matrix_size * sizeof(float));
	float* Sb = (float*)malloc(scatter_matrix_size * sizeof(float));
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
	
	/* Free */
	free(mu_X);
	free(Sw);
	free(Sb);
	free(samples_of_each_class);
	free(mu_Xi);
	free(XiXiT);
	free(diff);
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
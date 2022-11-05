/*
 * pf.c
 *
 *  Created on: 31 okt. 2022
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Particle filter
 * x[m] Your unfiltered state vector measurement
 * xhat[m] Your filtered state vector measurement
 * xhatp[m] Previous state vector measurement
 * horizon[m*p] This is how back we look for the previous measurement
 * noise[m*p] This is the estimation error as noise. Used for the deviation for the kernel density estimation
 * m This is the row dimension
 * k This is a number, begins at 0 and will count up to number p
 * p This is a fixed number for the length of horizon and noise. See this variable as a tuning parameter for the particle filter
 */


static void shift_matrix(float matrix[], float x[], uint8_t p, uint8_t *k, uint8_t m);
static void kernel_density_estimation(float P[], float H[], float horizon[], float noise[], uint8_t m, uint8_t p);
static float normal_pdf(float x, float mu, float sigma);

void pf(float x[], float xhat[], float xhatp[], float horizon[], float noise[], uint8_t m, uint8_t p, uint8_t *k){

	// Horizon matrix shifting
	shift_matrix(horizon, x, p, k, m);

	// Compute the kernel density estimation from horizon
	float P[m*p];
	float H[m*p];
	kernel_density_estimation(P, H, horizon, noise, m, p);

	// Save address
	float *P0 = P;
	float *H0 = H;

	// Estimate the next value
	uint8_t index = 0;
	float error, min_error, ratio, diff, e[m];
	for(uint8_t i = 0; i < m; i++){
		// Find  the index that has the lowest error
		min_error = fabsf(x[i] - H0[0]);
		for(uint8_t j = 1; j < p; j++){
			error = fabsf(x[i] - H0[j]);
			if(error < min_error){
				min_error = error;
				index = j;
			}
		}

		/* Compute the ratio (0.5-1.0)
		 * If P[i*p + index] = 1 (100%), then ratio = 0.5 (Good)
		 * If P[i*p + index] = 0 (0%), then ratio = 1.0 (Problem, bad kernel density estimation...)
		 */
		if(fabsf(x[i]) > 0.0f){
			ratio = x[i]/(x[i] + x[i]*P0[index]);
		}else{
			ratio = 0.5f;
		}

		// Difference between old and new
		diff = x[i] - xhatp[i];

		// This a smoother filtering
		horizon[*k] = horizon[*k]*fabsf(diff);

		// Update state. It MUST be negative
		xhat[i] = x[i] - ratio*diff;

		// Compute the noise
		e[i] = xhat[i] - x[i];

		// Shift row
		horizon += p;
		P0 += p;
		H0 += p;
	}

	// Noise matrix shifting
	shift_matrix(noise, e, p, k, m);

	// Next matrix column
	if(*k < p-1){
		*k += 1;
	}
}

static void shift_matrix(float matrix[], float x[], uint8_t p, uint8_t *k, uint8_t m){
	// Matrix shifting
	for(uint8_t i = 0; i < m; i++){
		// Shift
		if(*k >= p-1){
			for(uint8_t j = 0; j < p-1; j++){
				matrix[j] = matrix[j+1];
			}
		}

		// Add the last
		matrix[*k] = x[i];

		// Shift
		matrix += p;
	}
}

static void kernel_density_estimation(float P[], float H[], float horizon[], float noise[], uint8_t m, uint8_t p){
	// Save address
	float *P0 = P;

	// Sort smallest values first because H is like a histogram with only one count for each values
	memcpy(H, horizon, m*p*sizeof(float));
	sort(H, m, p, 2, 1);

	// Create empty array
	memset(P, 0, m*p*sizeof(float));

	// Variables
	float sigma;

	// Loop every row of P
	for(uint8_t i = 0; i < m; i++){
		// Create sigma std
		sigma = stddev(&noise[0], p);

		for(uint8_t j = 0; j < p; j++){
			// Fill the array P(:, k)
			for(uint8_t k = 0; k < p; k++){
				P0[k] += normal_pdf(H[k], H[j], sigma);
			}
		}

		// Shift to next row
		noise += p;
		H += p;
		P0 += p;
	}

	// Turn P into a probability matrix
	P0 = P; // Reset
	float y[1];
	for(uint8_t i = 0; i < m; i++){
		// Do a sum of P
		sum(&P0[0], y, 1, p, 2); // Column direction, get one value back
		for(uint8_t j = 0; j < p; j++){
			P0[j] /= y[0];
		}

		// Shift to next row
		P0 += p;
	}
}

static float normal_pdf(float x, float mu, float sigma){
	return 1.0f/(sigma*sqrt(2.0f*PI))*expf(-1.0f/2.0f*(x-mu)*(x-mu)/(sigma*sigma));
}

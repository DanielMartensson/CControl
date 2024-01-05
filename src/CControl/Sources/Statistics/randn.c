/*
 * randn.c
 *
 *  Created on: 7 juni 2021
 *      Author: Daniel Mårtensson
 */

#include "statistics.h"

static float generate_gauss(float mu, float sigma);
static bool has_been_called = false;

/*
 * Compute Gaussian Distribution
 * Vector: x[length]
 * Mean: mu
 * Standard deviation: sigma
 */
void randn(float x[], size_t length, float mu, float sigma){
	if(!has_been_called){
		srand(time(NULL));
		has_been_called = true;
	}
	size_t i;
	for (i = 0; i < length; i++) {
		x[i] = generate_gauss(mu, sigma);
	}
}

static float generate_gauss(float mu, float sigma) {
	float U1, U2, W, scalar;
	static float X1, X2;
	static int32_t call = 0;

	if (call == 1) {
		call = !call;
		return (mu + sigma * (float) X2);
	}

	/* Compute the uniform norm */
	do {
		U1 = -1 + ((float) rand() / RAND_MAX) * 2;
		U2 = -1 + ((float) rand() / RAND_MAX) * 2;
		W = powf(U1, 2) + powf(U2, 2);
	} while (W >= 1 || W == 0);

	scalar = sqrtf((-2 * logf(W)) / W);
	X1 = U1 * scalar;
	X2 = U2 * scalar;

	call = !call;

	return (mu + sigma * (float) X1);
}

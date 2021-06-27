/*
 * randn.c
 *
 *  Created on: 7 juni 2021
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

static float generate_gauss(float mu, float sigma);

/*
 * Compute Gaussian Distribution
 * Vector: x[length]
 * Mean: mu
 * Standard deviation: sigma
 */
void randn(float x[], uint16_t length, float mu, float sigma){
	srand(time(NULL));
	for(uint16_t i = 0; i < length; i++)
		x[i] = generate_gauss(mu, sigma);
}

static float generate_gauss(float mu, float sigma) {
	float U1, U2, W, scalar;
	static float X1, X2;
	static int call = 0;

	if (call == 1) {
		call = !call;
		return (mu + sigma * (float) X2);
	}

	// Compute the uniform norm
	do {
		U1 = -1 + ((float) rand() / RAND_MAX) * 2;
		U2 = -1 + ((float) rand() / RAND_MAX) * 2;
		W = pow(U1, 2) + pow(U2, 2);
	} while (W >= 1 || W == 0);

	scalar = sqrt((-2 * log(W)) / W);
	X1 = U1 * scalar;
	X2 = U2 * scalar;

	call = !call;

	return (mu + sigma * (float) X1);
}

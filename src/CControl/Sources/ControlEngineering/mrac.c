/*
 * mrac.c
 *
 *  Created on: 21 Juli 2020
 *      Author: Daniel MÃ¥rtensson
 */


#include "../../Headers/Functions.h"

static void integral(float I[], float gain, float x[], float e[], uint8_t RDIM);
static void saturate(float I[], float limit, uint8_t RDIM);
static void modelerror(float e[], float y[], float r[], uint8_t RDIM);
static void findinput(float u[], float r[], float I1[], float y[], float I2[], uint8_t RDIM);

/*
 * This is Adaptive Model Reference Control - We assume that the reference model is Gm(s) = 1.
 *
 * u = r*integral(-gain*r*e) - y*integral(gain*y*e);
 * Where: u = input to our system
 *      : r = reference
 *      : y = output
 *      : gain = learning factor e.g 0.00001
 *      : e = Model error y-r
 *
 * This can be used with multiple outputs and references
 * HINT: Look up my repository Adaptive-Control and look for Model Reference Adaptive Control with Lyapunov rule
 * HINT: The book Adaptive Control by Karl-Johan Åström describe on page 208 how this works. ISBN: 9780486462783
 */
void mrac(float limit, float gain, float y[], float u[], float r[], float I1[], float I2[], uint8_t RDIM){

	// Find the model error
	float e[RDIM];
	modelerror(e, y, r, RDIM);

	// Integrate
	integral(I1, -gain, r, e, RDIM); // I1 = I1 - gain*r*e
	integral(I2, gain, y, e, RDIM); // I2 = I2 + gain*y*e

	// Saturate
	saturate(I1, limit, RDIM);
	saturate(I2, limit, RDIM);

	// Find input signal
	findinput(u, r, I1, y, I2, RDIM);
}

static void integral(float I[], float gain, float x[], float e[], uint8_t RDIM){
	for(uint8_t i = 0; i < RDIM; i++)
		I[i] += gain * x[i] * e[i];
}

static void saturate(float I[], float limit, uint8_t RDIM){
	for(uint8_t i = 0; i < RDIM; i++)
		I[i] = saturation(I[i], -limit, limit);
}

static void modelerror(float e[], float y[], float r[], uint8_t RDIM){
	for(uint8_t i = 0; i < RDIM; i++)
		e[i] = y[i] - r[i];
}

static void findinput(float u[], float r[], float I1[], float y[], float I2[], uint8_t RDIM){
	for(uint8_t i = 0; i < RDIM; i++)
		u[i] = r[i] * I1[i] - y[i] * I2[i];
}

/*
 * filtfilt.c
 *
 *  Created on: 29 april 2020
 *      Author: Daniel Mårtensson
 */

#include "signalprocessing.h"

static void simulation(float K, float y[], float t[], size_t l);
static void flip(float y[], size_t l);

/*
 * This filter a array. Very simple method, but powerful
 * y [l]
 * t [l]
 * K > 0, but small number
 * Returns y as filtered
 */
void filtfilt(float y[], const float t[], const size_t l, const float K) {
	/* Simulate */
    simulation(K, y, t, l);

    /* Flip y */
   	flip(y, l);

   	/* Run the simulation again */
	simulation(K, y, t, l);

   	/* Flip again - Done */
   	flip(y, l);
}

/* Euler method for simple ODE - Low pass filter */
static void simulation(float K, float y[], float t[], size_t l) {
	float h = t[1] - t[0]; /* Time step */
	float x = y[0]; /* Initial state */
	size_t i;
	for (i = 0; i < l; i++) {
		x = x + h * (-1.0f / K * x + 1.0f / K * y[i]);
		y[i] = x; /* Save */
	}
}

/* Flip signal */
static void flip(float y[], size_t l){
	size_t i;
	const size_t half = l / ((size_t)2);
	for(i = 0; i < half; i++){
        	float temp = y[i];
        	y[i] = y[l - 1 - i];
        	y[l - 1 - i] = temp;
	}
}

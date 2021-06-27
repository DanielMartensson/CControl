/*
 * filtfilt.c
 *
 *  Created on: 29 april 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

static void simulation(float K, float y[], float t[], uint16_t l);
static void flip(float y[], uint16_t l);

/*
 * This filter a array. Very simple method, but powerful
 * y [l]
 * t [l]
 * K > 0, but small number
 * Returns y as filtered
 */
void filtfilt(float y[], float t[], uint16_t l, float K) {
	// Simulate
    simulation(K, y, t, l);

    // Flip y
   	flip(y, l);

   	// Run the simulation again
	simulation(K, y, t, l);

   	// Flip again - Done
   	flip(y, l);
}

// Euler method for simple ODE - Low pass filter
static void simulation(float K, float *y, float *t, uint16_t l) {
	float h = t[1] - t[0]; // Time step
	float x = y[0]; // Initial state
	for (uint16_t i = 0; i < l; i++) {
		x = x + h * (-1 / K * x + 1 / K * y[i]);
		y[i] = x; // Save
	}
}

// Flip signal
static void flip(float y[], uint16_t l){
	for(uint16_t i = 0; i < l/2; i++){
        	float temp = y[i];
        	y[i] = y[l - 1 - i];
        	y[l - 1 - i] = temp;
	}
}

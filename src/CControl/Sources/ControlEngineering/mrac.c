/*
 * mrac.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Configurations.h"
#include "../../Headers/Functions.h"

/*
 * This is Adaptive Model Reference Control - No need to use a transfer function here
 *
 * u = -LEARNING*r*integral(r*y - r*r) - LEARNING*y*integral(y*y - y*r);
 * Where: u = input to our system
 *      : r = reference
 *      : y = output
 *      : LEARNING = learning factor e.g 0.00001
 *      : K1 = integral(r*e) = integral(r*y - r*r)
 *      : K2 = integral(y*e) = integral(y*y - y*r);
 *      : e = Model error y - r
 *
 * This can be used with multiple outputs and references
 * HINT: Look up my repository Adaptive-Control and look for Model Reference Adaptive Control with Lyapunov rule
 */
void mrac(float* y, float* u, float* r, float* K1, float* K2){

	// First create these vectors
	float ry[RDIM];
	float rr[RDIM];
	float yy[RDIM];
	float yr[RDIM];

	// Then compute the product
	for(int i = 0; i < RDIM; i++){
		*(ry + i) = *(r + i) * *(y + i);
		*(rr + i) = *(r + i) * *(r + i);
		*(yy + i) = *(y + i) * *(y + i);
		*(yr + i) = *(y + i) * *(r + i);
	}

	// Integrate K = K + y - r
	integral(K1, ry, rr);
	integral(K2, yy, yr);

	// Compute u now
	for(int i = 0; i < RDIM; i++){
		*(u + i) = -LEARNING * *(r + i) * *(K1 + i) - LEARNING * *(y + i) * *(K2 + i);
	}
}



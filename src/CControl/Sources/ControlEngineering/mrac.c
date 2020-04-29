/*
 * mrac.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Configurations.h"
#include "../../Headers/Functions.h"

static void integral(uint8_t ANTI_WINDUP, float* xi, float* r, float* y, uint8_t ADIM, uint8_t YDIM, uint8_t RDIM);

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
void mrac(uint8_t ANTI_WINDUP, float LEARNING, float* y, float* u, float* r, float* K1, float* K2, uint8_t ADIM, uint8_t YDIM, uint8_t RDIM){

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
	integral(ANTI_WINDUP, K1, ry, rr, ADIM, YDIM, RDIM);
	integral(ANTI_WINDUP, K2, yy, yr, ADIM, YDIM, RDIM);

	// Compute u now
	for(int i = 0; i < RDIM; i++){
		*(u + i) = -LEARNING * *(r + i) * *(K1 + i) - LEARNING * *(y + i) * *(K2 + i);
	}
}

/*
 * This computes the integral by sum state vector xi with reference - measurement
 * xi = xi + r - y;
 */
static void integral(uint8_t ANTI_WINDUP, float* xi, float* r, float* y, uint8_t ADIM, uint8_t YDIM, uint8_t RDIM) {
	for(int i = 0; i < RDIM; i++){
		/*
		 * Anti-windup
		 */
		if(ANTI_WINDUP == 0){
			*(xi + i) = *(xi + i) + *(r + i) - *(y + i); // Always integrate
		}else if(ANTI_WINDUP == 1){
			if(*(r + i) > *(y + i)){
				*(xi + i) = *(xi + i) + *(r + i) - *(y + i); // Only integrate when r > y, else delete
			}else{
				*(xi + i) = 0; // Delete just that xi
			}
		}else if(ANTI_WINDUP == 2){
			if(*(r + i) > *(y + i)){
				*(xi + i) = *(xi + i) + *(r + i) - *(y + i); // Only integrate r > y, else stop
			}
		}else{
			*(xi + i) = *(xi + i) + *(r + i) - *(y + i); // Always integrate if nothing else selected
		}
	}
}



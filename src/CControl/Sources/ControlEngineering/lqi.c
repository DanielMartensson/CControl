/*
 * lqi.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

static void integral(uint8_t ANTI_WINDUP, float xi[], float r[], float y[], uint8_t RDIM);

/*
 * This computes the Linear Quadratic Integral inputs
 * Measurement vector: y[YDIM]
 * Input signal vector: u[RDIM]
 * Integral scalar: qi
 * Reference vector: r[RDIM]
 * LQR control law L[RDIM*ADIM]
 * LQI integral law: Li[RDIM*ADIM]
 * State vector: x[ADIM]
 * Integral vector: xi[YDIM]
 */
void lqi(float y[], float u[], float qi, float r[], float L[], float Li[], float x[], float xi[], uint8_t ADIM, uint8_t YDIM, uint8_t RDIM, uint8_t ANTI_WINDUP) {

	// First compute the control law: L_vec = L*x
	float L_vec[RDIM*1];
	mul(L, x, L_vec, RDIM, ADIM, 1);

	// Then compute the integral law: Li_vec = Li*xi
	float Li_vec[RDIM];
	integral(ANTI_WINDUP, xi, r, y, RDIM);
	mul(Li, xi, Li_vec, RDIM, YDIM, 1);

	// Now combine these two laws: u = Li/(1-qi)*r - (L*x - Li*xi)
	for(uint8_t i = 0; i < RDIM; i++){
		u[i] = Li[i*RDIM]/(1-qi) * r[i] - (L_vec[i] - Li_vec[i]);
	}
}

/*
 * This computes the integral by sum state vector xi with reference - measurement
 * xi = xi + r - y;
 */
static void integral(uint8_t ANTI_WINDUP, float xi[], float r[], float y[], uint8_t RDIM) {
	for(uint8_t i = 0; i < RDIM; i++){
		/*
		 * Anti-windup
		 */
		if(ANTI_WINDUP == 0){
			xi[i] = xi[i] + r[i] - y[i]; 		// Always integrate
		}else if(ANTI_WINDUP == 1){
			if(r[i] > y[i]){
				xi[i] = xi[i] + r[i] - y[i]; 	// Only integrate when r > y, else delete
			}else{
				xi[i] = 0; 						// Delete just that xi
			}
		}else if(ANTI_WINDUP == 2){
			if(r[i] > y[i]){
				xi[i] = xi[i] + r[i] - y[i]; 	// Only integrate r > y, else do nothing
			}
		}else{
			xi[i] = xi[i] + r[i] - y[i]; 		// Always integrate if nothing else selected
		}
	}
}

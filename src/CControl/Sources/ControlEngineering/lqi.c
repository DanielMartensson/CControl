/*
 * lqi.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Configurations.h"
#include "../../Headers/Functions.h"
/*
 * This computes the Linear Quadratic Integral inputs
 * Input: Vector y, Vector u, Scalar qi, Vector r, Matrix L, Matrix Li, Vector x, Vecor xi
 */
void lqi(float* y, float* u, float qi, float* r, float* L, float* Li, float* x, float* xi) {

	// First compute the control law: L_vec = L*x
	float L_vec[RDIM*1];
	memset(L_vec, 0, RDIM*sizeof(float));
	mul(L, x, L_vec, RDIM, ADIM, 1);

	// Then compute the integral law: Li_vec = Li*xi
	float Li_vec[RDIM];
	memset(Li_vec, 0, RDIM*sizeof(float));
	integral(xi, r, y);
	mul(Li, xi, Li_vec, RDIM, YDIM, 1);

	// Now combine these two laws: u = Li/(1-qi)*r - (L*x - Li*xi)
	for(int i = 0; i < RDIM; i++){
		*(u + i) = *(Li + i*RDIM)/(1-qi) * *(r + i) - (*(L_vec + i) - *(Li_vec + i));
	}
}



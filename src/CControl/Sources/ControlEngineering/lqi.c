/*
 * lqi.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

static void integral(size_t ANTI_WINDUP, float xi[], float r[], float y[], size_t RDIM);

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
void lqi(float y[], float u[], float qi, float r[], float L[], float Li[], float x[], float xi[], size_t ADIM, size_t YDIM, size_t RDIM, ANTI_WINUP anti_winup) {

	/* First compute the control law: L_vec = L*x */
	float *L_vec = (float*)malloc((RDIM + 1) * sizeof(float));
	mul(L, x, L_vec, RDIM, ADIM, 1);

	/* Then compute the integral law: Li_vec = Li*xi */
	float *Li_vec = (float*)malloc(RDIM * sizeof(float));
	integral(anti_winup, xi, r, y, RDIM);
	mul(Li, xi, Li_vec, RDIM, YDIM, 1);

	/* Now combine these two laws: u = Li/(1-qi)*r - (L*x - Li*xi) */
	size_t i;
	for(i = 0; i < RDIM; i++){
		u[i] = Li[i*RDIM]/(1-qi) * r[i] - (L_vec[i] - Li_vec[i]);
	}

	/* Free */
	free(L_vec);
	free(Li_vec);
}

/*
 * This computes the integral by sum state vector xi with reference - measurement
 * xi = xi + r - y;
 */
static void integral(ANTI_WINUP anti_winup, float xi[], float r[], float y[], size_t RDIM) {
	size_t i;
	for(i = 0; i < RDIM; i++){
		/*
		 * Anti-windup
		 */
		switch (anti_winup) {
		case ANTI_WINUP_ALWAYS_INTEGRATE:
			xi[i] = xi[i] + r[i] - y[i];		/* Always integrate */
			break;
		case ANTI_WINUP_ONLY_INTEGRATE_WHEN_R_GRATER_THAN_Y_ELSE_DELETE:
			if (r[i] > y[i]) {
				xi[i] = xi[i] + r[i] - y[i]; 	/* Only integrate when r > y, else delete */
			}
			else {
				xi[i] = 0.0f; 					/* Delete just that xi */
			}
			break;
		case ANTI_WINUP_ONLY_INTEGRATE_WHEN_R_Y:
			if (r[i] > y[i]) {
				xi[i] = xi[i] + r[i] - y[i]; 	/* Only integrate r > y, else do nothing */
			}
			break;
		default:
			xi[i] = xi[i] + r[i] - y[i]; 		/* Always integrate if nothing else selected */
			break;
		}
	}
}

/*
 * rls.c
 *
 *  Created on: 5 oct. 2019
 *      Author: Daniel Mårtensson
 */

#include "systemidentification.h"

static void recursive(size_t NP, size_t NZ, size_t NZE, float y, float phi[], float theta[], float P[], float* past_e, float forgetting);

/*
 * Recursive least square. We estimate A(q)y(t) = B(q) + C(q)e(t)
 * theta [NP + NZ + NZE]
 * P [(NP + NZ + NZE)*(NP + NZ + NZE)]
 * phi [NP + NZ + NZE]
 * Pq > 0
 * 0 < forgetting <= 1
 */
void rls(size_t NP, size_t NZ, size_t NZE, float theta[], float u, float y, size_t* count, float* past_e, float* past_y, float* past_u, float phi[], float P[], float Pq, float forgetting) {

	/* Static values that belongs to this function - OLD CODE, but they have the same size */
	/* static float past_e = 0; // The past e */
	/* static float past_y = 0; // The past y */
	/* static float past_u = 0; // The past u */
	/* static float phi[NP + NZ + NZE]; */
	/* static float P[(NP + NZ + NZE)*(NP + NZ + NZE)]; */

	/* Decleration */
	int8_t i; /* This must be signed */

	if(*count == 0){
		/* Nothing here - Leave phi with only zeros - Important to have phi as zeros */
		memset(phi, 0, (NP + NZ + NZE) * sizeof(float));

		/* Init P with zeros and then create P as an identify matrix with q as diagonal */
		memset(P, 0, (NP + NZ + NZE)*(NP + NZ + NZE) * sizeof(float)); /* Initial P with zeros */
		for(i = 0; i < NP + NZ + NZE; i++){
			P[i*(NP + NZ + NZE) + i] = Pq;
		}

		/* Reset the past */
		*past_y = 0;
		*past_u = 0;
		*past_e = 0;

		/* Reset phi and theta */
		memset(phi, 0, (NP + NZ + NZE)*sizeof(float));
		memset(theta, 0, (NP + NZ + NZE)*sizeof(float));

		/* Count - We only count to 2 */
		*count = 1;
	}else if(*count == 1){
		/* Insert the first values */
		phi[0] = *past_y;
		phi[NP] = *past_u;
		phi[NP+NZ] = *past_e;
		*count = 2; /* No more - Every time when we start rls again from rest, we set k = 0 */
	}else{
		/*
		 * Shifting example:
		 * From [-y(t-1), -y(t-2), -y(t-3), -y(t-4), -y(t-5)...]
		 * To [-y(t-1), -y(t-1), -y(t-2), -y(t-3), -y(t-4)...]
		 */
		/* Shift 1 step for y */
		for(i = NP - 2; i >= 0; i--){
			phi[i + 1] = phi[i];
		}
		/* Shift 1 step for u */
		for(i = NZ - 2; i >= 0; i--){
			phi[i + NP + 1] = phi[i + NP];
		}
		/* Shift 1 step for e */
		for(i = NZE - 2; i >= 0; i--){
			phi[i + NP + NZ + 1] = phi[i + NP + NZ];
		}

		/* Insert the values at first e.g y(t) = -y(t-1) */
		phi[0] = *past_y;
		phi[0 + NP] = *past_u;
		phi[0 + NP + NZ] = *past_e;
	}
	/* Call recursive */
	recursive(NP, NZ, NZE, y, phi, theta, P, past_e, forgetting);

	/* Set the past values */
	*past_y = -y;
	*past_u = u;
}

/*
 * This function is the updater for theta, P and past_e
 */
static void recursive(size_t NP, size_t NZ, size_t NZE, float y, float phi[], float theta[], float P[], float* past_e, float forgetting) {
	/* Decleration */
	size_t i;
	
	/* Compute error = y - phi'*theta; */
	float sum = 0;
	for(i = 0; i < NP + NZ + NZE; i++)
		sum += phi[i] * theta[i];
	*past_e = y - sum;

	/* Compute: P = 1/l*(P - P*phi*phi'*P/(l + phi'*P*phi)); */

	/* Step 1: phiTP = phi'*P - > 1 row matrix */
	float *phiTP = (float*)malloc((NP + NZ + NZE)*sizeof(float));
	mul(phi, P, phiTP, 1, NP + NZ + NZE, NP + NZ + NZE); /* We pretend that phi is transpose */

	/* Step 2: Pphi = P*phi -> Vector */
	float *Pphi = (float*)malloc((NP + NZ + NZE) * sizeof(float));
	mul(P, phi, Pphi, NP + NZ + NZE, NP + NZ + NZE, 1);

	/* Step 3: l + phiTP*phi = l + phi'*P*phi */
	sum = 0;
	for (i = 0; i < NP + NZ + NZE; i++) {
		sum += phiTP[i] * phi[i];
	}
	sum += forgetting; /* Our LAMBDA */

	/* Step 4: Pphi*phiTP = P*phi*phi'*P -> Matrix */
	float *PphiphiTP = (float*)malloc((NP + NZ + NZE)*(NP + NZ + NZE)*sizeof(float));
	mul(Pphi, phiTP, PphiphiTP, NP + NZ + NZE, 1, NP + NZ + NZE);

	/* Step 5: Compute P = 1/l*(P - 1/sum*PphiphiTP); */
	for (i = 0; i < (NP + NZ + NZE) * (NP + NZ + NZE); i++) {
		P[i] = 1 / forgetting * (P[i] - 1 / sum * PphiphiTP[i]);
	}

	/* Compute theta = theta + P*phi*error; */
	mul(P, phi, Pphi, NP + NZ + NZE, NP + NZ + NZE, 1);

	/* Compute theta = theta + Pphi*error */
	for(i = 0; i < NP + NZ + NZE; i++){
		theta[i] = theta[i] + Pphi[i] * *past_e;
	}

	/* Free */
	free(phiTP);
	free(Pphi);
	free(PphiphiTP);
}

/*
 * GNU Octave code:
 * https://github.com/DanielMartensson/Mataveid/blob/master/sourcecode/rls.m
 */

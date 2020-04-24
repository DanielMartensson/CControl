/*
 * kalman.c
 *
 *  Created on: 5 oct. 2019
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Configurations.h"
#include "../../Headers/Functions.h"

/*
 * This function updates the state by this equation
 * x = Ax - KCx + Bu + Ky % Kalman filter
 */
void kalman(float* A, float* B, float* C, float* K, float* u, float* x, float* y) {

	// Note that INTEGRATION is only used for AUTOMATIC control.
	// Makes no sense to use it for MANUAL when you implement integral action by your self
	if(INTEGRATION == TRUE){
		// We have integration inside our model, model is augmented.
		// But our K matrix is built by none-integral augmented models.
		float A_[(ADIM-1)*(ADIM-1)];
		float B_[(ADIM-1)*RDIM];
		float C_[YDIM*(ADIM-1)];
		memset(A_, 0, (ADIM-1)*(ADIM-1)*sizeof(float));
		for(int i = 0; i < (ADIM-1); i++){
			*(A_ + i*(ADIM-1)) = *(A + i*ADIM); // Copy over all rows (except last) from column 0
		}
		for(int i = 0; i < (ADIM-1)-1; i++){
			*(A_ + i*(ADIM-1) + 1 + i) = 1; // Ones at the diagonal with 1 in shift to right to second last row
		}
		memcpy(B_, B, (ADIM-1)*RDIM*sizeof(float)); // Just copy the whole rows, except last row
		memset(C_, 0, YDIM*(ADIM-1)*sizeof(float));
		*(C_ + 0) = 1; // Only need to put a 1 at the column 0 and row 0

		// Note that our state vector x is actually [x; y] vector because y is integrated output
		float x_[ADIM-1]; // TODO: Här kanske vi behöver ta hänsyn till MISO
		memcpy(x_, x, (ADIM-1)*sizeof(float)); // Skip the last element y from [x;y]

		// Compute the vector A_vec = A_*x_
		float A_vec[(ADIM-1)*1];
		memset(A_vec, 0, (ADIM-1)*sizeof(float));
		mul(A_, x_, A_vec, (ADIM-1), (ADIM-1), 1);

		// Compute the vector B_vec = B_*u
		float B_vec[(ADIM-1)*1];
		memset(B_vec, 0, (ADIM-1)*sizeof(float));
		mul(B_, u, B_vec, (ADIM-1), RDIM, 1);

		// Compute the vector C_vec = C_*x_
		float C_vec[YDIM*1];
		memset(C_vec, 0, YDIM*sizeof(float));
		mul(C_, x_, C_vec, YDIM, (ADIM-1), 1);

		// Compute the vector KC_vec = K*C_vec
		float KC_vec[(ADIM-1)*1];
		memset(KC_vec, 0, (ADIM-1)*sizeof(float));
		mul(K, C_vec, KC_vec, (ADIM-1), YDIM, 1);

		// Compute the vector Ky_vec = K*y
		float Ky_vec[(ADIM-1)*1];
		memset(Ky_vec, 0, (ADIM-1)*sizeof(float));
		mul(K, y, Ky_vec, (ADIM-1), YDIM, 1);

		// Now add x = A_vec - KC_vec + B_vec + Ky_vec
		for(int i = 0; i < (ADIM-1); i++){
			*(x + i) = *(A_vec + i) - *(KC_vec + i) + *(B_vec + i) + *(Ky_vec + i);
		}

	}else{
		// Compute the vector A_vec = A*x
		float A_vec[ADIM*1];
		memset(A_vec, 0, ADIM*sizeof(float));
		mul(A, x, A_vec, ADIM, ADIM, 1);

		// Compute the vector B_vec = B*u
		float B_vec[ADIM*1];
		memset(B_vec, 0, ADIM*sizeof(float));
		mul(B, u, B_vec, ADIM, RDIM, 1);

		// Compute the vector C_vec = C*x
		float C_vec[YDIM*1];
		memset(C_vec, 0, YDIM*sizeof(float));
		mul(C, x, C_vec, YDIM, ADIM, 1);

		// Compute the vector KC_vec = K*C_vec
		float KC_vec[ADIM*1];
		memset(KC_vec, 0, ADIM*sizeof(float));
		mul(K, C_vec, KC_vec, ADIM, YDIM, 1);

		// Compute the vector Ky_vec = K*y
		float Ky_vec[ADIM*1];
		memset(Ky_vec, 0, ADIM*sizeof(float));
		mul(K, y, Ky_vec, ADIM, YDIM, 1);

		// Now add x = A_vec - KC_vec + B_vec + Ky_vec
		for(int i = 0; i < ADIM; i++){
			*(x + i) = *(A_vec + i) - *(KC_vec + i) + *(B_vec + i) + *(Ky_vec + i);
		}
	}
}

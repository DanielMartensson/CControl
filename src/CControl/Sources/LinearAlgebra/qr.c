/*
 * qr.c
 *
 *  Created on: 1 mars 2020
 *      Author: hp
 */

#include "../../Headers/Functions.h"

#include "../../Headers/Functions.h"

/*
 * Householder QR-decomposition
 * A [m*n]
 * Q [m*m]
 * R [m*n]
 *
 * Returns 1 == Success
 * Returns 0 == Fail
 */
uint8_t qr(float* A, float* Q, float* R, uint16_t row_a, uint16_t column_a, bool only_compute_R){

	// Declare
	uint16_t row_a_row_a = row_a*row_a;
	uint16_t l = row_a - 1 < column_a ? row_a - 1 : column_a;
	float s, Rk, r, W[row_a], WW[row_a_row_a], Hi[row_a_row_a], H[row_a_row_a], HiH[row_a_row_a], HiR[row_a*column_a];

	// Give A to R
	memcpy(R, A, row_a*column_a*sizeof(float));

	// Turn H into identity matrix
	memset(H, 0, row_a_row_a*sizeof(float));
	for(uint16_t i = 0; i < row_a; i++)
		H[row_a*i + i] = 1.0f;

	// Do house holder transformations
	for(uint16_t k = 0; k < l; k++){
		// Do L2 norm
		s = 0;
		for(uint16_t i = k; i < row_a; i++)
			s += R[i*column_a + k] * R[i*column_a + k];
		s = sqrtf(s);

		// Find Rk
		Rk = R[k*column_a + k];

		// Do sign
		if(Rk < 0.0f)
			s = -s;

		// Compute r
		r = sqrtf(2*s*(Rk + s));

		// Fill W
		memset(W, 0, row_a*sizeof(float));
		W[k] = (Rk+s)/r;
		for(uint16_t i = k+1; i < row_a; i++)
			W[i] = R[i*column_a + k] / r;

		// WW = W*W'
		mul(W, W, WW, row_a, 1, row_a);

		// Fill Hi matrix
		for(uint16_t i = 0; i < row_a_row_a; i++)
			Hi[i] = -2.0f*WW[i];

		// Use identity matrix on Hi
		for(uint16_t i = 0; i < row_a; i++)
			Hi[i*row_a + i] += 1.0f;

		// HiH = Hi * H -> HiH = H
		if(!only_compute_R) {
			mul(Hi, H, HiH, row_a, row_a, row_a);
			memcpy(H, HiH, row_a_row_a*sizeof(float));
		}

		// HiR = Hi * R -> HiR = R
		mul(Hi, R, HiR, row_a, row_a, column_a);
		memcpy(R, HiR, row_a*column_a*sizeof(float));
	}

	// Do inverse on H and give it to Q
	uint8_t status = 1;
	if(!only_compute_R) {
		status = inv(H, row_a);
		memcpy(Q, H, row_a_row_a*sizeof(float));
	}
	return status;
}

/*
 * GNU Octave code:
 *  function [Q,R] = qr(A)
	  % Perform QR decompostion by using Householder transformation
	  % Input  - A is an m x n matrix
	  % Output - Q, R : QR decomposition results
	  [m,n] = size(A);
	  R = A;
	  H = eye(m);
	  for k = 1:min(m-1,n)
		  s = norm(R(k:m,k));
		  Rk = R(k,k);
		  if (Rk < 0)
			  s=-s;
		  end
		  r = sqrt(2*s*(Rk+s));
		  W = zeros(m,1);
		  W(k) = (Rk+s)/r;
		  W(k+1:m) = R(k+1:m,k)/r;
		  Hi=eye(m)-2*W*W';
		  HiH=Hi*H;
		  H = HiH;
		  HiR = Hi*R;
		  R = HiR;
	  end
	  Q = inv(H);
	end
 */

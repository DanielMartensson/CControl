/*
 * qr.c
 *
 *  Created on: 1 mars 2020
 *      Author: hp
 */

#include "../../Headers/Functions.h"

/*
 * Householder QR-decomposition.
 * Example how to solve Ax=b with QR
 * 1: Ax = b
 * 2: A = QR
 * 3: QRx = b
 * 4: Rx = Q^Tb <- Solve this by using linsolve_upper_triangular.c
 *
 * A [m*n]
 * Q [m*m]
 * R [m*n]
 */
void qr(float* A, float* Q, float* R, uint16_t row_a, uint16_t column_a){
	// Turn Q into identity matrix
	memset(Q, 0, row_a*row_a*sizeof(float));
	for(uint16_t i = 0; i < row_a; i++)
		Q[row_a*i + i] = 1;

	// Give A to R
	memcpy(R, A, row_a*column_a*sizeof(float));

	// Parameters
	float normx;
	float s;
	float u1;
	float tau;
	float temp[column_a]; // This is for holding values for Q and R

	// This make sure that we can use A transpose as well
	uint16_t loops = row_a <= column_a ? row_a : column_a;

	// H = I-tau*w*w’ to put zeros below R(j,j)
	for(uint16_t j = 0; j < loops; j++){
		// Do the euclidean norm
		normx = 0;
		for(uint16_t i = j; i < row_a; i++)
			normx += R[column_a*i + j] * R[column_a*i + j];
		normx = sqrtf(normx);

		// Do reversed sign!
		s = -sign(R[column_a*j + j]);

		// Now create Q and R
		u1 = R[column_a*j + j] - s*normx;
		float w[row_a - j];
		for(uint16_t i = 0; i < row_a-j; i++)
			w[i] = R[column_a*(i+j) + j]/u1;
		w[0] = 1;
		tau = -s*u1/normx;

		// R = HR, Q := QH

		// Do temp = w'*B
		// B = R(j:end, :)
		float B[(row_a-j)*column_a];
		cut(R, row_a, column_a, B, j, row_a-1, 0, column_a-1);
		mul(w, B, temp, 1, row_a - j, column_a);

		// Do B = w*temp
		mul(w, temp, B, row_a - j, 1, column_a);

		// Do C = w*w'
		float C[(row_a - j)*(row_a - j)];
		mul(w, w, C, row_a - j, 1, row_a - j);

		// Create D = Q(:,j:end)
		float D[row_a*(row_a-j)];
		cut(Q, row_a, row_a, D, 0, row_a-1, j, row_a-1);

		// Do E = D*C
		float E[row_a*(row_a - j)];
		mul(D, C, E, row_a, row_a-j, row_a-j);

		// Find now Q and R
		for(uint16_t i = 0; i < row_a-j; i++)
			for(uint16_t a = 0; a < column_a; a++)
				R[column_a*(i+j) + a] += -tau*B[column_a*i + a];

		for(uint16_t i = 0; i < row_a; i++)
			for(uint16_t a = 0; a < row_a-j; a++)
				Q[row_a*i + (a+j)] += -tau*E[(row_a-j)*i + a];
	}
}

/*
 * GNU Octave code:
 *  function [Q,R] = qr(A)
	  % Compute the QR decomposition of an m-by-n matrix A using Householder transformations.
	  [m,n] = size(A);

	  Q = eye(m);      % Orthogonal transform so farR = A;
	  R = A;           % Transformed matrix so far

	  % This make sure that we can use A transpose as well
	  if(m <= n)
		n = m;
	  end

	  for j = 1:n % -- Find H = I-tau*w*w’ to put zeros below R(j,j)
		normx = norm(R(j:end,j));
		s     = -sign(R(j,j));
		u1    = R(j,j) - s*normx;
		w     = R(j:end,j)/u1;
		w(1)  = 1;
		tau   = -s*u1/normx;
		% Find Q and R now
		R(j:end,:) = R(j:end,:)-(tau*w)*(w'*R(j:end,:));
		Q(:,j:end) = Q(:,j:end)-(Q(:,j:end)*w)*(tau*w)';
	  end
	end
 */

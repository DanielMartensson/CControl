/*
 * rk4.c
 *
 *  Created on: 24:e Januari 2025
 *      Author: Daniel Mårtensson
 */

#include "differentialequation.h"

/*
 * Fourth-order Runge–Kutta method.
 * Runge-Kutta method is not the best ODE solver, but it's the best ODE solver for fixed step time iteration. 
 * h - Step time
 * Y[iterations*N] - Output
 * y[N] - Initial state vector 
 * N - Dimension for y-vector
 * odefun(const float t, float y[])
 */
void rk4(const bool no_ode_output, const size_t iterations, const float h, float Y[], float y[], const size_t N, void (*odefun)(float, float*)) {
	/* Variables */
	size_t i, j;

	/* Constants */
	const size_t length = N * sizeof(float);

	/* Create vectors */
	float* k1 = (float*)malloc(length);
	float* k2 = (float*)malloc(length);
	float* k3 = (float*)malloc(length);
	float* k4 = (float*)malloc(length);

	/* Temporary vectors */
	float* yt = (float*)malloc(length);

	/* Variables */
	float t = 0.0f;

	/* Initial output */
	memcpy(Y, y, length);

	/* Perform Runge-Kutta */
	for (i = 1; i < iterations; i++) {
		/* Receive old output */
		for (j = 0; j < N; j++) {
			y[j] = Y[(i-1) * N * (!no_ode_output) + j];
		}

		/* First statement */
		memcpy(yt, y, length);
		odefun(t, yt);
		for (j = 0; j < N; j++) {
			k1[j] = yt[j] * h;
		}

		/* Second statement */
		memcpy(yt, y, length);
		for (j = 0; j < N; j++) {
			yt[j] += k1[j] / 2.0f;
		}
		odefun(t + h / 2.0f, yt);
		for (j = 0; j < N; j++) {
			k2[j] = yt[j] * h;
		}

		/* Third statement */
		memcpy(yt, y, length);
		for (j = 0; j < N; j++) {
			yt[j] += k2[j] / 2.0f;
		}
		odefun(t + h / 2.0f, yt);
		for (j = 0; j < N; j++) {
			k3[j] = yt[j] * h;
		}

		/* Fourth statement */
		memcpy(yt, y, length);
		for (j = 0; j < N; j++) {
			yt[j] += k3[j];
		}
		odefun(t + h, yt);
		for (j = 0; j < N; j++) {
			k4[j] = yt[j] * h;
		}	

		/* Save output */
		for (j = 0; j < N; j++) {
			Y[i * N * (!no_ode_output) + j] = y[j] + (k1[j] + 2.0f * k2[j] + 2.0f * k3[j] + k4[j]) / 6.0f;
		}

		/* Update t */
		t += h;
	}

	/* Free */
	free(k1);
	free(k2);
	free(k3);
	free(k4);
	free(yt);
}

/*
GNU Octave code:

#Applying the Runge-Kutta method (This code must be implement on a different file than the main one).

function temp = rk4(func,x,pvi,h)
	K1 = h*func(x,pvi);
	K2 = h*func(x+0.5*h,pvi+0.5*K1);
	K3 = h*func(x+0.5*h,pvi+0.5*K2);
	K4 = h*func(x+h,pvi+K3);
	temp = pvi + (K1 + 2*K2 + 2*K3 + K4)/6;
endfunction

#Main Program.

f  = @(t) (1/16)*((t.^2 + 4).^2);
df = @(t,y) t*sqrt(y);

pvi = 1.0;
h   = 0.1;
Yn  = pvi;

for x = 0:h:10-h
	pvi = rk4(df,x,pvi,h);
	Yn = [Yn pvi];
endfor

fprintf('Time \t Exact Value \t ODE4 Value \t Num. Error\n');

for i=0:10
	fprintf('%d \t %.5f \t %.5f \t %.4g \n',i,f(i),Yn(1+i*10),f(i)-Yn(1+i*10));
endfor

*/
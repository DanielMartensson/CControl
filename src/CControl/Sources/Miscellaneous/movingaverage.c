/*
 * movingaverage.c
 *
 *  Created on: 14 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Configurations.h"
#include "../../Headers/Functions.h"

/*
 * This filter a array. Very simple method, but powerful
 * y [l]
 * p > 0 // Tuning factor
 * Returns y as filtered
 */
void movingaverage(float* y, uint16_t l, uint8_t p){
	// Just to make sure we wont divide with 0
	if(p <= 0) return;

	// Do first filtering by jumping k/2 elements in the future
	float sum = 0;
	uint16_t k = 0;
	for(uint16_t i = 0; i < l; i++){
		sum = 0;
		k = 0;
		for(int j = 0; j < p; j++){
			if(i + j < l){
				sum += *(y + i + j);
				k++;
			}
		}
		if(k != 0){
			*(y + i + k/2) = sum/k;
		}
	}

	// Do the first p elements!
	for(uint8_t i = p; i > p; i--){
		sum = 0;
		for(int j = 0; j < i; j++){
			sum += *(y + j);
		}
		if(i != 0)
			*(y + i) = sum/i;
	}
}

/*
 * GNU Octave code:
 *  % Use Matavecontrol library from GitHub
	sys = ss(0, [0 1; -2 -3], [0; 1], [1 0]);
	[u,t] = gensig('square', 10, 40, 100);
	y = lsim(sys, u, t);
	l = length(t);
	ye = y + 0.1*randn(1, l);
	yp = ye; % Save this
	% Forward filtering
	p = 5;
	% Do first filtering by jumping k/2 elements in the future
	for i = 1:l
	  sum = 0;
	  k = 0;
	  for j = 0:p-1
		if(i + j <= l)
		  sum = sum + ye(i + j);
		  k = k + 1;
		end
	  end
	  if(k != 0)
		ye(i+floor(k/2)) = sum/k;
	  end
	end

	% Do the first p elements!
	for i = p:-1:1
	  sum = 0;
	  for j = 1:i
		sum = sum + ye(j);
	  end
	  ye(i) = sum/i;
	end

	% Done!
	plot(t, ye, t, yp);
	legend("Filtered", "Noisy")
 */

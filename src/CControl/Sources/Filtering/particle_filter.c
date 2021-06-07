/*
 * particle_filter.c
 *
 *  Created on: 7 juni 2021
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Particle filter collect
 * P[row_x*2*column_p]
 * x[row_x]
 * index_vactor > 0
 */
void particle_filter_collect(float* P, uint16_t column_p, float* x, uint8_t row_x, float index_factor){
	// Increase index
	for(uint8_t i = 0; i < row_x; i++){
		// Compute index
		uint16_t index = fabsf(*(x + i)) * index_factor; // E.g 45 = 78.23 * 0.57

		// Save a count and store the latest value in every second row
		if(*(x + i) >= 0){
			*(P + (i+i)*column_p + index) += 1;
			*(P + (i+i+1)*column_p + index) = *(x + i); // Start [, , , , 23, , , , .... , , , , ] end
		}else{
			*(P + (i+i)*column_p + column_p - index) += 1;
			*(P + (i+i+1)*column_p + column_p - index) = *(x + i); // Start [, , , , , , , , .... , , , -23 , ] end
		}
	}
}

/*
 * Particle filter estimate
 * P[row_x*2*column_p]
 * x[row_x]
 */
void particle_filter_estimate(float* P, uint16_t column_p, float* x, uint8_t row_x){
	// Find the largest index
	uint16_t max_index;
	uint16_t max_value;
	for(uint8_t i = 0; i < row_x; i++){
		// Find the largest index
		max_index = 0;
		max_value = 0;
		for(uint16_t j = 0; j < column_p; j++){
			uint16_t value = *(P + (i+i)*column_p + j);
			if(value > max_value){
				max_index = j;
				max_value = value;
			}
		}

		// Save the value at the specific index
		*(x + i) = *(P + (i+i+1)*column_p + max_index);
	}
}

void particle_filter_clean(float* P, uint16_t column_p, uint8_t row_x){
	memset(P, 0, column_p*row_x*2*sizeof(float));
}

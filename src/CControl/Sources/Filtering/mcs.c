/*
 * mcs.c
 *
 *  Created on: 7 juni 2021
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Monte Carlo Simulation collect
 * P[row_x*2*column_p]
 * x[row_x]
 * index_vactor > 0
 */
void mcs_collect(float P[], uint16_t column_p, float x[], uint8_t row_x, float index_factor){
	// Increase index
	uint16_t index;
	for(uint8_t i = 0; i < row_x; i++){
		// Compute index
		index = fabsf(x[i]) * index_factor; // E.g 45 = 78.23 * 0.57
		if(index >= column_p){
			printf("Decrease index_factor!\n");
			continue;
		}

		// Save a count and store the latest value in every second row
		if(x[i] >= 0){
			P[(i+i)*column_p + index] += 1;
			P[(i+i+1)*column_p + index] = x[i]; 				// Start [, , , , 23, , , , .... , , , , ] end
		}else{
			P[(i+i)*column_p + column_p - 1 - index] += 1;
			P[(i+i+1)*column_p + column_p - 1 - index] = x[i]; 	// Start [, , , , , , , , .... , , , -23 , ] end
		}
	}
}

/*
 * Monte Carlo Simulation estimate
 * P[row_x*2*column_p]
 * x[row_x]
 */
void mcs_estimate(float P[], uint16_t column_p, float x[], uint8_t row_x){
	// Find the largest index
	uint16_t index_of_value;
	uint16_t max_value_index;
	uint16_t column_value;
	uint16_t column_index;
	for(uint8_t i = 0; i < row_x; i++){
		// Find the largest index
		index_of_value = 0;
		max_value_index = 0;
		column_index = (i+i)*column_p;
		column_value = (i+i+1)*column_p;
		for(uint16_t j = 0; j < column_p; j++){
			uint16_t value = P[column_index + j];
			if(value > max_value_index){
				index_of_value = j;
				max_value_index = value;
			}
		}

		// Save the value at the specific index
		x[i] = P[column_value + index_of_value];
	}
}

void mcs_clean(float P[], uint16_t column_p, uint8_t row_x){
	memset(P, 0, column_p*row_x*2*sizeof(float));
}

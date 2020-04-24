/*
 * lbph.c
 *
 *  Created on: 8 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Configurations.h"
#include "../../Headers/Functions.h"

static uint16_t computeError(uint16_t i, uint8_t* histograms, uint8_t* histogram);

/*
 * This is Local Binary Pattern Histogram(LBPH) method.
 * This can identify a picture from a histogram training set.
 * The drawback with this algorithm is that cannot handle noise e.g complex pictures.
 * The positive with this algorithm is that it's super simple and fast and it's robust at different light scenarios.
 * To handle complex pictures, split them up into several sub-pictures.
 * It requires more training histograms.
 *
 * Dimensions:
 * picture [row_p*column_p]
 * histograms [row_h*256]
 *
 * Maximum size of row_h: 2^16 -2
 *
 * Return:
 * 0 == Fail (row_p and column_p need to be divided with 3)
 * 1 == Saved OK.
 * >= 2 == The index of array histograms where you need to take -2 to get the correct row
 *
 * Training:
 * Set variable save == true and enter a row number for row_h.
 * Then the last row of histograms will have the latest histogram.
 *
 * Identify:
 * Set variable save == false and you will get the row + 2 of the selected identification
 * from array histograms.
 */
uint16_t lbph(uint8_t* picture, uint16_t row_p, uint16_t column_p, uint8_t* histograms, uint16_t row_h, bool save){
	// Check if row_p and column_p can be divided in 3
	if(row_p % 3 != 0 || column_p % 3 != 0)
		return 0; // Error

	// Create the histogram
	uint8_t histogram[256];
	memset(histogram, 0, 256*sizeof(uint8_t));
	//We need to have -2 smaller on row and column due to the cube method
	for(uint16_t i = 0; i < row_p-2; i++){
		for(uint16_t j = 0; j < column_p-2; j++){
			/*
			 * Do cube
			 *  b0	b1	b2
			 * 	b7	B	b3
			 * 	b6	b5	b4
			 *
			 * 	if B >= b_i -> 1, else 0
			 */
			uint8_t B = *(picture + column_p*(i+1) + j+1);
			uint8_t b0 = *(picture + column_p*i + j) >= B ? 1 : 0; // MBS
			uint8_t b1 = *(picture + column_p*i + j+1) >= B ? 1 : 0;
			uint8_t b2 = *(picture + column_p*i + j+2) >= B ? 1 : 0;
			uint8_t b3 = *(picture + column_p*(i+1) + j+2) >= B ? 1 : 0;
			uint8_t b4 = *(picture + column_p*(i+2) + j+2) >= B ? 1 : 0;
			uint8_t b5 = *(picture + column_p*(i+2) + j+1) >= B ? 1 : 0;
			uint8_t b6 = *(picture + column_p*(i+2) + j) >= B ? 1 : 0;
			uint8_t b7 = *(picture + column_p*(i+1) + j) >= B ? 1 : 0; // LSB
			B = (b0 << 7) | (b1 << 6) | (b2 << 5) | (b3 << 4) | (b4 << 3) | (b5 << 2) | (b6 << 1) | (b7 << 0);
			if(B != 0 && B != 255) // This is used because u1 and u2(down in the computeError function) should not be the same all the time
				histogram[B] += 1; // This B number can be between 1 and 254
		}
	}

	// Check if you want to train the histogram? If not, then we start to identify the picture
	if(save){
		memcpy(histograms + 256*(row_h-1), histogram, 256*sizeof(uint8_t)); // Save histogram to the last row of histograms
		return 1;
	}else{
		// Compare which histogram is most correct
		uint16_t oldValue;
		uint16_t index = 0;
		uint16_t newValue;
		for(uint16_t i = 0; i < row_h; i++){
			if(i == 0){
				oldValue = computeError(i, histograms, histogram); // First time we set a value to variable value
			}else{
				newValue = computeError(i, histograms, histogram); // Now compare with the old value
				if(oldValue > newValue){
					oldValue = newValue;
					index = i;
				}
			}
		}
		return index + 2; // Because 0 is dimension fail, 1 is saving and the rest is the index of histograms - 2
	}
}

// Compute the error
static uint16_t computeError(uint16_t i, uint8_t* histograms, uint8_t* histogram){
	// Create means, without dividing with 256
	uint16_t u1 = 0;
	uint16_t u2 = 0;
	for(uint16_t j = 0; j < 256; j++){
		u1 += *(histogram + j);
		u2 += *(histograms + 256*i + j);
	}

	// Create standard deviations, without dividing with 255 (not 256!)
	uint16_t s1 = 0;
	uint16_t s2 = 0;
	for(uint16_t j = 0; j < 256; j++){
		s1 += abs((*(histogram + j) - u1));
		s2 += abs(*(histograms + 256*i + j) - u2);
	}

	// Compute the total error
	uint16_t sum = 0;
	for(uint16_t j = 0; j < 256; j++){
		sum += abs(*(histograms + 256*i + j) - *(histogram + j));
	}

	// Print summary
	printf("sum = %d, u1 = %d, u2 = %d, s1 = %d, s2 = %d\n", sum, u1, u2, s1, s2);

	return sum + abs(u1-u2) + abs(s1-s2);
}

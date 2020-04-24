/*
 * integral.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Configurations.h"
#include "../../Headers/Functions.h"

/*
 * This computes the integral by sum state vector xi with reference - measurement
 * xi = xi + r - y;
 */
void integral(float* xi, float* r, float* y) {
	for(int i = 0; i < RDIM; i++){
		/*
		 * Anti-windup
		 */
		if(ANTI_WINDUP == 0){
			*(xi + i) = *(xi + i) + *(r + i) - *(y + i); // Always integrate
		}else if(ANTI_WINDUP == 1){
			if(*(r + i) > *(y + i)){
				*(xi + i) = *(xi + i) + *(r + i) - *(y + i); // Only integrate when r > y, else delete
			}else{
				*(xi + i) = 0; // Delete just that xi
			}
		}else if(ANTI_WINDUP == 2){
			if(*(r + i) > *(y + i)){
				*(xi + i) = *(xi + i) + *(r + i) - *(y + i); // Only integrate r > y, else stop
			}
		}else{
			*(xi + i) = *(xi + i) + *(r + i) - *(y + i); // Always integrate if nothing else selected
		}
	}
}



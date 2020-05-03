/*
 ============================================================================
 Name        : Model_Reference_Adaptice_Control.c
 Author      : Daniel Mårtensson
 Version     : 1.0
 Copyright   : MIT
 Description : Model Reference Adaptive Control
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "CControl/Headers/Configurations.h"
#include "CControl/Headers/Functions.h"

#define RDIM 1
#define ANTI_WINDUP 0
#define LEARNING 0.001

int main() {

	/* MRAC works as K1 and K2 integrates over time so y -> r when time -> inf
	 * r ----{K1}*(-LEARNING)---->+()----->G(s)-----------------------> y
	 *               			  -^            			  |
	 *               			   |            			  |
	 *                			   |              			  |
	 *                  		   |---*(LEARNING){K2}<--------
	 */

	float y[RDIM] = {7};
	float u[RDIM] = {0};
	float r[RDIM] = {51};
	float K1[RDIM] = {3.1};
	float K2[RDIM] = {4.1};

	// Compute now
	mrac(ANTI_WINDUP, LEARNING, y, u, r, K1, K2, RDIM);

	// Show input signal
	printf("Our output signal\n");
	print(u, RDIM, 1);

	return EXIT_SUCCESS;
}

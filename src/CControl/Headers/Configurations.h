/*
 * Configurations.h
 *
 *  Created on: 5 oct. 2019
 *      Author: Daniel Mårtensson
 */

/*
 * Instead of using stdbool.h
 */
#define TRUE 1
#define FALSE 0

/*
 * Control strategies
 */
#define AUTOMATIC 0  // For Autotuning Predictive Control, Generalized Predictive Control
#define MANUAL 1	 // For Linear Quadratic Integral Control, Model Predictive Control

/*
 * Integration strategies
 */
#define ALWAYS_INTEGRATE 0 // Always integrate
#define UPPER_INTEGRATE_LOWER_DELETE 1 // Delete xi when r < y, else integrate when r > y
#define UPPER_INTEGRATE_LOWER_STOP 2 // Only integrate xi when r < y, but stop integrate when r > y

/*
 * Algorithms iterations
 */
#define MAX_ITERATION_COUNT_SVD 30   // Maximum number of iterations

#ifndef CCONTROL_CONFIGURATIONS_H_
#define CCONTROL_CONFIGURATIONS_H_


/*
 * Step 1: Select your SELECTED_CONTROL_STRATEGY first
 */
#define SELECTED_CONTROL_STRATEGY AUTOMATIC

/*
 * Step 2: Configure the parameters that you need. Read the PDF manual.
 */
#if SELECTED_CONTROL_STRATEGY == AUTOMATIC

	/*
	 * Prediction & Integration
	 */
	//#define HORIZON 50 // How long we want to look in the future
	//#define ALPHA 0.1 // This will prevent dead-beat control and gives more smooth input values
	//#define INTEGRATION TRUE // Enable integral action inside model - Recommended
	//#define ITERATION_LIMIT 100 // This is for MPC
	//#define ANTI_WINDUP ALWAYS_INTEGRATE // The integral function for MRAC
    //#define LEARNING 0.0001 // For MRAC

	/*
	 * This is configuration for the state space model.
	 * Let the configurations above configure this below - Do not touch
	 */
	//#define NP POLY_LENGTH // Number of poles for output polynomial A(q)
	//#define NZ POLY_LENGTH // Number of zeros for input polynomial B(q)
	//#define NZE POLY_LENGTH // Number of zeros for error polynomial C(q)
	#if INTEGRATION
		//#define ADIM (POLY_LENGTH+1) // Matrix A will be [A 0; CA 1]
		//#define RDIM 1 // Matrix B will be [B; CB]
		//#define YDIM 1 // Matrix C will be [0 1]
	#else
		//#define ADIM POLY_LENGTH // Matrix A will be [A]
		//#define RDIM 1 // Matrix B will be [B]
		//#define YDIM 1 // Matrix C will be [C]
	#endif

#elif SELECTED_CONTROL_STRATEGY == MANUAL
	#define ADIM 2 // Matrix A will be [A]
	#define YDIM 1 // Matrix B will be [C]
	#define RDIM 1 // Matrix C will be [B]
	#define HORIZON 200
	#define ALPHA 0.1 // Prevent deadbeat control for MPC and GPC
    #define ITERATION_LIMIT 200 // This is for MPC
    #define ANTI_WINDUP ALWAYS_INTEGRATE // The integral function for LQR

	/*
	 * We need to at least set some values of these, even if MANUAL don't use identification, integration and prediction
	 */
    #define POLY_LENGTH ADIM // No need this
	#define INTEGRATION FALSE // No need this. Note that INTEGRATION is only used for AUTOMATIC control. Makes no sense to use it for MANUAL when you implement integral action by your self
	#define NP 1 // No need this
	#define NZ 1 // No need this
	#define NZE 1 // No need this
	#define q 1 // No need this
    #define LAMBDA 1 // No need this
	#define LEARNING 0 // No need this
#endif




#endif /* CCONTROL_CONFIGURATIONS_H_ */



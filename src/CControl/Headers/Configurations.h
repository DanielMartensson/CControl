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
#define SHOW_QP_OUTPUT TRUE // Set this to TRUE if you want mpc-result to appear. Good to know how many nWSR you should have

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
	 * Polynomial transfer function estimation
	 */
	#define q 1000 // Initial diagonal values for P matrix for system identification. 1000 is a good number
	#define LAMBDA 1 // Forgetting factor for system identification. Lambda Should not be less than zero
	#define POLY_LENGTH 6 // Length of polynomials A(q), B(q) and C(q) in Recursive least squares

	/*
	 * Prediction & Integration
	 */
	#define HORIZON 100 // How long we want to look in the future
	#define ALPHA 1.0 // This will prevent dead-beat control and gives more smooth input values
	#define INTEGRATION TRUE // Enable integral action inside model - Recommended

	/*
	 * This is configuration for the state space model.
	 * Let the configurations above configure this below
	 */
	#define NP POLY_LENGTH // Number of poles for output polynomial A(q)
	#define NZ POLY_LENGTH // Number of zeros for input polynomial B(q)
	#define NZE POLY_LENGTH // Number of zeros for error polynomial C(q)
	#if INTEGRATION
		#define ADIM (POLY_LENGTH+1) // Matrix A will be [A 0; CA 1]
		#define RDIM 1 // Matrix B will be [B; CB]
		#define YDIM 1 // Matrix C will be [0 1]
	#else
		#define ADIM POLY_LENGTH // Matrix A will be [A]
		#define RDIM 1 // Matrix B will be [B]
		#define YDIM 1 // Matrix C will be [C]
	#endif

#elif SELECTED_CONTROL_STRATEGY == MANUAL
	#define ADIM 10 // Matrix A will be [A]
	#define YDIM 1 // Matrix B will be [C]
	#define RDIM 1 // Matrix C will be [B]
	#define HORIZON 10

	/*
	 * We need to at least set some values of these, even if MANUAL don't use identification, integration and prediction
	 */
    #define POLY_LENGTH ADIM // Not need this
	#define INTEGRATION FALSE // Not need this. Note that INTEGRATION is only used for AUTOMATIC control. Makes no sense to use it for MANUAL when you implement integral action by your self
	#define NP 1 // Not need this
	#define NZ 1 // Not need this
	#define NZE 1 // Not need this
	#define q 1 // Not need this
    #define LAMBDA 1 // Not need this
	#define ALPHA 0 // Not need this
#endif









#endif /* CCONTROL_CONFIGURATIONS_H_ */



/*
 ============================================================================
 Name        : Adaptive_Model_Predictive_Control.c
 Author      : Daniel Mårtensson
 Version     : 1.0
 Copyright   : MIT
 Description : Adaptive Model Predictive Control
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "CControl/Headers/Configurations.h"
#include "CControl/Headers/Functions.h"

int main() {

	/*
	 * In this example we have selected the following configuration inside Confiugration.h
	 * SELECTED_CONTROL_STRATEGY AUTOMATIC
	 * q 1000
	 * LAMBDA 1
	 * POLY_LENGTH 6
	 * HORIZON 100
	 * ALPHA 1.0
	 * INTEGRATION TRUE
	 * SHOW_QP_OUTPUT FALSE - Set this to TRUE to begin with, then red text appear. Change nWSR so QP can compute its sets
	 */
	clock_t start = clock();

	float input[200] = { 0, 0.301, 0.59896, 0.89088, 1.1738, 1.4449, 1.7014,
			1.9407, 2.1605, 2.3584, 2.5325, 2.6811, 2.8026, 2.8959, 2.9599,
			2.994, 2.9979, 2.9716, 2.9153, 2.8295, 2.7152, 2.5735, 2.4058,
			2.2139, 1.9996, 1.7651, 1.5128, 1.2452, 0.96509, 0.67522, 0.37854,
			0.078035, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.14536, 0.44528, 0.74069,
			1.0286, 1.3062, 1.5706, 1.8191, 2.0493, 2.2588, 2.4455, 2.6075,
			2.7432, 2.8512, 2.9304, 2.9801, 2.9997, 2.989, 2.9481, 2.8775,
			2.7779, 2.6502, 2.4958, 2.3162, 2.1132, 1.8888, 1.6455, 1.3855,
			1.1115, 0.82633, 0.5328, 0.2339, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.29038,
			0.5885, 0.88069, 1.164, 1.4355, 1.6926, 1.9326, 2.153, 2.3518,
			2.5268, 2.6763, 2.7988, 2.8931, 2.9581, 2.9933, 2.9983, 2.973,
			2.9178, 2.833, 2.7197, 2.579, 2.4122, 2.2211, 2.0075, 1.7737, 1.522,
			1.2549, 0.97519, 0.68562, 0.38912, 0.088701, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0.1347, 0.43472, 0.73035, 1.0186, 1.2966, 1.5615, 1.8106, 2.0415,
			2.2517, 2.4393, 2.6022, 2.7388 }; // Input signal

	float output[200] = { 0.00000, 0.00000, 0.00290, 0.01369, 0.03614, 0.07300,
			0.12601, 0.19605, 0.28321, 0.38692, 0.50604, 0.63892, 0.78353,
			0.93752, 1.09829, 1.26309, 1.42905, 1.59329, 1.75293, 1.90515,
			2.04728, 2.17677, 2.29128, 2.38868, 2.46710, 2.52494, 2.56087,
			2.57386, 2.56320, 2.52846, 2.46955, 2.38668, 2.28033, 2.15346,
			2.01158, 1.86012, 1.70366, 1.54599, 1.39022, 1.23880, 1.09368,
			0.95628, 0.82764, 0.70843, 0.59902, 0.49953, 0.40990, 0.32986,
			0.25906, 0.19702, 0.14320, 0.09702, 0.05786, 0.02510, -0.00189,
			-0.02370, -0.04093, -0.05414, -0.06386, -0.07056, -0.07470,
			-0.07670, -0.07693, -0.07571, -0.07194, -0.06198, -0.04165,
			-0.00784, 0.04152, 0.10763, 0.19087, 0.29093, 0.40686, 0.53724,
			0.68018, 0.83345, 0.99457, 1.16086, 1.32948, 1.49758, 1.66226,
			1.82070, 1.97014, 2.10800, 2.23184, 2.33943, 2.42880, 2.49821,
			2.54621, 2.57164, 2.57363, 2.55162, 2.50538, 2.43494, 2.34069,
			2.22392, 2.08893, 1.94164, 1.78708, 1.62943, 1.47216, 1.31804,
			1.16928, 1.02757, 0.89414, 0.76984, 0.65519, 0.55045, 0.45563,
			0.37056, 0.29494, 0.22835, 0.17028, 0.12016, 0.07739, 0.04136,
			0.01142, -0.01302, -0.03258, -0.04782, -0.05930, -0.06751, -0.07293,
			-0.07600, -0.07710, -0.07658, -0.07478, -0.06916, -0.05506,
			-0.02886, 0.01203, 0.06923, 0.14352, 0.23492, 0.34280, 0.46597,
			0.60274, 0.75106, 0.90856, 1.07265, 1.24055, 1.40942, 1.57636,
			1.73851, 1.89307, 2.03737, 2.16886, 2.28524, 2.38437, 2.46440,
			2.52373, 2.56104, 2.57532, 2.56584, 2.53221, 2.47431, 2.39236,
			2.28686, 2.16066, 2.01918, 1.86791, 1.71146, 1.55366, 1.39764,
			1.24590, 1.10037, 0.96253, 0.83341, 0.71371, 0.60381, 0.50384,
			0.41373, 0.33325, 0.26202, 0.19957, 0.14539, 0.09886, 0.05940,
			0.02636, -0.00087, -0.02291, -0.04033, -0.05371, -0.06356, -0.07039,
			-0.07463, -0.07671, -0.07700, -0.07584, -0.07222, -0.06257,
			-0.04266, -0.00938, 0.03941, 0.10491, 0.18755, 0.28702, 0.40242,
			0.53232, 0.67485 };

	/*
	 * Estimation SISO model - Assume that this this is inside the microprocessor's while(1) loop
	 */
	float past_e; // The past e
	float past_y; // The past y
	float past_u; // The past u
	float phi[NP + NZ + NZE];
	float P[(NP + NZ + NZE)*(NP + NZ + NZE)];
	float theta[NP + NZ + NZE]; // Remember that total length is POLY_LENGTH*3 - ALWAYS have these dimensions
	int count = 0; // Initial set to 0. Will be counted to 2 then stop. Set count = 0 again and the algorithm starts over
	for (int i = 0; i < sizeof(output) / sizeof(output[0]); i++) {
		rls(theta, input[i], output[i], &count, &past_e, &past_y, &past_u, phi, P);
	}

	/*
	 * Find our state space model with kalman gain matrix from theta - ALWAYS have these dimensions
	 */
	float A[ADIM * ADIM];
	float B[ADIM * RDIM];
	float C[YDIM * ADIM];
	float K[(ADIM - 1) * YDIM]; // (ADIM-1) only if we have INTEGRATION TRUE, else ADIM
	theta2ss(A, B, C, theta, K);

	/*
	 * Print our state space matrix

	printf("System matrix: A\n");
	print(A, ADIM, ADIM);

	printf("Input matrix: B\n");
	print(B, ADIM, RDIM);

	printf("Output matrix: C\n");
	print(C, YDIM, ADIM);

	printf("Kalman gain matrix: K\n");
	print(K, (ADIM - 1), YDIM);
	*/

	float x[ADIM] = { 0, 0, 0, 0, 0, 0, output[sizeof(output) / sizeof(output[0]) - 1]};

	float u[RDIM] = { 0 };
	float r[YDIM] = { 12.5 };

	float ulb[RDIM] = { 0 };
	float uub[RDIM] = { 20 };

	float ylb[YDIM] = { 0 };
	float yub[YDIM] = { 20 };

	int nWSR = 50;

	mpc(A, B, C, x, u, r, ulb, uub, ylb, yub, &nWSR);

	printf("Best input\n");
	print(u, RDIM, 1);

	float y[YDIM] = { x[ADIM - 1] };

	// Update our state vector x
	printf("Our current state is:\n");
	print(x, ADIM, 1);
	kalman(A, B, C, K, u, x, y);
	printf("Our estimated state is:\n");
	print(x, ADIM, 1);

	clock_t end = clock();
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;
	printf("Total time was %f", seconds);

	return EXIT_SUCCESS;
}

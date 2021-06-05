/*
 * nonlinsolve.c
 *
 *  Created on: 5 june 2021
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

static float check_solution(float* dx, float* x, float* past_sqrt_sum_dx, float* best_x, uint8_t* elements);

void nonlinsolve(void (*nonlinear_equation_system)(float*, float*, float*), float* b, float* x, uint8_t elements, float alpha, float max_value, float min_value, bool random_guess_active){
	// Initial parameters and arrays
	float dx[elements];
	float best_sqrt_sum_dx = FLT_MAX;
	float past_sqrt_sum_dx = 0;
	float best_x[elements];
	float sqrt_sum_dx = 1;
	uint8_t times_until_break = 10;
	uint8_t break_count = 0;

	// Do random guesses
	if(random_guess_active){
		srand(time(NULL));
		float difference_value = max_value - min_value;
		for(uint32_t i = 0; i < 20000; i++){
			// Init x with random values between min_value and max_value
			for(uint8_t j = 0; j < elements; j++)
				*(x + j) = difference_value*((float) rand() / RAND_MAX) + min_value;

			// Simulate the nonlinear system
			(*nonlinear_equation_system)(dx, b, x);

			// Check the solution if it's good or not
			check_solution(dx, x, &best_sqrt_sum_dx, best_x, &elements);
		}

		// Use gradient to find the best solution, beginning from best_x vector
		memcpy(x, best_x, sizeof(float)*elements);
	}
	for(uint32_t i = 0; i < 200000; i++){
		// Simulate the nonlinear system
		(*nonlinear_equation_system)(dx, b, x);

		// Check the solution if it's good or not
		sqrt_sum_dx = check_solution(dx, x, &best_sqrt_sum_dx, best_x, &elements);

		// If sqrt_sum_dx stands still - break
		if(past_sqrt_sum_dx == sqrt_sum_dx){
			break_count++;
			if(break_count >= times_until_break)
				break;
		}
		past_sqrt_sum_dx = sqrt_sum_dx;

		// Update the vector x using gradient descent
		for(uint8_t j = 0; j < elements; j++)
			*(x + j) = *(x + j) - alpha * *(dx + j); // x = x + alpha * dx
	}

	// Copy over our final solution x
	memcpy(x, best_x, sizeof(float)*elements);
}

static float check_solution(float* dx, float* x, float* best_sqrt_sum_dx, float* best_x, uint8_t* elements){
	// Do L2-norm on dx
	float sqrt_sum_dx = norm(dx, 1, *elements, 2);

	// Now we are finding the best solution to the function
	if(*best_sqrt_sum_dx > sqrt_sum_dx){
		*best_sqrt_sum_dx = sqrt_sum_dx;
		memcpy(best_x, x, sizeof(float)* *elements);
	}

	return sqrt_sum_dx;
}

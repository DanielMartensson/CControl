/*
 ============================================================================
 Name        : Main.c
 Author      : Daniel Mårtensson
 Version     : 1.0
 Copyright   : MIT
 Description : Empty
 ============================================================================
 */

#include "CControl/Headers/Functions.h"

int main() {

	clock_t start, end;
	float cpu_time_used;
	start = clock();

	/*
	 * Do your logic here
	 */

	end = clock();
	cpu_time_used = ((float) (end - start)) / CLOCKS_PER_SEC;
	printf("\nTotal speed  was %f\n", cpu_time_used);
	return EXIT_SUCCESS;
}

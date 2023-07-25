/*
 * concatenate_paths.c
 *
 *  Created on: 24 juli 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

#ifdef _WIN32
void concatenate_paths(char total_path[], const char path_first[], const char path_second[]){
	sprintf(total_path, "%s\\%s", path_first, path_second);
}
#else
void concatenate_paths(char total_path[], const char path_first[], const char path_second[]) {
	sprintf(total_path, "%s/%s", path_first, path_second);
}
#endif
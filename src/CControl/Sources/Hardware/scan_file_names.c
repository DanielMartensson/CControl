/*
 * scan_file_names.c
 *
 *  Created on: 24 juli 2023
 *      Author: Daniel Mårtensson
 */

#include "hardware.h"

#ifdef _WIN32
#undef UNICODE /* We want to use ANSI */
#include <Windows.h>

size_t scan_file_names(const char folder_path[], char** file_names[], const char ending[]) {
	/* Check if null */
	if (folder_path == NULL) {
		return 0;
	}
	
	/* Information about how many counted files */
	char search_path[MAX_PATH];
	concatenate_paths(search_path, folder_path, "*");
	size_t file_count = 0;

	WIN32_FIND_DATA findData;
	HANDLE hFind = FindFirstFile(search_path, &findData);
	if (hFind == INVALID_HANDLE_VALUE) {
		return 0;
	}

	bool exist = true;
	while (exist) {
		/* Ignore the virtual folders "." and ".." */
		if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0) {
			/* If there is a file */
			if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				/* Get file name */
				char* file_name = findData.cFileName;

				/* Check if the file is... */
				if (ending) {
					if (!strstr(file_name, ending)) {
						exist = FindNextFile(hFind, &findData);
						continue;
					}
				}

				/* Yes */
				(*file_names) = (char**)realloc(*file_names, (file_count + 1) * sizeof(char*));
				(*file_names)[file_count] = (char*)malloc(strlen(file_name) + 1);
				strcpy((*file_names)[file_count], file_name);
				file_count++;
			}
		}

		/* Pick next file */
		exist = FindNextFile(hFind, &findData);
	}

	FindClose(hFind);
	return file_count;
}
#else
size_t scan_file_names(const char folder_path[], char** file_names[], const char ending[]) {
	printf("scan_file_names.c not implemented for your operative system!\n");
	return 0;
}
#endif
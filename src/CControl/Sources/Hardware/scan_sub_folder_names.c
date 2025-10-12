/*
 * scan_sub_folder_names.c
 *
 *  Created on: 24 juli 2023
 *      Author: Daniel Mårtensson
 */

#include "hardware.h"

/*
 * Return the amount of sub folders and also their name. Call sub_folder_names with a pointer
 */
#ifdef _WIN32
#undef UNICODE /* We want to use ANSI */
#include <Windows.h>

size_t scan_sub_folder_names(const char folder_path[], char** sub_folder_names[]) {
	/* Information about how many counted folders */
	char search_path[MAX_PATH];
	concatenate_paths(search_path, folder_path, "*");
	size_t sub_folder_count = 0;

	WIN32_FIND_DATA findData;
	HANDLE hFind = FindFirstFile(search_path, &findData);
	if (hFind == INVALID_HANDLE_VALUE) {
		return 0;
	}

	bool exist = true;
	while (exist) {
		/* Ignore the virtual folders "." and ".." */
		if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0) {
			/* If there is a folder */
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				char* sub_folder_name = findData.cFileName;
				(*sub_folder_names) = (char**)realloc(*sub_folder_names, (sub_folder_count + 1) * sizeof(char*));
				(*sub_folder_names)[sub_folder_count] = (char*)malloc(strlen(sub_folder_name) + 1);
				strcpy((*sub_folder_names)[sub_folder_count], sub_folder_name);
				sub_folder_count++;
			}
		}
		exist = FindNextFile(hFind, &findData);
	}

	FindClose(hFind);
	return sub_folder_count;
}
#else
size_t scan_sub_folder_names(const char folder_path[], char** sub_folder_names[]) {
	return 0;
}
#endif
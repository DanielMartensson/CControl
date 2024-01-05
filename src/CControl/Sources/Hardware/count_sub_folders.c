/*
 * count_sub_folders.c
 *
 *  Created on: 24 juli 2023
 *      Author: Daniel Mårtensson
 */

#include "hardware.h"

/*
 * Return the amount of sub folders
 */
#ifdef _WIN32
#undef UNICODE /* We want to use ANSI */
#include <Windows.h>

size_t count_sub_folders(const char folder_path[]) {
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
				sub_folder_count++;
			}
		}
		exist = FindNextFile(hFind, &findData);
	}

	FindClose(hFind);
	return sub_folder_count;
}
#else
size_t count_sub_folders(const char folder_path[]) {
	return 0;
}
#endif // !_WIN32


/*
 * count_folders.c
 *
 *  Created on: 24 juli 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

#ifdef _WIN32
#undef UNICODE
#include <Windows.h>

size_t count_folders(const char folderPath[]) {
    /* Information about how many counted folders */
    char searchPath[MAX_PATH];
    sprintf(searchPath, "%s\\*", folderPath);
    size_t folderCount = 0;

    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(searchPath, &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        return 0;
    }

    do {
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            /* Check if it's a folder */
            if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0) {
                folderCount++;
            }
        }
    } while (FindNextFile(hFind, &findData));

    FindClose(hFind);
    return folderCount;
}
#endif // !_WIN32


#ifndef HARDWARE_H_
#define HARDWARE_H_

#include "../../Headers/headers.h"

#ifdef __cplusplus
extern "C" {
#endif

void concatenate_paths(char total_path[], const char path_first[], const char path_second[]);
size_t count_sub_folders(const char folder_path[]);
void detectmemoryleak();
size_t scan_file_names(const char folder_path[], char** file_names[], const char ending[]);
size_t scan_sub_folder_names(const char folder_path[], char** sub_folder_names[]);
bool saveload(uint8_t data[], const uint32_t data_length, char file_name[], const bool save);

#ifdef __cplusplus
}
#endif

#endif /* !HARDWARE_H_ */

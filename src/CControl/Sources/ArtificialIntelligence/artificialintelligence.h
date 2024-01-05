#ifndef ARTIFICIAL_INTELLIGENCE_H_
#define ARTIFICIAL_INTELLIGENCE_H_

#include "../../Headers/headers.h"

#ifdef __cplusplus
extern "C" {
#endif

void Astar(int32_t map[], int32_t path_x[], int32_t path_y[], size_t x_start, size_t y_start, size_t x_stop, size_t y_stop, size_t height, size_t width, ASTAR_MODE astar_mode, size_t* steps);
bool inpolygon(float x, float y, float px[], float py[], size_t p);

#ifdef __cplusplus
}
#endif

#endif /* !ARTIFICIAL_INTELLIGENCE_H_ */

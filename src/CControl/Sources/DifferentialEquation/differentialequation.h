#ifndef DIFFERENTIAL_EQUATION_H_
#define DIFFERENTIAL_EQUATION_H_

#include "../../Headers/headers.h"

#ifdef __cplusplus
extern "C" {
#endif

void rk4(const size_t iterations, const float h, float Y[], float y[], const size_t N, void (*odefun)(float, float*));

#ifdef __cplusplus
}
#endif

#endif /* !DIFFERENTIAL_EQUATION_H_ */

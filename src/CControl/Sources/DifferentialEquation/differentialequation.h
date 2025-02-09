#ifndef DIFFERENTIAL_EQUATION_H_
#define DIFFERENTIAL_EQUATION_H_

#include "../../Headers/headers.h"

#ifdef __cplusplus
extern "C" {
#endif

void rk4(const bool no_ode_output, const size_t iterations, const float h, float Y[], float y[], const size_t N, void (*odefun)(float, float*));
void rk4args(const bool no_ode_output, const size_t iterations, const float h, float Y[], float y[], const size_t N, void (*odefun)(float, float*, float**, const size_t*, const size_t*), const size_t number_of_pointers, ...);

#ifdef __cplusplus
}
#endif

#endif /* !DIFFERENTIAL_EQUATION_H_ */

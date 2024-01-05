#ifndef OPTIMIZATION_H_
#define OPTIMIZATION_H_

#include "../../Headers/headers.h"

#ifdef __cplusplus
extern "C" {
#endif

bool linprog(float c[], float A[], float b[], float x[], size_t row_a, size_t column_a, bool maximization);
bool lsqnonneg(float A[], float x[], float b[], float residual[], size_t row, size_t column);
bool quadprog(float Q[], float c[], float A[], float b[], float G[], float h[], float x[], size_t row_a, size_t row_g, size_t column_a, bool equality_constraints_are_used);

#ifdef __cplusplus
}
#endif

#endif /* !OPTIMIZATION_H_ */

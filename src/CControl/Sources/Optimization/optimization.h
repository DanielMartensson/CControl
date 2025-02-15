#ifndef OPTIMIZATION_H_
#define OPTIMIZATION_H_

#include "../../Headers/headers.h"

#ifdef __cplusplus
extern "C" {
#endif

bool linprog(float c[], float A[], float b[], float x[], size_t row_a, size_t column_a, bool maximization);
bool lsqnonneg(float A[], float x[], float b[], float residual[], size_t row, size_t column);
STATUS_CODES quadprog(const float Q[], const float c[], const float A[], const float b[], const float G[], const float h[], float x[], const size_t row_a, const size_t row_g, const size_t column_a, const bool equality_constraints_are_used);
STATUS_CODES quadprogslim(const float Q[], const float c[], const float A[], const float b[], const float G[], const float h[], float x[], const size_t row_a, const size_t row_g, const size_t column_a, const bool equality_constraints_are_used);

#ifdef __cplusplus
}
#endif

#endif /* !OPTIMIZATION_H_ */

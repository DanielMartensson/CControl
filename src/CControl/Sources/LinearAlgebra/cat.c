#include <assert.h>
#include "../../Headers/Functions.h"

void cat(uint16_t  dim, float A[], float B[], float C[], uint16_t  rowA,
		uint16_t  colA, uint16_t  rowB, uint16_t  colB, uint16_t  rowC, uint16_t  colC)
{
	if (dim == 1) /* C = [A;B] */
	{
		assert(colA == colB);
		assert(rowA + rowB == rowC);

		memcpy(C, A, rowA * colA * sizeof(double));
		memcpy(C + rowA * colA, B, rowB * colB * sizeof(double));
	}
	else if (dim == 2) /* C = [A, B] */
	{
		assert(rowA == rowB);
		assert(colA + colB == colC);

		memcpy(C, A, colA * sizeof(double));
		memcpy(C + colA, B, colB * sizeof(double));
		memcpy(C + colA + colB, A + colA, colA * sizeof(double));
		memcpy(C + colA + colB + colA, B + colB, colB * sizeof(double));
	}

}


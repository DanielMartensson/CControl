/*
 * ind2sub.c
 *
 *  Created on: December 23, 2023
 *      Author: Daniel Mårtensson
 */

#include "miscellaneous.h"

/*
 * This is the MATLAB function ind2sub for generate the coordinate of an index at a matrix
 */
void ind2sub(const size_t index, const size_t column, size_t* row_index, size_t* column_index) {
    *row_index = index / column;
    *column_index = index % column;
}

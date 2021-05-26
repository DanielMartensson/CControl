
#include "../../Headers/Functions.h"


/*
 * lsq.c
 *
 *  Created on: 2021.05.02
 *      Author: yushb0602@gmail.com
 *  Updated on: 2021-05-26
 *  	Author: Daniel Mårtensson
 */

/*
 * least-squares solution for Ax=b   -> (A.T A + alpha*I) x = (A.T)*b
 * The big consideration is how to reduce the memory space. there is no  A' transpose matrix inside.   
 * in many cases, row_a is times bigger than col_a
 * Returns 1 == Success
 * Returns 0 == Fail
 */


uint8_t linsolve_lsq(float* A, float* x, float* b, int row_a, int column_a, float alpha){
	// Compute the tikhonov then solve
    float ATA[column_a*column_a];
    float ATb[column_a];
    tikhonov(A, b, ATA, ATb, row_a, column_a, alpha);
    return linsolve_lup(ATA, x, ATb, column_a);
}

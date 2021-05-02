
#include "../../Headers/Functions.h"


/*
 * lsq.c
 *
 *  Created on: 2021.05.02
 *      Author: yushb0602@gmail.com
 */

/*
 * least-squares solution for Ax=b   -> (A.T A) x = (A.T)*b
 * The big consideration is how to reduce the memory space. there is no  A' transpose matrix inside.   
 * in many cases, row_a is times bigger than col_a
 * Returns 1 == Success
 * Returns 0 == Fail
 */


int linsolve_lsq(float* A, float* x, float* b, int row_a, int col_a)
{	
	//caculate Covariance matrix as  A'A first to avoid storing  A' transpose 
    float ATA[col_a*col_a];    
    for(int r= 0; r<col_a; r++){
	    for(int c= r; c<col_a; c++) {
	        float sum = 0;
	        for(int k = 0; k<row_a; k++)  sum += A[r+col_a*k]*A[c+col_a*k];
	        ATA[r+col_a*c] = sum;
	        ATA[col_a*r+c] = sum;
	    }
	}

    float ATB[col_a*col_a];   
    for(int c = 0; c<col_a; c++){
        float sum = 0.0;
        for(int k = 0; k<row_a;k++) sum += A[k*col_a+c]*b[k];
        ATB[c] = sum;
    }

    int row_ata = col_a;
    int rt = linsolve_lup(ATA, x, ATB, row_ata);

    return rt;
}

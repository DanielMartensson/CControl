/*
 * pdist2.c
 *
 *  Created on: 22 juli 2023
 *      Author: Daniel Mårtensson
 */

#include "miscellaneous.h"

/*
 * Compute pdist2
 * A[row_a*column_a]
 * B[row_b*column_a]
 * C[row_a*row_b]
 */
void pdist2(const float A[], const float B[], float C[], const size_t row_a, const size_t column_a, const size_t row_b, const PDIST2_METRIC metric) {
	/* Get size */
	const size_t row_a_column_a = row_a * column_a;
	const size_t sizeBytesA = row_a_column_a * sizeof(float);
	const size_t row_b_column_a = row_b * column_a;
	const size_t sizeBytesB = row_b_column_a * sizeof(float);
	size_t i, j;
	switch (metric) {
	case PDIST2_METRIC_SQEUCLIDEAN: {
		/* MATLAB: Bt = B' */
		float* Bt = (float*)malloc(sizeBytesB);
		memcpy(Bt, B, sizeBytesB);
		tran(Bt, row_b, column_a);

		/* 
		 * MATLAB: AA = sum(A.*A, 2)
		 */
		float* AtimesA = (float*)malloc(sizeBytesA);
		for (i = 0; i < row_a_column_a; i++) {
			AtimesA[i] = A[i] * A[i];
		}
		float* AA = (float*)malloc(row_a * sizeof(float));
		sum(AtimesA, AA, row_a, column_a, false);
		
		/* 
		 * MATLAB: BB = sum(Bt.*Bt, 1);
		 */
		float* BttimesBt = (float*)malloc(sizeBytesB);
		for (i = 0; i < row_b_column_a; i++) {
			BttimesBt[i] = Bt[i] * Bt[i];
		}
		float* BB = (float*)malloc(row_b * sizeof(float));
		sum(BttimesBt, BB, column_a, row_b, true);

		/* MATLAB: C = AA + BB - 2*A*Bt */
		mul(A, Bt, C, row_a, column_a, row_b);
		for (i = 0; i < row_a; i++) {
			for (j = 0; j < row_b; j++) {
				C[j] = AA[i] + BB[j] - 2.0f * C[j];
				if (C[j] < 0.0f) {
					C[j] = fabsf(C[j]);
				}
			}
			C += row_b;
		}

		/* Free */
		free(Bt);
		free(AtimesA);
		free(BttimesBt);
		free(AA);
		free(BB);
		break;
	}
	case PDIST2_METRIC_EUCLIDEAN: {
		pdist2(A, B, C, row_a, column_a, row_b, PDIST2_METRIC_SQEUCLIDEAN);
		const size_t row_a_row_b = row_a * row_b;
		for (i = 0; i < row_a_row_b; i++) {
			C[i] = sqrtf(C[i]);
		}
		break;
	}
	}
}

/* GNU Octave code:
 *  
 * 
	 switch metric
	  case {0,'sqeuclidean'}
		D = distEucSq( X, Y );
	  case 'euclidean'
		D = sqrt(distEucSq( X, Y ));
	  case 'L1'
		D = distL1( X, Y );
	  case 'cosine'
		D = distCosine( X, Y );
	  case 'emd'
		D = distEmd( X, Y );
	  case 'chisq'
		D = distChiSq( X, Y );
	  otherwise
		error(['pdist2 - unknown metric: ' metric]);
	end
	D = max(0,D);
	end

	function D = distL1( X, Y )
		m = size(X,1);  n = size(Y,1);
		mOnes = ones(1,m); D = zeros(m,n);
		for i=1:n
		  yi = Y(i,:);  yi = yi( mOnes, : );
		  D(:,i) = sum( abs( X-yi),2 );
		end
	end

	function D = distCosine( X, Y )
		p=size(X,2);
		XX = sqrt(sum(X.*X,2)); X = X ./ XX(:,ones(1,p));
		YY = sqrt(sum(Y.*Y,2)); Y = Y ./ YY(:,ones(1,p));
		D = 1 - X*Y';
	end

	function D = distEmd( X, Y )
		Xcdf = cumsum(X,2);
		Ycdf = cumsum(Y,2);
		m = size(X,1);  n = size(Y,1);
		mOnes = ones(1,m); D = zeros(m,n);
		for i=1:n
		  ycdf = Ycdf(i,:);
		  ycdfRep = ycdf( mOnes, : );
		  D(:,i) = sum(abs(Xcdf - ycdfRep),2);
		end
	end

	function D = distChiSq( X, Y )
		% note: supposedly it's possible to implement this without a loop!
		m = size(X,1);  n = size(Y,1);
		mOnes = ones(1,m); D = zeros(m,n);
		for i=1:n
		  yi = Y(i,:);  yiRep = yi( mOnes, : );
		  s = yiRep + X;    d = yiRep - X;
		  D(:,i) = sum( d.^2 ./ (s+eps), 2 );
		end
		D = D/2;
	end
 

	function D = distEucSq(X, Y)
	  Yt = Y';
	  XX = sum(X.*X,2);
	  YY = sum(Yt.*Yt,1);
	  D = bsxfun(@plus,XX,YY)-2*X*Yt;
	end

*/

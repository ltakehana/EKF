/**********************************************************************************************
***********************************************************************************************
***  File:			GMatrix_statistics.c
***	 Author:		Geovany Araujo Borges
***	 Contents:		Implementation of Matrix type and related functions for use in C and C++.
***					Statistics functions
***********************************************************************************************
***********************************************************************************************
	This file is part of gMatrix.

    gMatrix is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 3 of the License, or any later version.

    gMatrix is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with gMatrix.  If not, see <http://www.gnu.org/licenses/>.

	Copyright 2010 Geovany Araujo Borges
**********************************************************************************************/

//#include "stdafx.h" // MSVC6.0 may require this.
#include "Gmatrix.h"
#include "Gmatrix_linalg.h"
#include "Gmatrix_statistics.h"

/**********************************************************************************************
***** GMatrix: Includes.
**********************************************************************************************/
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/**********************************************************************************************
***** GMatrix: Statistics
**********************************************************************************************/

#if GMATRIX_DEFINE_MEAN
GMATRIX_FLOAT PGMATRIX_MEAN(PGMATRIX pMat)
{
	int i,j;
	GMATRIX_FLOAT sum = 0.0;
	for(i=1;i<=pMat->Nr;++i){	
		for(j=1;j<=pMat->Nc;++j){	
			sum += PGMATRIX_DATA(pMat,i,j);
		}	
	}
	return(sum/(pMat->Nr*pMat->Nc));
}
#endif

#if GMATRIX_DEFINE_VARIANCE
GMATRIX_FLOAT PGMATRIX_VARIANCE(PGMATRIX pMat)
{
	int i,j;
	GMATRIX_FLOAT sum = 0.0;
	GMATRIX_FLOAT mean;

	mean = PGMATRIX_MEAN(pMat);
	for(i=1;i<=pMat->Nr;++i){	
		for(j=1;j<=pMat->Nc;++j){	
			sum += GMATRIXMACRO_SQR(PGMATRIX_DATA(pMat,i,j)-mean);
		}	
	}
	return(sum/(pMat->Nr*pMat->Nc-1));
}
#endif

/**********************************************************************************************
***** GMatrix: Covariance propagation
**********************************************************************************************/

#if GMATRIX_DEFINE_COVARIANCE_PROPAGATION_COPY
void PGMATRIX_COVARIANCE_PROPAGATION_COPY(PGMATRIX pMatCovResult, PGMATRIX pMatGain, PGMATRIX pMatCov, PGMATRIX pMatDummy)
{
	GMATRIX_ASSERT("PGMATRIX_COVARIANCE_PROPAGATION_COPY",pMatGain->Nc != pMatCov->Nr); 
	PGMATRIX_MULTIPLY_COPY_EXTENDED(pMatDummy, pMatCov, 0, pMatGain, 1);	
	PGMATRIX_MULTIPLY_COPY(pMatCovResult, pMatGain, pMatDummy);
}
#endif

#if GMATRIX_DEFINE_COVARIANCE_PROPAGATION_ADD
void PGMATRIX_COVARIANCE_PROPAGATION_ADD(PGMATRIX pMatCovResult, PGMATRIX pMatGain, PGMATRIX pMatCov, PGMATRIX pMatDummy)
{
	GMATRIX_ASSERT("PGMATRIX_COVARIANCE_PROPAGATION_ADD",pMatGain->Nc != pMatCov->Nr); 
	PGMATRIX_MULTIPLY_COPY_EXTENDED(pMatDummy, pMatCov, 0, pMatGain, 1);	
	PGMATRIX_MULTIPLY_ADD(pMatCovResult, pMatGain, pMatDummy);
}
#endif

/**********************************************************************************************
***** GMatrix: Mahalanobis Distance
**********************************************************************************************/

#if GMATRIX_DEFINE_MAHALANOBIS_DISTANCE
GMATRIX_FLOAT PGMATRIX_MAHALANOBIS_DISTANCE(PGMATRIX pMatResidual, PGMATRIX pMatCovResidual, PGMATRIX pMatDummy1, PGMATRIX pMatDummy2)
{
	GMATRIX_ASSERT("PGMATRIX_MAHALANOBIS_DISTANCE",pMatCovResidual->Nc != pMatCovResidual->Nr); 
	GMATRIX_ASSERT("PGMATRIX_MAHALANOBIS_DISTANCE",pMatResidual->Nr != pMatCovResidual->Nr); 
	GMATRIX_ASSERT("PGMATRIX_MAHALANOBIS_DISTANCE",pMatResidual->Nc != 1); 
//	PGMATRIX_COPY(pMatDummy1, pMatCovResidual); // Inverse using gauss_jordan decomposition.
//	PGMATRIX_INVERSE(pMatDummy1);	// pMatDummy1 = inv(pMatCovResidual);
	PGMATRIX_INVERSE_COPY(pMatDummy1,pMatCovResidual);	// pMatDummy1 = inv(pMatCovResidual);
	PGMATRIX_MULTIPLY_COPY(pMatDummy2, pMatDummy1, pMatResidual); // pMatDummy2 = inv(pMatCovResidual)*pMatResidual;
	PGMATRIX_MULTIPLY_COPY_EXTENDED(pMatDummy1, pMatResidual,1,pMatDummy2,0); // pMatDummy1 = pMatResidual'*inv(pMatCovResidual)*pMatResidual;
	return(PGMATRIX_DATA(pMatDummy1,1,1));
}
#endif

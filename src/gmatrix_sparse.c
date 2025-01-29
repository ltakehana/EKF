/**********************************************************************************************
***********************************************************************************************
***  File:			Gmatrix_sparse.c
***	 Author:		Geovany Araujo Borges
***	 Contents:		Implementation of Matrix type and related functions for use in C and C++.
***					Starsity handling functions
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
#include "Gmatrix_sparse.h"

/**********************************************************************************************
***** GMatrix: Includes.
**********************************************************************************************/
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/**********************************************************************************************
***** GMatrix: Starsity Coefficient. Returns a number which is the ratio between the number 
*****          of zeroes in a matrix and its size.
**********************************************************************************************/
#if GMATRIX_DEFINE_SPARSITY_COEFFICIENT
GMATRIX_FLOAT PGMATRIX_SPARSITY_COEFFICIENT(PGMATRIX pMat, GMATRIX_FLOAT threshold)
{
	int i,j;
	long int nzeroes = 0;

	GMATRIX_ASSERT("GMATRIX_SPARSITY_COEFFICIENT",pMat->Nr <= 0);	
	GMATRIX_ASSERT("GMATRIX_SPARSITY_COEFFICIENT",pMat->Nc <= 0);	
	GMATRIX_ASSERT("GMATRIX_SPARSITY_COEFFICIENT",pMat->MaxSize <= 0);	

	for (i=1;i<=pMat->Nr;i++)  {
		for (j=1;j<=pMat->Nc;j++)  {
			if (fabs(PGMATRIX_DATA(pMat,i,j)) <= threshold) ++nzeroes;
		}
	} 

	return ((GMATRIX_FLOAT)(nzeroes))/(((GMATRIX_FLOAT)(pMat->Nr))*((GMATRIX_FLOAT)(pMat->Nr)));

}
#endif
/**********************************************************************************************
***** GMatrix: Plot GMATRIX_SPARSE. 
*****          
**********************************************************************************************/
void PGMATRIX_SPARSE_PRINT_RAWFORM_NAMED(char* NameString, PGMATRIX_SPARSE pMat)
{
	int i;

	GMATRIX_ASSERT("PGMATRIX_PRINT_RAWFORM_NAMED",pMat == NULL);	
	GMATRIX_PRINTCOMMAND("\n%s = ",NameString);

	GMATRIX_PRINTCOMMAND("\nIndex: ");
	for(i=1;i<=pMat->Index[pMat->Index[1]-1]-1;++i){
		GMATRIX_PRINTCOMMAND("%i ",pMat->Index[i]);
	}
	GMATRIX_PRINTCOMMAND("\nData: ");
	for(i=1;i<=pMat->Index[pMat->Index[1]-1]-1;++i){
		GMATRIX_PRINTCOMMAND("%c%f ",GMATRIXMACRO_SIGNCHAR(pMat->Data[i]),fabs(pMat->Data[i]));
	}
	GMATRIX_PRINTCOMMAND("\n");	
}


/**********************************************************************************************
***** GMatrix: Alloc from full GMATRIX. 
*****          
**********************************************************************************************/

PGMATRIX_SPARSE PGMATRIX_SPARSE_ALLOC(int N)
{	
	PGMATRIX_SPARSE pMatSparse;

	pMatSparse = (GMATRIX_SPARSE*) malloc(sizeof(GMATRIX_SPARSE));
	GMATRIX_ASSERT("PGMATRIX_SPARSE_ALLOC_FROM_GMATRIX",pMatSparse==NULL);	

	pMatSparse->N = N;
	pMatSparse->MaxSize = N * N + 2; //  maximum length of each vector for the sparse representation.
	pMatSparse->Data = (GMATRIX_FLOAT*) malloc(pMatSparse->MaxSize*sizeof(GMATRIX_FLOAT));
	GMATRIX_ASSERT("PGMATRIX_ALLOC",pMatSparse->Data==NULL);	
	pMatSparse->Index = (int*) malloc(pMatSparse->MaxSize*sizeof(int));
	GMATRIX_ASSERT("PGMATRIX_ALLOC",pMatSparse->Index==NULL);	

	return(pMatSparse);
}

//#if GMATRIX_DEFINE_SPARSITY_COEFFICIENT
PGMATRIX_SPARSE PGMATRIX_SPARSE_ALLOC_FROM_GMATRIX(PGMATRIX pMat, GMATRIX_FLOAT threshold)
{	
	int i,j,k;
	PGMATRIX_SPARSE pMatSparse;

	GMATRIX_ASSERT("PGMATRIX_SPARSE_ALLOC_FROM_GMATRIX",pMat->Nr!=pMat->Nc);	

	pMatSparse = PGMATRIX_SPARSE_ALLOC(pMat->Nr);

	// Copy contents to sparse matrix:
	for (j=1;j<=pMat->Nr;j++) 
		pMatSparse->Data[j] = PGMATRIX_DATA(pMat,j,j); // Store diagonal elements.
	pMatSparse->Index[1] = pMat->Nr+2; //Index to 1st row off-diagonal element, if any.
	k = pMat->Nr+1;
	for (i=1;i<=pMat->Nr;i++) { //Loop over rows.
		for (j=1;j<=pMat->Nr;j++) { //Loop over columns.
			if (fabs(PGMATRIX_DATA(pMat,i,j)) > threshold && i != j) {
				if (++k > pMatSparse->MaxSize) GMATRIX_ERROR("PGMATRIX_SPARSE_ALLOC_FROM_GMATRIX: MaxSize too small");
				pMatSparse->Data[k] = PGMATRIX_DATA(pMat,i,j); //Store off-diagonal elements and their columns.
				pMatSparse->Index[k] = j;
			}
		}
	pMatSparse->Index[i+1] = k+1; // As each row is completed, store index tonext. 
	}
	return(pMatSparse);
}

void PGMATRIX_SPARSE_FREE(PGMATRIX_SPARSE pMatrixSparse)
{
	free(pMatrixSparse->Data);
	free(pMatrixSparse->Index);
	free(pMatrixSparse);
}

//#endif

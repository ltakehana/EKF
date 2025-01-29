/**********************************************************************************************
***********************************************************************************************
***  File:			Gmatrix_sparse.h
***	 Author:		Geovany Araujo Borges
***	 Contents:		Gmatrix_sparse header file.
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

/**********************************************************************************************
***** GMatrix: Type Definitions for specific sparse representations
**********************************************************************************************/

/*! 
 *  GMATRIX_SPARSE is the data structure of a matrix or a vector. 
 *  PGMATRIX_SPARSE is a pointer for GMATRIX_SPARSE.
 */

// Main GMATRIX_SPARSE type structure and pointer
typedef struct{	
		int N;		 /* Matrix dimension N x N */
		int MaxSize; /* Number of entries of *Data. */
		GMATRIX_FLOAT *Data;  /* Pointer to entries of GMATRIX_SPARSE. The entries are row organized */	
		int *Index; /* Pointer to entries of GMATRIX_SPARSE. The entries are row organized */	
} GMATRIX_SPARSE, *PGMATRIX_SPARSE;

/**********************************************************************************************
***** GMatrix: Function prototypes
**********************************************************************************************/

#define GMATRIX_SPARSITY_COEFFICIENT(Mat,tolerance) PGMATRIX_SPARSITY_COEFFICIENT(&Mat,tolerance)
GMATRIX_FLOAT PGMATRIX_SPARSITY_COEFFICIENT(PGMATRIX pMat, GMATRIX_FLOAT tolerance);
void PGMATRIX_SPARSE_PRINT_RAWFORM_NAMED(char* NameString, PGMATRIX_SPARSE pMat);
PGMATRIX_SPARSE PGMATRIX_SPARSE_ALLOC(int N);
PGMATRIX_SPARSE PGMATRIX_SPARSE_ALLOC_FROM_GMATRIX(PGMATRIX pMat, GMATRIX_FLOAT threshold);
void PGMATRIX_SPARSE_FREE(PGMATRIX_SPARSE pMatrixSparse);

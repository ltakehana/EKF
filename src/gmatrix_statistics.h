/**********************************************************************************************
***********************************************************************************************
***  File:			GMatrix_statistics.h
***	 Author:		Geovany Araujo Borges
***	 Contents:		GMatrix_statistics header file.
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
***** GMatrix: Function prototypes
**********************************************************************************************/

#define GMATRIX_MEAN(Mat) PGMATRIX_MEAN(&Mat)
GMATRIX_FLOAT PGMATRIX_MEAN(PGMATRIX pMat);

#define GMATRIX_VARIANCE(Mat) PGMATRIX_VARIANCE(&Mat)
GMATRIX_FLOAT PGMATRIX_VARIANCE(PGMATRIX pMat);

#define GMATRIX_COVARIANCE_PROPAGATION_COPY(MatCovResult, MatGain, MatCov, MatDummy) PGMATRIX_COVARIANCE_PROPAGATION_COPY(&MatCovResult, &MatGain, &MatCov, &MatDummy)
void PGMATRIX_COVARIANCE_PROPAGATION_COPY(PGMATRIX pMatCovResult, PGMATRIX pMatGain, PGMATRIX pMatCov, PGMATRIX pMatDummy);

#define GMATRIX_COVARIANCE_PROPAGATION_ADD(MatCovResult, MatGain, MatCov, MatDummy) PGMATRIX_COVARIANCE_PROPAGATION_ADD(&MatCovResult, &MatGain, &MatCov, &MatDummy)
void PGMATRIX_COVARIANCE_PROPAGATION_ADD(PGMATRIX pMatCovResult, PGMATRIX pMatGain, PGMATRIX pMatCov, PGMATRIX pMatDummy);

#define GMATRIX_MAHALANOBIS_DISTANCE(MatResidual, MatCovResidual, MatDummy1, MatDummy2) PGMATRIX_MAHALANOBIS_DISTANCE(&MatResidual, &MatCovResidual, &MatDummy1, &MatDummy2)
GMATRIX_FLOAT PGMATRIX_MAHALANOBIS_DISTANCE(PGMATRIX pMatResidual, PGMATRIX pMatCovResidual, PGMATRIX pMatDummy1, PGMATRIX pMatDummy2);


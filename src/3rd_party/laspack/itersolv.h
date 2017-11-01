/****************************************************************************/
/*                                itersolv.h                                */
/****************************************************************************/
/*                                                                          */
/* ITERative SOLVers for systems of linear equations                        */
/*                                                                          */
/* Copyright (C) 1992-1995 Tomas Skalicky. All rights reserved.             */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*        ANY USE OF THIS CODE CONSTITUTES ACCEPTANCE OF THE TERMS          */
/*              OF THE COPYRIGHT NOTICE (SEE FILE COPYRGHT.H)               */
/*                                                                          */
/****************************************************************************/

#ifndef ITERSOLV_H
#define ITERSOLV_H

#include "vector.h"
#include "qmatrix.h"
#include "precond.h"
#include "eigenval.h"
#include "copyrght.h"

#ifdef __cplusplus
extern "C"
{
#endif
typedef Vector *(*IterProcType)(QMatrix_L *, Vector *, Vector *, int,
				PrecondProcType, double);

/* classical iterative methods */

Vector *JacobiIter(QMatrix_L *A, Vector *x, Vector *b, int NoIter,
	    PrecondProcType Dummy, double Omega);
Vector *SORForwIter(QMatrix_L *A, Vector *x, Vector *b, int NoIter,
            PrecondProcType Dummy, double Omega);
Vector *SORBackwIter(QMatrix_L *A, Vector *x, Vector *b, int NoIter,
            PrecondProcType Dummy, double Omega);
Vector *SSORIter(QMatrix_L *A, Vector *x, Vector *b, int NoIter,
            PrecondProcType Dummy, double Omega);

/* semi-iterative methods */

Vector *ChebyshevIter(QMatrix_L *A, Vector *x, Vector *b, int MaxIter,
            PrecondProcType PrecondProc, double OmegaPrecond);

/* CG and CG-like methods */

Vector *CGIter(QMatrix_L *A, Vector *x, Vector *b, int MaxIter, 
            PrecondProcType PrecondProc, double OmegaPrecond);
Vector *CGNIter(QMatrix_L *A, Vector *x, Vector *b, int MaxIter, 
            PrecondProcType PrecondProc, double OmegaPrecond);
Vector *GMRESIter(QMatrix_L *A, Vector *x, Vector *b, int MaxIter, 
            PrecondProcType PrecondProc, double OmegaPrecond);
Vector *BiCGIter(QMatrix_L *A, Vector *x, Vector *b, int MaxIter,
            PrecondProcType PrecondProc, double OmegaPrecond);
Vector *QMRIter(QMatrix_L *A, Vector *x, Vector *b, int MaxIter,
            PrecondProcType PrecondProc, double OmegaPrecond);
Vector *CGSIter(QMatrix_L *A, Vector *x, Vector *b, int MaxIter,
            PrecondProcType PrecondProc, double OmegaPrecond);
Vector *BiCGSTABIter(QMatrix_L *A, Vector *x, Vector *b, int MaxIter,
            PrecondProcType PrecondProc, double OmegaPrecond);
void SetGMRESRestart(int MaxSteps);
#ifdef __cplusplus
}
#endif
#endif /* ITERSOLV_H */

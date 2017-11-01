/****************************************************************************/
/*                                eigenval.h                                */
/****************************************************************************/
/*                                                                          */
/* estimation of extremal EIGENVALues                                       */
/*                                                                          */
/* Copyright (C) 1992-1995 Tomas Skalicky. All rights reserved.             */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*        ANY USE OF THIS CODE CONSTITUTES ACCEPTANCE OF THE TERMS          */
/*              OF THE COPYRIGHT NOTICE (SEE FILE COPYRGHT.H)               */
/*                                                                          */
/****************************************************************************/

#ifndef EIGENVAL_H
#define EIGENVAL_H

#include "vector.h"
#include "qmatrix.h"
#include "precond.h"
#include "copyrght.h"

/* estimation of extremal eigenvalues */

#ifdef __cplusplus
extern "C"
{
#endif
void SetEigenvalAccuracy(double Eps);
double GetMinEigenval(QMatrix_L *Q, PrecondProcType PrecondProc, double OmegaPrecond);
double GetMaxEigenval(QMatrix_L *Q, PrecondProcType PrecondProc, double OmegaPrecond);
#ifdef __cplusplus
}
#endif
#endif /* EIGENVAL_H */

/****************************************************************************/
/*                                precond.h                                 */
/****************************************************************************/
/*                                                                          */
/* PRECONDitioners for iterative solvers of systems of linear equations     */
/*                                                                          */
/* Copyright (C) 1992-1995 Tomas Skalicky. All rights reserved.             */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*        ANY USE OF THIS CODE CONSTITUTES ACCEPTANCE OF THE TERMS          */
/*              OF THE COPYRIGHT NOTICE (SEE FILE COPYRGHT.H)               */
/*                                                                          */
/****************************************************************************/

#ifndef PRECOND_H
#define PRECOND_H

#include "lastypes.h"
#include "vector.h"
#include "qmatrix.h"
#include "copyrght.h"

typedef Vector *(*PrecondProcType)(QMatrix_L *, Vector *, Vector *, double);

/* declaration of preconditioners */
#ifdef __cplusplus
extern "C"
{
#endif
Vector *JacobiPrecond(QMatrix_L *A, Vector *y, Vector *c, double Omega);
Vector *SSORPrecond(QMatrix_L *A, Vector *y, Vector *c, double Omega);
Vector *ILUPrecond(QMatrix_L *A, Vector *y, Vector *c, double Omega);
#ifdef __cplusplus
}
#endif
#endif /* PRECOND_H */

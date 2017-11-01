/****************************************************************************/
/*                                operats.h                                 */
/****************************************************************************/
/*                                                                          */
/* basic OPERATionS for the types vector, matrix and qmatrix                */
/*                                                                          */
/* Copyright (C) 1992-1995 Tomas Skalicky. All rights reserved.             */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*        ANY USE OF THIS CODE CONSTITUTES ACCEPTANCE OF THE TERMS          */
/*              OF THE COPYRIGHT NOTICE (SEE FILE COPYRGHT.H)               */
/*                                                                          */
/****************************************************************************/

#ifndef OPERATS_H
#define OPERATS_H

#include <stdlib.h>

#include "lastypes.h"
#include "vector.h"
#include "matrix.h"
#include "qmatrix.h"
#include "copyrght.h"

#ifdef __cplusplus
extern "C"
{
#endif
Vector *Asgn_VV(Vector *V1, Vector *V2);
Vector *AddAsgn_VV(Vector *V1, Vector *V2);
Vector *SubAsgn_VV(Vector *V1, Vector *V2);
Vector *MulAsgn_VS(Vector *V, double S);
Vector *Add_VV(Vector *V1, Vector *V2);
QMatrix_L *Add_QQ(QMatrix_L *Q1, QMatrix_L *Q2);
Vector *Sub_VV(Vector *V1, Vector *V2);
QMatrix_L *Sub_QQ(QMatrix_L *Q1, QMatrix_L *Q2);
Vector *Mul_SV(double S, Vector *V);
Matrix *Mul_SM(double S, Matrix *M);
QMatrix_L *Mul_SQ(double S, QMatrix_L *Q);
double Mul_VV(Vector *V1, Vector *V2);
Vector *Mul_MV(Matrix *M, Vector *V);
Vector *Mul_QV(QMatrix_L *Q, Vector *V);
Vector *MulInv_QV(QMatrix_L *Q, Vector *V);
Matrix *Transp_M(Matrix *M);
QMatrix_L *Transp_Q(QMatrix_L *Q);
QMatrix_L *Diag_Q(QMatrix_L *Q);
QMatrix_L *Upper_Q(QMatrix_L *Q);
QMatrix_L *Lower_Q(QMatrix_L *Q);
double l1Norm_V(Vector *V);
double l2Norm_V(Vector *V);
double MaxNorm_V(Vector *V);
Vector *OrthoRightKer_VQ(Vector *V, QMatrix_L *Q);
Vector *OrthoLeftKer_VQ(Vector *V, QMatrix_L *Q);
#ifdef __cplusplus
}
#endif
#endif /* OPERATS_H */

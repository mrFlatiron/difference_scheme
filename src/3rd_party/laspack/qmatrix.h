/****************************************************************************/
/*                                qmatrix.h                                 */
/****************************************************************************/
/*                                                                          */
/* type QMATRIX                                                             */
/*                                                                          */
/* Copyright (C) 1992-1995 Tomas Skalicky. All rights reserved.             */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*        ANY USE OF THIS CODE CONSTITUTES ACCEPTANCE OF THE TERMS          */
/*              OF THE COPYRIGHT NOTICE (SEE FILE COPYRGHT.H)               */
/*                                                                          */
/****************************************************************************/

#ifndef QMATRIX_L_H
#define QMATRIX_L_H

#include <stdlib.h>

#include "lastypes.h"
#include "elcmp.h"
#include "vector.h"
#include "copyrght.h"

typedef struct QMatrixType {
    char *Name;
    size_t Dim;
    Boolean Symmetry;
    ElOrderType ElOrder;
    InstanceType Instance;
    int LockLevel;
    double MultiplD;
    double MultiplU;
    double MultiplL;
    Boolean OwnData;
    size_t *Len;
    ElType **El;
    Boolean *ElSorted;
    Boolean *DiagElAlloc;
    ElType **DiagEl;
    Boolean *ZeroInDiag;
    Real *InvDiagEl;
    Boolean UnitRightKer;
    Real *RightKerCmp;
    Boolean UnitLeftKer;
    Real *LeftKerCmp;
    void *EigenvalInfo;
    Boolean *ILUExists;
    struct QMatrixType *ILU;
} QMatrix_L;
#ifdef __cplusplus
extern "C"
{
#endif
void Q_Constr(QMatrix_L *Q, const char *Name, size_t Dim, Boolean Symmetry,
              ElOrderType ElOrder, InstanceType Instance, Boolean OwnData);
void Q_Destr(QMatrix_L *Q);
void Q_SetName(QMatrix_L *Q, char *Name);
const char *Q_GetName(QMatrix_L *Q);
size_t Q_GetDim(QMatrix_L *Q);
Boolean Q_GetSymmetry(QMatrix_L *Q);
ElOrderType Q_GetElOrder(QMatrix_L *Q);
void Q_SetLen(QMatrix_L *Q, size_t RoC, size_t Len);
size_t Q_GetLen(QMatrix_L *Q, size_t RoC);
void Q_SetEntry(QMatrix_L *Q, size_t RoC, size_t Entry, size_t Pos, Real Val);
size_t Q_GetPos(QMatrix_L *Q, size_t RoC, size_t Entry);
Real Q_GetVal(QMatrix_L *Q, size_t RoC, size_t Entry);
void Q_AddVal(QMatrix_L *Q, size_t RoC, size_t Entry, Real Val);

/* macros for fast access */
#define     Q__GetLen(PtrQ, RoC)               (PtrQ)->Len[RoC]
#define     Q__SetEntry(PtrQ, RoC, Entry, Pos_, Val_) { \
                (PtrQ)->El[RoC][Entry].Pos = (Pos_); \
                (PtrQ)->El[RoC][Entry].Val = (Val_); \
            }
#define     Q__GetPos(PtrQ, RoC, Entry)        (PtrQ)->El[RoC][Entry].Pos
#define     Q__GetVal(PtrQ, RoC, Entry)        (PtrQ)->El[RoC][Entry].Val
#define     Q__AddVal(PtrQ, RoC, Entry, Val_) \
                (PtrQ)->El[RoC][Entry].Val += (Val_)

Real Q_GetEl(QMatrix_L *Q, size_t Row, size_t Clm);

void Q_SortEl(QMatrix_L *Q);
void Q_AllocInvDiagEl(QMatrix_L *Q);

void Q_SetKer(QMatrix_L *Q, Vector *RightKer, Vector *LeftKer);
Boolean Q_KerDefined(QMatrix_L *Q);

void **Q_EigenvalInfo(QMatrix_L *Q);

void Q_Lock(QMatrix_L *Q);
void Q_Unlock(QMatrix_L *Q);
#ifdef __cplusplus
}
#endif
#endif /* QMATRIX_L_H */

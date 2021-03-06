/****************************************************************************/
/*                                errhandl.h                                */
/****************************************************************************/
/*                                                                          */
/* ERRor HANDLing routines                                                  */
/*                                                                          */
/* Copyright (C) 1992-1995 Tomas Skalicky. All rights reserved.             */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*        ANY USE OF THIS CODE CONSTITUTES ACCEPTANCE OF THE TERMS          */
/*              OF THE COPYRIGHT NOTICE (SEE FILE COPYRGHT.H)               */
/*                                                                          */
/****************************************************************************/

#ifndef ERRHANDL_H
#define ERRHANDL_H

#include <stdio.h>

#include "copyrght.h"

typedef enum {
    LASOK,
    LASMemAllocErr,
    LASLValErr,
    LASDimErr,
    LASRangeErr,
    LASSymStorErr,
    LASMatrCombErr,
    LASMulInvErr,
    LASElNotSortedErr,
    LASZeroInDiagErr,
    LASZeroPivotErr,
    LASILUStructErr,
    LASBreakdownErr,
    LASUserBreak
} LASErrIdType;
#ifdef __cplusplus
extern "C"
{
#endif
void LASError(LASErrIdType ErrId, const char *ProcName, const char *Object1Name,
              const char *Object2Name, const char *Object3Name);
void LASBreak(void);
LASErrIdType LASResult(void);
void WriteLASErrDescr(FILE *File);
#ifdef __cplusplus
}
#endif
#endif /* ERRHANDL_H */

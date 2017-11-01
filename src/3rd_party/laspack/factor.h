/****************************************************************************/
/*                                factor.h                                  */
/****************************************************************************/
/*                                                                          */
/* incomplete FACTORization for the type qmatrix                            */
/*                                                                          */
/* Copyright (C) 1992-1995 Tomas Skalicky. All rights reserved.             */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*        ANY USE OF THIS CODE CONSTITUTES ACCEPTANCE OF THE TERMS          */
/*              OF THE COPYRIGHT NOTICE (SEE FILE COPYRGHT.H)               */
/*                                                                          */
/****************************************************************************/

#ifndef FACTOR_H
#define FACTOR_H

#include "vector.h"
#include "qmatrix.h"
#include "copyrght.h"

#ifdef __cplusplus
extern "C"
{
#endif
QMatrix_L *ILUFactor(QMatrix_L *Q);
#ifdef __cplusplus
}
#endif
#endif /* FACTOR_H */


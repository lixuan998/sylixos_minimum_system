/**
 * @file
 * atomic library.
 *
 * VxWorks compatibility layer in SylixOS.
 *
 * Copyright (c) 2001-2014 SylixOS Group.
 * All rights reserved.
 *
 * Author: Han.hui <sylixos@gmail.com>
 */

#ifndef __VXWORKS_VXATOMICLIB_H
#define __VXWORKS_VXATOMICLIB_H

#include "vxWorksCommon.h"

/* type define */
typedef int atomicVal_t;

/* definitions */

#define VX_CODE_BARRIER()   KN_SMP_MB()

#define VX_MEM_BARRIER_R()  KN_SMP_RMB()
#define VX_MEM_BARRIER_W()  KN_SMP_WMB()
#define VX_MEM_BARRIER_RW() KN_SMP_MB()

#ifdef __cplusplus
extern "C" {
#endif

/* WARING: VxWorks returns the old value, the following functions return value is the new value */

/* prototypes */

atomicVal_t vxAtomicAdd(atomic_t *target, atomicVal_t value);
atomicVal_t vxAtomicAnd(atomic_t *target, atomicVal_t value);
atomicVal_t vxAtomicDec(atomic_t *target);
atomicVal_t vxAtomicInc(atomic_t *target);
atomicVal_t vxAtomicNand(atomic_t *target, atomicVal_t value);
atomicVal_t vxAtomicOr(atomic_t *target, atomicVal_t value);
atomicVal_t vxAtomicSub(atomic_t *target, atomicVal_t value);
atomicVal_t vxAtomicXor(atomic_t *target, atomicVal_t value);

atomicVal_t vxAtomicClear(atomic_t *target);
atomicVal_t vxAtomicGet(atomic_t *target);
atomicVal_t vxAtomicSet(atomic_t *target, atomicVal_t value);

BOOL        vxAtomicCas(atomic_t *target, atomicVal_t oldValue, atomicVal_t newValue);

#ifdef __cplusplus
}
#endif

#endif /* __VXWORKS_VXATOMICLIB_H */

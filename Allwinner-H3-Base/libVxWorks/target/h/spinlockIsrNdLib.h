/**
 * @file
 * spinlock common library.
 *
 * VxWorks compatibility layer in SylixOS.
 *
 * Copyright (c) 2001-2018 SylixOS Group.
 * All rights reserved.
 *
 * Author: Han.hui <sylixos@gmail.com>
 */

#ifndef __VXWORKS_SPINLOCKISRNDLIB_H
#define __VXWORKS_SPINLOCKISRNDLIB_H

#include "vxWorksCommon.h"

/* NOTICE: you can change SPIN_LOCK_ND_USE_QUICK_LOCK to 1 and rebuild libVxWorks
 * make spin lock nd use SylixOS Quick spinlock */

#define SPIN_LOCK_ND_USE_QUICK_LOCK   0

/* types */

typedef struct spinlockIsrNd_s {
    spinlock_t  lock;
} spinlockIsrNd_t;

/* macros */

#define SPIN_LOCK_ISR_ND_DECL(lock)   spinlockIsrNd_t lock = { LW_SPIN_INITIALIZER }

#ifdef __cplusplus
extern "C" {
#endif

/* NOTICE: SylixOS use INTREG type instead int type! */

void    spinLockIsrNdInit(spinlockIsrNd_t * spin);
INTREG  spinLockIsrNdTake(spinlockIsrNd_t * spin);
void    spinLockIsrNdGive(spinlockIsrNd_t * spin, INTREG key);

#ifdef __cplusplus
}
#endif

#endif /* __VXWORKS_SPINLOCKISRNDLIB_H */

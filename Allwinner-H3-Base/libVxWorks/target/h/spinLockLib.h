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

#ifndef __VXWORKS_SPINLOCKLIB_H
#define __VXWORKS_SPINLOCKLIB_H

#include "vxWorksCommon.h"

/* NOTICE: you can change SPIN_LOCK_USE_QUICK_LOCK to 1 and rebuild libVxWorks
 * make spin lock use SylixOS Quick spinlock */

#define SPIN_LOCK_USE_QUICK_LOCK   0

#define S_spinLockLib_NOT_SPIN_LOCK_CALLABLE    EEXIST
#define S_spinLockLib_INVALID_OPERATION         EINVAL
#define S_spinLockLib_LOCK_IN_RECOVERY          EINVAL
#define S_spinLockLib_LOCK_UNAVAILABLE          EINVAL
#define S_spinLockLib_LOCK_TIMEOUT              EBUSY

/* Return ERROR and set errno if a spinlock is held in the context of the
 * running CPU. */

#define SPIN_LOCK_RESTRICT()    spinLockRestrict()

/* Abstract the underline API to dynamically expand the appropriate code if
 * debugging is enabled for spinLock libraries.
 *
 * It is assumed that C code never invokes spinLockTaskTake () nor
 * spinLockIsrTake ()  directly. Use of the macros below ensures that
 * spinLockXXXTake will be called with the correct parameters. */

#define SPIN_LOCK_ISR_INIT(x, y)    (spinLockIsrInit(x, y))
#define SPIN_LOCK_ISR_TAKE(x)       (spinLockIsrTake(x))
#define SPIN_LOCK_ISR_GIVE(x)       (spinLockIsrGive(x))

#define SPIN_LOCK_TASK_INIT(x, y)   (spinLockTaskInit(x, y))
#define SPIN_LOCK_TASK_TAKE(x)      (spinLockTaskTake(x))
#define SPIN_LOCK_TASK_GIVE(x)      (spinLockTaskGive(x))

typedef struct spinlockIsr {
    spinlock_t  lock;
    int         flags;
} spinlockIsr_t;

typedef struct spinlockTask {
    spinlock_t  lock;
    int         flags;
} spinlockTask_t;

#ifdef __cplusplus
extern "C" {
#endif

/* check if a spinlock is held in the context of the running CPU. */

STATUS spinLockRestrict(void);

/* ISR callable spinLocks functions */

void spinLockIsrInit(spinlockIsr_t * lock, int flags);
void spinLockIsrGive(spinlockIsr_t * lock);
void spinLockIsrTake(spinlockIsr_t * lock);

/* task callable spinLock functions */

void spinLockTaskInit(spinlockTask_t * lock, int flags);
void spinLockTaskGive(spinlockTask_t * lock);
void spinLockTaskTake(spinlockTask_t * lock);

#ifdef __cplusplus
}
#endif

#endif /* __VXWORKS_SPINLOCKLIB_H */

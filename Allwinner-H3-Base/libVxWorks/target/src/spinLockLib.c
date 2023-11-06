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

#define  __SYLIXOS_KERNEL
#include <pthread.h>
#include "spinLockLib.h"
#include "spinlockIsrNdLib.h"

/*
 * check if a spinlock is held in the context of the running CPU.
 */
STATUS spinLockRestrict (void)
{
    if (API_SpinRestrict()) {
        errno = S_spinLockLib_NOT_SPIN_LOCK_CALLABLE;
        return  (ERROR);

    } else {
        return  (OK);
    }
}

/*
 * ISR callable spinLocks functions
 */
void spinLockIsrInit (spinlockIsr_t * lock, int flags)
{
    if (lock) {
        API_SpinInit(&lock->lock);
        lock->flags = flags;
    }
}

void spinLockIsrGive (spinlockIsr_t * lock)
{
    INTREG *key;

    if (lock) {
        key = (INTREG *)&(lock->lock.SL_pvReserved);
#if SPIN_LOCK_USE_QUICK_LOCK > 0
        API_SpinUnlockQuick(&lock->lock, *key);
#else
        API_SpinUnlockIrq(&lock->lock, *key);
#endif
    }
}

void spinLockIsrTake (spinlockIsr_t * lock)
{
    INTREG  intreg;

    if (lock) {
#if SPIN_LOCK_USE_QUICK_LOCK > 0
        API_SpinLockQuick(&lock->lock, &intreg);
#else
        API_SpinLockIrq(&lock->lock, &intreg);
#endif
        *(INTREG *)&(lock->lock.SL_pvReserved) = intreg;
    }
}

/*
 * task callable spinLock functions
 */
void spinLockTaskInit (spinlockTask_t * lock, int flags)
{
    if (lock) {
        pthread_spin_init(&lock->lock, 1);
        lock->flags = flags;
    }
}

void spinLockTaskGive (spinlockTask_t * lock)
{
    if (lock) {
        pthread_spin_unlock(&lock->lock);
    }
}

void spinLockTaskTake (spinlockTask_t * lock)
{
    if (lock) {
        pthread_spin_lock(&lock->lock);
    }
}

/*
 * Nd spinlock
 */
void spinLockIsrNdInit (spinlockIsrNd_t * spin)
{
    if (spin) {
        API_SpinInit(&spin->lock);
    }
}

INTREG spinLockIsrNdTake (spinlockIsrNd_t * spin)
{
    INTREG  key = -1;

    if (spin) {
#if SPIN_LOCK_ND_USE_QUICK_LOCK > 0
        API_SpinLockQuick(&spin->lock, &key);
#else
        API_SpinLockIrq(&spin->lock, &key);
#endif
    }

    return  (key);
}

void spinLockIsrNdGive (spinlockIsrNd_t * spin, INTREG key)
{
    if (spin) {
#if SPIN_LOCK_ND_USE_QUICK_LOCK > 0
        API_SpinUnlockQuick(&spin->lock, key);
#else
        API_SpinUnlockIrq(&spin->lock, key);
#endif
    }
}

/*
 * end
 */

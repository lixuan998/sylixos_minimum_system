/**
 * @file
 * interrupt library.
 *
 * VxWorks compatibility layer in SylixOS.
 *
 * Copyright (c) 2001-2018 SylixOS Group.
 * All rights reserved.
 *
 * Author: Han.hui <sylixos@gmail.com>
 */

#define  __SYLIXOS_KERNEL
#include "vxWorksCommon.h"

/*
 * lock interrupt Warning: SMP system only lock current cpu interrupt
 */
INTREG  intLock (void)
{
    return  (KN_INT_DISABLE());
}

/*
 * same as intLock()
 */
INTREG  intCpuLock (void)
{
    return  (KN_INT_DISABLE());
}

/*
 * unlock interrupt Warning: SMP system only unlock current cpu interrupt
 */
void  intUnlock (INTREG old)
{
    KN_INT_ENABLE(old);
}

/*
 * same as intUnlock()
 */
void  intCpuUnlock (INTREG key)
{
    KN_INT_ENABLE(key);
}

/*
 * end
 */


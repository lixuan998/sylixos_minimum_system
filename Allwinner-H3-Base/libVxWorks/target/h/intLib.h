/**
 * @file
 * interrupt library.
 *
 * VxWorks compatibility layer in SylixOS.
 *
 * Copyright (c) 2001-2014 SylixOS Group.
 * All rights reserved.
 *
 * Author: Han.hui <sylixos@gmail.com>
 */

#ifndef __VXWORKS_INTLIB_H
#define __VXWORKS_INTLIB_H

/* status codes */

#define S_intLib_NOT_ISR_CALLABLE           ERROR_KERNEL_IN_ISR
#define S_intLib_VEC_TABLE_WP_UNAVAILABLE   ERROR_KERNEL_VECTOR_NULL

#ifdef __cplusplus
extern "C" {
#endif

INTREG  intLock(void);
void    intUnlock(INTREG old);

INTREG  intCpuLock(void);
void    intCpuUnlock(INTREG key);

#ifdef __cplusplus
}
#endif

#endif /* __VXWORKS_INTLIB_H */

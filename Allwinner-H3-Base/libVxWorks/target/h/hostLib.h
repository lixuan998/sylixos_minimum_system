/**
 * @file
 * host table subroutine library.
 *
 * VxWorks compatibility layer in SylixOS.
 *
 * Copyright (c) 2001-2014 SylixOS Group.
 * All rights reserved.
 *
 * Author: Han.hui <sylixos@gmail.com>
 */

#ifndef __VXWORKS_HOSTLIB_H
#define __VXWORKS_HOSTLIB_H

#ifdef __cplusplus
extern "C" {
#endif

/* function declarations */
int hostGetByName(char *name);

#ifdef __cplusplus
}
#endif

#endif /* __VXWORKS_HOSTLIB_H */

/**
 * @file
 * network interface library.
 *
 * VxWorks compatibility layer in SylixOS.
 *
 * Copyright (c) 2001-2014 SylixOS Group.
 * All rights reserved.
 *
 * Author: Han.hui <sylixos@gmail.com>
 */

#ifndef __VXWORKS_IFLIB_H
#define __VXWORKS_IFLIB_H

#include "vxWorks.h"

#ifdef __cplusplus
extern "C" {
#endif

/* function declarations */
extern STATUS    ifAddrGet(char *interfaceName, char *interfaceAddress);
extern STATUS    ifMaskGet(char *interfaceName, int *netMask);

#ifdef __cplusplus
}
#endif

#endif /* __VXWORKS_IFLIB_H */

/**
 * @file
 * Internet address manipulation routines.
 *
 * VxWorks compatibility layer in SylixOS.
 *
 * Copyright (c) 2001-2014 SylixOS Group.
 * All rights reserved.
 *
 * Author: Han.hui <sylixos@gmail.com>
 */

#ifndef __VXWORKS_INETLIB_H
#define __VXWORKS_INETLIB_H

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int    Ip_u32;
typedef signed int      Ip_s32;

/* status codes */
#define S_inetLib_ILLEGAL_INTERNET_ADDRESS  EINVAL
#define S_inetLib_ILLEGAL_NETWORK_NUMBER    EINVAL

/* length of ASCII represention of inet addresses, eg. "90.0.0.0" */
#define INET_ADDR_LEN       18
#define INET6_ADDR_LEN      46
#define INETLIB             5 /* first available node */

/* function declarations */
extern void inet_ntoa_b(struct in_addr inetAddress, char *pString);

#ifdef __cplusplus
}
#endif

#endif /* __VXWORKS_INETLIB_H */

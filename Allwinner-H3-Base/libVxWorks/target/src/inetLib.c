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

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <netinet/in.h>
#include "vxWorks.h"
#include "errnoLib.h"
#include "inetLib.h"

/*
 * inet_ntoa_b - convert an network address to dot notation, store it in a buffer
 */
void inet_ntoa_b (struct in_addr  inetAddress,    /* inet address */
                  char            *pString)       /* where to return ASCII string */
{
    char *p = (char *)&inetAddress;

#define UC(b)   (((int)b)&0xff)

    if (pString != NULL) {
        sprintf(pString, "%d.%d.%d.%d", UC(p[0]), UC(p[1]), UC(p[2]), UC(p[3]));
    }
}

/*
 * end
 */

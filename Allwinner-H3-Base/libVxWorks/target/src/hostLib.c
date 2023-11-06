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

#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include "vxWorks.h"
#include "inetLib.h"
#include "hostLib.h"

/*
 * hostGetByName - look up a host in the host table by its name
 */
int hostGetByName (char  *name)
{
    int               hostAddr;
    struct hostent   *hptr;

    if (name == NULL) {
        printf("Error name.\n");
        return  (ERROR);
    }

    hptr = gethostbyname(name);
    if (hptr == NULL) {
        printf("gethostbyname error for host: %s: %s\n", name, strerror(errno));
        return  (ERROR);
    }

    hostAddr = *(int *)hptr->h_addr_list[0];
    return  (hostAddr);
}

/*
 * end
 */

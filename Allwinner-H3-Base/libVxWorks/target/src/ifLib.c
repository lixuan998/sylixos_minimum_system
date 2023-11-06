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

#include <net/if.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include "vxWorks.h"
#include "errnoLib.h"
#include "ifLib.h"
#include "inetLib.h"
#include "logLib.h"

/*
 * ifIoctlCall - make ioctl call to socket layer
 */
LOCAL STATUS ifIoctlCall (int            code,  /* ioctl code */
                          struct ifreq  *ifrp)  /* pointer to the interface ioctl request */
{
    int sock;
    int status;

    if ((sock = socket(AF_INET, SOCK_RAW, 0)) < 0) {
        return  (ERROR);
    }

    status = ioctl(sock, code, ifrp);
    close(sock);

    if (status != 0) {
        if (status != ERROR) {   /* iosIoctl() can return -1 */
            errnoSet(status);
        }
        return  (ERROR);
    }

    return  (OK);
}

/*
 * ifIoctlGet - retrieve information about the network interface
 */
LOCAL STATUS ifIoctlGet (char  *interfaceName,        /* name of the network interface, i.e. ei0 */
                         int    code,                 /* network interface ioctl function code */
                         int   *val)                  /* where to return result */
{
    struct ifreq  ifr;

    lib_bzero(&ifr, sizeof(ifr));

    lib_strncpy(ifr.ifr_name, interfaceName, (sizeof(ifr.ifr_name) - 1));
    ifr.ifr_name[IFNAMSIZ - 1] = 0;

    if (ifIoctlCall(code, &ifr) == ERROR) {
        return  (ERROR);
    }

    switch (code) {

    case SIOCGIFFLAGS:
        *val = ifr.ifr_flags;
        break;

    case SIOCGIFMETRIC:
        *val = ifr.ifr_metric;
        break;

    default:
        *val = (int)(((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr);
        break;
    }

    return  (OK);
}

/*
 * ifIoctl - network interface ioctl front-end
 */
LOCAL STATUS ifIoctl (char          *interfaceName,          /* name of the interface, i.e. ei0 */
                      int            code,                   /* ioctl function code */
                      _Vx_usr_arg_t  arg)                    /* some argument */
{
    int status;

    switch (code) {

    case SIOCGIFNETMASK:
    case SIOCGIFADDR:
        status = ifIoctlGet(interfaceName, code, (int *)arg);
        break;

    default:
        errnoSet(EOPNOTSUPP); /* not supported operation */
        status = ERROR;
        break;
    }

    return  (status);
}

/*
 * ifAddrParamGet - call ifIoctl to get the Internet address of the interface
 */
LOCAL STATUS ifAddrParamGet (char  *interfaceName,        /* name of interface to configure, i.e. ei0 */
                             int    code,                 /* SIOCG ioctl code */
                             char  *address)              /* address retrieved here */
{
    struct in_addr inetAddrBuf;    /* result */
    char netString [INET_ADDR_LEN];

    if (address == NULL) {
        return  (ERROR);
    }

    if (ifIoctl(interfaceName, code, (_Vx_usr_arg_t)&inetAddrBuf) == OK) {
        inet_ntoa_b(inetAddrBuf, netString);
        lib_strncpy(address, netString, INET_ADDR_LEN);
        return  (OK);
    }

    return  (ERROR);
}

/*
 * ifAddrGet - get the Internet address of a network interface
 */
STATUS ifAddrGet (char  *interfaceName,
                  char  *interfaceAddress)
{
    return  (ifAddrParamGet(interfaceName, (int)SIOCGIFADDR, interfaceAddress));
}

/*
 * ifMaskGet - get the subnet mask for a network interface
 */
STATUS ifMaskGet (char  *interfaceName,  /* name of interface to configure, i.e. ei0 */
                  int   *netMask)        /* buffer for subnet mask */
{
    int status = ifIoctl(interfaceName, (int)SIOCGIFNETMASK, (_Vx_usr_arg_t)netMask);

    if (status != ERROR) {
        *netMask = (int)(ntohl(*netMask));
        return  (OK);
    }

    return  (ERROR);
}

/*
 * end
 */

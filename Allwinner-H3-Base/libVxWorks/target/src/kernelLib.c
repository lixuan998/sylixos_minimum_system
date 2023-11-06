/**
 * @file
 * kernel login.
 *
 * VxWorks compatibility layer in SylixOS.
 *
 * Copyright (c) 2001-2014 SylixOS Group.
 * All rights reserved.
 *
 * Author: Han.hui <sylixos@gmail.com>
 */

#define  __SYLIXOS_KERNEL
#include "vxWorksCommon.h"

/*
 * return the WIND kernel revision string
 */
char  *kernelVersion (void)
{
    return  ("WIND version 2.13");
}

/*
 * enable round-robin selection
 */
STATUS  kernelTimeSlice (int ticks)
{
    UINT16  usSlice = (UINT16)ticks;

    API_ThreadSetSlice(API_ThreadIdSelf(), usSlice);

    return  (OK);
}

/*
 * get the time slice ticks
 */
STATUS  kernelTimeSliceGet (ULONG *pTimeSlice)
{
    UINT16  usSlice;

    if (pTimeSlice) {
        API_ThreadGetSlice(API_ThreadIdSelf(), &usSlice);
        *pTimeSlice = (ULONG)usSlice;
        return  (OK);
    }

    return  (ERROR);
}

/*
 * install VxWorks Round Robin implementation
 */
STATUS  kernelRoundRobinInstall (void)
{
    return  (OK);
}

/*
 * non-bootstrap processor entry point into Wind kernel
 */
STATUS  kernelCpuEnable (unsigned int cpuToEnable)
{
    return  (OK);
}

/*
 * determine whether the specified CPU is idle
 */
BOOL  kernelIsCpuIdle (unsigned int cpu)
{
    return  (API_KernelIsCpuIdle((ULONG)cpu));
}

/*
 * determine whether all enabled processors are idle
 */
BOOL  kernelIsSystemIdle (void)
{
    return  (API_KernelIsSystemIdle());
}

/*
 * end
 */

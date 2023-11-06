/**
 * @file
 * CPU library header.
 *
 * VxWorks compatibility layer in SylixOS.
 *
 * Copyright (c) 2001-2014 SylixOS Group.
 * All rights reserved.
 *
 * Author: Han.hui <sylixos@gmail.com>
 */

#ifndef __VXWORKS_VXCPULIB_H
#define __VXWORKS_VXCPULIB_H

#include "vxWorksCommon.h"
#include <sched.h>

/*
 * CPU set
 */
typedef cpu_set_t    cpuset_t;

#ifdef __cplusplus
extern "C" {
#endif

/* function declarations */

cpuid_t      vxCpuIdGet(void);
unsigned int vxCpuIndexGet(void);
cpuset_t     vxCpuEnabledGet(void);
unsigned int vxCpuConfiguredGet(void);

#if (LW_CFG_SMP_EN > 0) && (LW_CFG_POSIX_EN > 0) && (LW_CFG_POSIXEX_EN > 0)
STATUS vxCpuReservedGet(cpuset_t *set);
STATUS vxCpuReserve(cpuset_t set, cpuset_t *set_r);
STATUS vxCpuUnreserve(cpuset_t set);
#endif /* (LW_CFG_SMP_EN > 0) && (LW_CFG_POSIX_EN > 0) && (LW_CFG_POSIXEX_EN > 0) */

#ifdef __cplusplus
}
#endif

#endif /* __VXWORKS_VXCPULIB_H */

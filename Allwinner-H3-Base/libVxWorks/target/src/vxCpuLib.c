/**
 * @file
 * CPU library.
 *
 * VxWorks compatibility layer in SylixOS.
 *
 * Copyright (c) 2001-2014 SylixOS Group.
 * All rights reserved.
 *
 * Author: Han.hui <sylixos@gmail.com>
 */

#define  __SYLIXOS_KERNEL
#include <unistd.h>
#include "vxCpuLib.h"

/*
 * get the hardware identification of the calling CPU
 */
cpuid_t vxCpuIdGet (void)
{
    return  ((unsigned int)API_CpuCurId());
}

/*
 * get current CPU index.
 */
unsigned int vxCpuIndexGet (void)
{
    return  ((unsigned int)API_CpuCurId());
}

/*
 * get enable CPU index.
 */
cpuset_t vxCpuEnabledGet (void)
{
    static cpuset_t set;

#if LW_CFG_SMP_EN > 0
    int i;
    int max = (int)sysconf(_SC_NPROCESSORS_CONF);
#endif

    CPU_ZERO(&set);

#if LW_CFG_SMP_EN > 0
    for (i = 0; i < max; i++) {
        if (API_CpuIsUp(i)) {
            CPU_SET(i, &set);
        }
    }
#else
    CPU_SET(0, &set);
#endif

    return  (set);
}

/*
 * get CPU count.
 */
unsigned int vxCpuConfiguredGet (void)
{
    return  ((unsigned int)sysconf(_SC_NPROCESSORS_CONF));
}

#if (LW_CFG_SMP_EN > 0) && (LW_CFG_POSIX_EN > 0) && (LW_CFG_POSIXEX_EN > 0)
/*
 * Obtain the set of reserved CPUs
 */
STATUS vxCpuReservedGet (cpuset_t *set)
{
    return  (sched_cpuaffinity_get_np(sizeof(cpuset_t), set));
}

/*
 * Reserve one or more CPUs
 */
STATUS vxCpuReserve (cpuset_t set, cpuset_t *set_r)
{
    if (set_r) {
        sched_cpuaffinity_get_np(sizeof(cpuset_t), set_r);
    }

    return  (sched_cpuaffinity_enable_np(sizeof(cpuset_t), &set));
}

/*
 * Unreserve one or more CPUs
 */
STATUS vxCpuUnreserve (cpuset_t set)
{
    return  (sched_cpuaffinity_disable_np(sizeof(cpuset_t), &set));
}

#endif /* (LW_CFG_SMP_EN > 0) && (LW_CFG_POSIX_EN > 0) && (LW_CFG_POSIXEX_EN > 0) */
/*
 * end
 */

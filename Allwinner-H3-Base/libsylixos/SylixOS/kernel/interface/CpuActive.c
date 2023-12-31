/*********************************************************************************************************
**
**                                    中国软件开源组织
**
**                                   嵌入式实时操作系统
**
**                                SylixOS(TM)  LW : long wing
**
**                               Copyright All Rights Reserved
**
**--------------文件信息--------------------------------------------------------------------------------
**
** 文   件   名: CpuActive.c
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2014 年 07 月 21 日
**
** 描        述: SMP 系统开启/关闭一个 CPU.
**
** 注        意: 对 CPU 的启动和停止不可重入.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "../SylixOS/kernel/include/k_kernel.h"
/*********************************************************************************************************
  裁剪控制
*********************************************************************************************************/
#if LW_CFG_SMP_EN > 0
/*********************************************************************************************************
** 函数名称: API_CpuUp
** 功能描述: 开启一个 CPU. (非 0 号 CPU)
** 输　入  : ulCPUId       CPU ID
** 输　出  : ERROR
** 全局变量: 
** 调用模块: 
                                           API 函数
*********************************************************************************************************/
LW_API  
ULONG  API_CpuUp (ULONG  ulCPUId)
{
    INTREG          iregInterLevel;
    PLW_CLASS_CPU   pcpu;

    if ((ulCPUId == 0) || (ulCPUId >= LW_NCPUS)) {
        _ErrorHandle(EINVAL);
        return  (EINVAL);
    }
    
    KN_SMP_MB();
    pcpu = LW_CPU_GET(ulCPUId);
    if (LW_CPU_IS_ACTIVE(pcpu)
#if LW_CFG_SMP_CPU_DOWN_EN > 0
        || (LW_CPU_GET_IPI_PEND2(pcpu) & LW_IPI_DOWN_MSK)
#endif                                                                  /*  LW_CFG_SMP_CPU_DOWN_EN > 0  */
        ) {
        return  (ERROR_NONE);
    }
    
    iregInterLevel = KN_INT_DISABLE();
    bspCpuUp(ulCPUId);
    KN_INT_ENABLE(iregInterLevel);
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: API_CpuDown
** 功能描述: 关闭一个 CPU. (非 0 号 CPU, 调用后并不是马上关闭, 需要延迟使用 API_CpuIsUp 探测)
** 输　入  : ulCPUId       CPU ID
** 输　出  : ERROR
** 全局变量: 
** 调用模块: 
                                           API 函数
*********************************************************************************************************/
#if LW_CFG_SMP_CPU_DOWN_EN > 0

LW_API  
ULONG  API_CpuDown (ULONG  ulCPUId)
{
#if LW_CFG_CPU_ATOMIC_EN == 0
    INTREG          iregInterLevel;
#endif
    
    PLW_CLASS_CPU   pcpu;

    if ((ulCPUId == 0) || (ulCPUId >= LW_NCPUS)) {
        _ErrorHandle(EINVAL);
        return  (EINVAL);
    }
    
    __KERNEL_ENTER();
    pcpu = LW_CPU_GET(ulCPUId);
    if (!LW_CPU_IS_ACTIVE(pcpu) || 
        (LW_CPU_GET_IPI_PEND2(pcpu) & LW_IPI_DOWN_MSK)) {
        __KERNEL_EXIT();
        return  (ERROR_NONE);
    }
    
#if LW_CFG_CPU_ATOMIC_EN == 0
    LW_SPIN_LOCK_QUICK(&pcpu->CPU_slIpi, &iregInterLevel);
#endif
    
    LW_CPU_ADD_IPI_PEND2(pcpu, LW_IPI_DOWN_MSK);
    
#if LW_CFG_CPU_ATOMIC_EN == 0
    LW_SPIN_UNLOCK_QUICK(&pcpu->CPU_slIpi, iregInterLevel);
#endif
    
    _ThreadOffAffinity(pcpu);                                           /*  关闭与此 CPU 有关的亲和度   */
    __KERNEL_EXIT();
    
    _SmpSendIpi(ulCPUId, LW_IPI_DOWN, 0, LW_FALSE);                     /*  使用核间中断通知 CPU 停止   */
    
    return  (ERROR_NONE);
}

#endif                                                                  /*  LW_CFG_SMP_CPU_DOWN_EN > 0  */
/*********************************************************************************************************
** 函数名称: API_CpuIsUp
** 功能描述: 指定 CPU 是否启动.
** 输　入  : ulCPUId       CPU ID
** 输　出  : 是否启动
** 全局变量: 
** 调用模块: 
                                           API 函数
*********************************************************************************************************/
LW_API  
BOOL  API_CpuIsUp (ULONG  ulCPUId)
{
    PLW_CLASS_CPU   pcpu;
    
    if (ulCPUId >= LW_NCPUS) {
        _ErrorHandle(EINVAL);
        return  (LW_FALSE);
    }
    
    KN_SMP_MB();
    pcpu = LW_CPU_GET(ulCPUId);
    if (LW_CPU_IS_ACTIVE(pcpu)) {
        return  (LW_TRUE);
    
    } else {
        return  (LW_FALSE);
    }
}
/*********************************************************************************************************
** 函数名称: API_CpuIsRunning
** 功能描述: 指定 CPU 是否在运行.
** 输　入  : ulCPUId       CPU ID
** 输　出  : 是否在运行
** 全局变量: 
** 调用模块: 
                                           API 函数
*********************************************************************************************************/
LW_API  
BOOL  API_CpuIsRunning (ULONG  ulCPUId)
{
    PLW_CLASS_CPU   pcpu;
    
    if (ulCPUId >= LW_NCPUS) {
        _ErrorHandle(EINVAL);
        return  (LW_FALSE);
    }
    
    KN_SMP_MB();
    pcpu = LW_CPU_GET(ulCPUId);
    if (LW_CPU_IS_RUNNING(pcpu)) {
        return  (LW_TRUE);
    
    } else {
        return  (LW_FALSE);
    }
}

#endif                                                                  /*  LW_CFG_SMP_EN > 0           */
/*********************************************************************************************************
** 函数名称: API_CpuCurId
** 功能描述: 获得指定 CPU 物理 CPU ID.
** 输　入  : NONE
** 输　出  : CPU ID
** 全局变量: 
** 调用模块: 
                                           API 函数
*********************************************************************************************************/
LW_API 
ULONG  API_CpuCurId (VOID)
{
#if LW_CFG_SMP_EN > 0
    INTREG  iregInterLevel;
    ULONG   ulCPUId;
    
    iregInterLevel = KN_INT_DISABLE();
    ulCPUId = LW_CPU_GET_CUR_ID();
    KN_INT_ENABLE(iregInterLevel);
    
    return  (ulCPUId);
#else                                                                   /*  LW_CFG_SMP_EN > 0           */
    return  (0ul);
#endif
}
/*********************************************************************************************************
** 函数名称: API_CpuPhyId
** 功能描述: 获得指定 CPU 物理 CPU ID.
** 输　入  : ulCPUId       CPU ID
**           pulPhyId      物理 CPU ID
** 输　出  : ERROR
** 全局变量: 
** 调用模块: 
                                           API 函数
*********************************************************************************************************/
LW_API 
ULONG  API_CpuPhyId (ULONG  ulCPUId, ULONG  *pulPhyId)
{
    if ((ulCPUId >= LW_NCPUS) || !pulPhyId) {
        _ErrorHandle(EINVAL);
        return  (EINVAL);
    }
    
#if (LW_CFG_SMP_EN > 0) && (LW_CFG_CPU_ARCH_SMT > 0)
    *pulPhyId = LW_CPU_GET(ulCPUId)->CPU_ulPhyId;
#else
    *pulPhyId = ulCPUId;
#endif
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: API_CpuNum
** 功能描述: CPU 个数
** 输　入  : NONE
** 输　出  : CPU 个数
** 全局变量: 
** 调用模块: 
                                           API 函数
*********************************************************************************************************/
LW_API 
ULONG  API_CpuNum (VOID)
{
#if LW_CFG_SMP_EN > 0
    return  (LW_NCPUS);
#else
    return  (1ul);
#endif                                                                  /*  LW_CFG_SMP_EN > 0           */
}
/*********************************************************************************************************
** 函数名称: API_CpuUpNum
** 功能描述: 已经启动的 CPU 个数
** 输　入  : NONE
** 输　出  : 已经启动的 CPU 个数
** 全局变量: 
** 调用模块: 
                                           API 函数
*********************************************************************************************************/
LW_API 
ULONG  API_CpuUpNum (VOID)
{
#if LW_CFG_SMP_EN > 0
    INT     i;
    ULONG   ulCnt = 0;
    
    LW_CPU_FOREACH_ACTIVE (i) {
        ulCnt++;
    }
    
    return  (ulCnt);
#else
    return  (1ul);
#endif                                                                  /*  LW_CFG_SMP_EN > 0           */
}
/*********************************************************************************************************
  END
*********************************************************************************************************/

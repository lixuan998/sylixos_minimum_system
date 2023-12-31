/*********************************************************************************************************
**
**                                    中国软件开源组织
**
**                                   嵌入式实时操作系统
**
**                                       SylixOS(TM)
**
**                               Copyright  All Rights Reserved
**
**--------------文件信息--------------------------------------------------------------------------------
**
** 文   件   名: cskySpinlock.c
**
** 创   建   人: Wang.Xuan (王翾)
**
** 文件创建日期: 2018 年 05 月 12 日
**
** 描        述: C-SKY 体系架构自旋锁驱动.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "SylixOS.h"
/*********************************************************************************************************
  spinlock 状态
*********************************************************************************************************/
#if LW_CFG_SMP_EN > 0
/*********************************************************************************************************
  spinlock bug trace
*********************************************************************************************************/
#ifdef __LW_SPINLOCK_BUG_TRACE_EN
#define __LW_SPINLOCK_RECURSIVE_TRACE() \
        _BugFormat((psl->SL_ulCounter > 10), LW_TRUE, \
                   "spinlock RECURSIVE %lu!\r\n", psl->SL_ulCounter)
#else
#define __LW_SPINLOCK_RECURSIVE_TRACE()
#endif
/*********************************************************************************************************
** 函数名称: cskySpinLock
** 功能描述: C-SKY spin lock
** 输　入  : psld       spinlock data 指针
**           pfuncPoll  循环等待时调用函数
**           pvArg      回调参数
** 输　出  : NONE
** 全局变量:
** 调用模块:
** 注  意  : 自旋结束时, 操作系统会调用内存屏障, 所以这里不需要调用.
*********************************************************************************************************/
static LW_INLINE VOID  cskySpinLock (SPINLOCKTYPE *psld, VOIDFUNCPTR  pfuncPoll, PVOID  pvArg)
{
#if LW_CFG_CSKY_HAS_LDSTEX_INSTR > 0
    SPINLOCKTYPE    sldVal;
    UINT32          uiInc = 1 << LW_SPINLOCK_TICKET_SHIFT;
    UINT32          uiTemp;

    __asm__ __volatile__ (
        "1: ldex.w      %0 , (%2)   \n"
        "   mov         %1 , %0     \n"
        "   add         %0 , %3     \n"
        "   stex.w      %0 , (%2)   \n"
        "   bez         %0 , 1b     \n"
        : "=&r" (uiTemp), "=&r" (sldVal)
        : "r"(&psld->SLD_uiLock), "r"(uiInc)
        : "cc");

    while (sldVal.SLD_usTicket != sldVal.SLD_usSvcNow) {
        if (pfuncPoll) {
            pfuncPoll(pvArg);
        }
        sldVal.SLD_usSvcNow = LW_ACCESS_ONCE(UINT16, psld->SLD_usSvcNow);
    }
#endif                                                                  /*  LW_CFG_CSKY_HAS_LDSTEX_INSTR*/
}
/*********************************************************************************************************
** 函数名称: cskySpinTryLock
** 功能描述: C-SKY spin trylock
** 输　入  : psld       spinlock data 指针
** 输　出  : 1: busy 0: ok
** 全局变量:
** 调用模块:
** 注  意  : 自旋结束时, 操作系统会调用内存屏障, 所以这里不需要调用.
*********************************************************************************************************/
static LW_INLINE UINT32  cskySpinTryLock (SPINLOCKTYPE *psld)
{
#if LW_CFG_CSKY_HAS_LDSTEX_INSTR > 0
    UINT32  uiCont, uiRes, uiTemp;
    UINT32  uiInc = 1 << LW_SPINLOCK_TICKET_SHIFT;

    do {
        __asm__ __volatile__ (
        "   ldex.w      %0 , (%3)       \n"
        "   movi        %2 , 1          \n"
        "   rotli       %1 , %0 , 16    \n"
        "   cmpne       %1 , %0         \n"
        "   bt          1f              \n"
        "   movi        %2 , 0          \n"
        "   add         %0 , %0 , %4    \n"
        "   stex.w      %0 , (%3)       \n"
        "1:                             \n"
        : "=&r" (uiRes), "=&r" (uiTemp), "=&r" (uiCont)
        : "r"(&psld->SLD_uiLock), "r"(uiInc)
        : "cc");
    } while (!uiRes);

    if (uiCont) {
        return  (1);
    }
#endif                                                                  /*  LW_CFG_CSKY_HAS_LDSTEX_INSTR*/

    return  (0);
}
/*********************************************************************************************************
** 函数名称: cskySpinUnlock
** 功能描述: C-SKY spin unlock
** 输　入  : psld       spinlock data 指针
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static LW_INLINE VOID  cskySpinUnlock (SPINLOCKTYPE *psld)
{
#if LW_CFG_CSKY_HAS_LDSTEX_INSTR > 0
    psld->SLD_usSvcNow++;
    KN_SMP_WMB();
#endif                                                                  /*  LW_CFG_CSKY_HAS_LDSTEX_INSTR*/
}
/*********************************************************************************************************
** 函数名称: cskySpinLockDummy
** 功能描述: 空操作
** 输　入  : psl       spinlock 指针
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static VOID  cskySpinLockDummy (SPINLOCKTYPE  *psl, VOIDFUNCPTR  pfuncPoll, PVOID  pvArg)
{
}
/*********************************************************************************************************
** 函数名称: cskySpinTryLockDummy
** 功能描述: 空操作
** 输　入  : psl       spinlock 指针
** 输　出  : 0
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static UINT32  cskySpinTryLockDummy (SPINLOCKTYPE  *psl)
{
    return  (0);
}
/*********************************************************************************************************
** 函数名称: cskySpinUnlockDummy
** 功能描述: 空操作
** 输　入  : psl       spinlock 指针
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static VOID  cskySpinUnlockDummy (SPINLOCKTYPE  *psl)
{
}
/*********************************************************************************************************
  spin lock cache 依赖处理
*********************************************************************************************************/
static VOID    (*pfuncCskySpinLock)(SPINLOCKTYPE *, VOIDFUNCPTR, PVOID) = cskySpinLock;
static UINT32  (*pfuncCskySpinTryLock)(SPINLOCKTYPE *)                  = cskySpinTryLock;
static VOID    (*pfuncCskySpinUnlock)(SPINLOCKTYPE *)                   = cskySpinUnlock;
/*********************************************************************************************************
** 函数名称: archSpinBypass
** 功能描述: spinlock 函数不起效
** 输　入  : NONE
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID  archSpinBypass (VOID)
{
    pfuncCskySpinLock    = cskySpinLockDummy;
    pfuncCskySpinTryLock = cskySpinTryLockDummy;
    pfuncCskySpinUnlock  = cskySpinUnlockDummy;
}
/*********************************************************************************************************
** 函数名称: archSpinWork
** 功能描述: spinlock 函数起效
** 输　入  : NONE
** 输　出  : NONE
** 全局变量:
** 调用模块:
** 注  意  : 主核开启 CACHE 后, BSP 应立即调用此函数, 使 spinlock 生效,
             从核启动到开启 CACHE 过程中, 不得操作 spinlock.
*********************************************************************************************************/
VOID  archSpinWork (VOID)
{
    pfuncCskySpinLock    = cskySpinLock;
    pfuncCskySpinTryLock = cskySpinTryLock;
    pfuncCskySpinUnlock  = cskySpinUnlock;
}
/*********************************************************************************************************
** 函数名称: archSpinInit
** 功能描述: 初始化一个 spinlock
** 输　入  : psl       spinlock 指针
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID  archSpinInit (spinlock_t  *psl)
{
    psl->SL_sltData.SLD_uiLock = 0;                                     /*  0: 未锁定状态  1: 锁定状态  */
    psl->SL_pcpuOwner          = LW_NULL;
    psl->SL_ulCounter          = 0;
    psl->SL_pvReserved         = LW_NULL;
    KN_SMP_WMB();
}
/*********************************************************************************************************
** 函数名称: archSpinDelay
** 功能描述: 等待事件
** 输　入  : NONE
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID  archSpinDelay (VOID)
{
    volatile INT  i;

    for (i = 0; i < 3; i++) {
    }
}
/*********************************************************************************************************
** 函数名称: archSpinNotify
** 功能描述: 发送 spin 事件
** 输　入  : NONE
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID  archSpinNotify (VOID)
{
}
/*********************************************************************************************************
** 函数名称: archSpinLock
** 功能描述: spinlock 上锁
** 输　入  : psl        spinlock 指针
**           pcpuCur    当前 CPU
**           pfuncPoll  循环等待时调用函数
**           pvArg      回调参数
** 输　出  : 0: 没有获取
**           1: 正常加锁
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  archSpinLock (spinlock_t  *psl, PLW_CLASS_CPU  pcpuCur, VOIDFUNCPTR  pfuncPoll, PVOID  pvArg)
{
    if (psl->SL_pcpuOwner == pcpuCur) {
        psl->SL_ulCounter++;
        __LW_SPINLOCK_RECURSIVE_TRACE();
        return  (1);                                                    /*  重复调用                    */
    }

    pfuncCskySpinLock(&psl->SL_sltData, pfuncPoll, pvArg);

    psl->SL_pcpuOwner = pcpuCur;                                        /*  保存当前 CPU                */

    return  (1);                                                        /*  加锁成功                    */
}
/*********************************************************************************************************
** 函数名称: archSpinTryLock
** 功能描述: spinlock 试图上锁
** 输　入  : psl        spinlock 指针
**           pcpuCur    当前 CPU
** 输　出  : 0: 没有获取
**           1: 正常加锁
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  archSpinTryLock (spinlock_t  *psl, PLW_CLASS_CPU  pcpuCur)
{
    if (psl->SL_pcpuOwner == pcpuCur) {
        psl->SL_ulCounter++;
        __LW_SPINLOCK_RECURSIVE_TRACE();
        return  (1);                                                    /*  重复调用                    */
    }

    if (pfuncCskySpinTryLock(&psl->SL_sltData)) {                       /*  尝试加锁                    */
        return  (0);
    }

    psl->SL_pcpuOwner = pcpuCur;                                        /*  保存当前 CPU                */

    return  (1);
}
/*********************************************************************************************************
** 函数名称: archSpinUnlock
** 功能描述: spinlock 解锁
** 输　入  : psl        spinlock 指针
**           pcpuCur    当前 CPU
** 输　出  : 0: 没有获取
**           1: 正常解锁
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  archSpinUnlock (spinlock_t  *psl, PLW_CLASS_CPU  pcpuCur)
{
    if (psl->SL_pcpuOwner != pcpuCur) {
        return  (0);                                                    /*  没有权利释放                */
    }

    if (psl->SL_ulCounter) {
        psl->SL_ulCounter--;                                            /*  减少重复调用次数            */
        return  (1);
    }

    psl->SL_pcpuOwner = LW_NULL;                                        /*  没有 CPU 获取               */
    KN_SMP_WMB();

    pfuncCskySpinUnlock(&psl->SL_sltData);                              /*  解锁                        */

    return  (1);
}
/*********************************************************************************************************
** 函数名称: archSpinLockRaw
** 功能描述: spinlock 上锁 (不进行重入判断)
** 输　入  : psl        spinlock 指针
** 输　出  : 0: 没有获取
**           1: 正常加锁
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  archSpinLockRaw (spinlock_t  *psl)
{
    pfuncCskySpinLock(&psl->SL_sltData, LW_NULL, LW_NULL);

    return  (1);                                                        /*  加锁成功                    */
}
/*********************************************************************************************************
** 函数名称: archSpinTryLockRaw
** 功能描述: spinlock 试图上锁 (不进行重入判断)
** 输　入  : psl        spinlock 指针
** 输　出  : 0: 没有获取
**           1: 正常加锁
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  archSpinTryLockRaw (spinlock_t  *psl)
{
    if (pfuncCskySpinTryLock(&psl->SL_sltData)) {                       /*  尝试加锁                    */
        return  (0);
    }

    return  (1);                                                        /*  加锁成功                    */
}
/*********************************************************************************************************
** 函数名称: archSpinUnlockRaw
** 功能描述: spinlock 解锁
** 输　入  : psl        spinlock 指针
** 输　出  : 0: 没有获取
**           1: 正常解锁
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  archSpinUnlockRaw (spinlock_t  *psl)
{
    pfuncCskySpinUnlock(&psl->SL_sltData);                              /*  解锁                        */

    return  (1);
}

#endif                                                                  /*  LW_CFG_SMP_EN               */
/*********************************************************************************************************
  END
*********************************************************************************************************/

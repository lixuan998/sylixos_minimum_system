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
** 文   件   名: csky_support.h
**
** 创   建   人: Wang.Xuan (王翾)
**
** 文件创建日期: 2018 年 05 月 10 日
**
** 描        述: C-SKY 体系架构支持.
*********************************************************************************************************/

#ifndef __ARCH_CSKY_SUPPORT_H
#define __ARCH_CSKY_SUPPORT_H

#define __LW_SCHEDULER_BUG_TRACE_EN                                     /*  测试多核调度器              */
#define __LW_KERNLOCK_BUG_TRACE_EN                                      /*  测试内核锁                  */

/*********************************************************************************************************
  汇编相关头文件
*********************************************************************************************************/

#include "arch/assembler.h"

/*********************************************************************************************************
  存储器定义 (CPU 栈区操作)
*********************************************************************************************************/

#define CPU_STK_GROWTH              1                                   /*  1：入栈从高地址向低地址     */
                                                                        /*  0：入栈从低地址向高地址     */
/*********************************************************************************************************
  arch 已经提供的接口如下:
*********************************************************************************************************/

#define __ARCH_KERNEL_PARAM         archKernelParam
#define __ARCH_MEMCPY               archMemcpy
#define __ARCH_MEMMOVE              archMemmove
#define __ARCH_MEMSET               archMemset

VOID    archKernelParam(CPCHAR  pcParam);
PVOID   archMemcpy(PVOID   pvDest, CPVOID  pvSrc, size_t  stCount);
PVOID   archMemmove(PVOID  pvDest, CPVOID  pvSrc, size_t  stCount);
PVOID   archMemset(PVOID   pvDest, INT  iC, size_t  stCount);

/*********************************************************************************************************
  C-SKY 处理器断言
*********************************************************************************************************/

VOID    archAssert(CPCHAR  pcCond, CPCHAR  pcFunc, CPCHAR  pcFile, INT  iLine);

/*********************************************************************************************************
  C-SKY 处理器线程上下文相关接口
*********************************************************************************************************/

PLW_STACK       archTaskCtxCreate(ARCH_REG_CTX          *pregctx,
                                  PTHREAD_START_ROUTINE  pfuncTask,
                                  PVOID                  pvArg,
                                  PLW_CLASS_TCB          ptcb,
                                  PLW_STACK              pstkTop,
                                  ULONG                  ulOpt);
VOID            archTaskCtxSetFp(PLW_STACK               pstkDest,
                                 ARCH_REG_CTX           *pregctxDest,
                                 const ARCH_REG_CTX     *pregctxSrc);
ARCH_REG_CTX   *archTaskRegsGet(ARCH_REG_CTX  *pregctx, ARCH_REG_T  *pregSp);
VOID            archTaskRegsSet(ARCH_REG_CTX  *pregctxDest, const ARCH_REG_CTX  *pregctxSrc);

#if LW_CFG_DEVICE_EN > 0
VOID        archTaskCtxShow(INT  iFd, const ARCH_REG_CTX  *pregctx);
#endif                                                                  /*  LW_CFG_DEVICE_EN > 0        */
VOID        archTaskCtxPrint(PVOID  pvBuffer, size_t  stSize, const ARCH_REG_CTX  *pregctx);

VOID        archTaskCtxStart(PLW_CLASS_CPU  pcpuSw);
VOID        archTaskCtxSwitch(PLW_CLASS_CPU  pcpuSw);

VOID        archTaskCtxCopy(ARCH_REG_CTX  *pregctxDest, const ARCH_REG_CTX  *pregctxSrc);

#if LW_CFG_COROUTINE_EN > 0
VOID        archCrtCtxSwitch(PLW_CLASS_CPU  pcpuSw);
#endif                                                                  /*  LW_CFG_COROUTINE_EN > 0     */

VOID        archIntCtxLoad(PLW_CLASS_CPU  pcpuSw);
VOID        archSigCtxLoad(const ARCH_REG_CTX  *pregctx);

VOID        archIntCtxSaveReg(PLW_CLASS_CPU  pcpu,
                              ARCH_REG_T     reg0,
                              ARCH_REG_T     reg1,
                              ARCH_REG_T     reg2,
                              ARCH_REG_T     reg3);

PLW_STACK   archCtxStackEnd(const ARCH_REG_CTX  *pregctx);

/*********************************************************************************************************
  C-SKY 处理器调试接口
*********************************************************************************************************/

#if LW_CFG_GDB_EN > 0
VOID    archDbgBpInsert(addr_t   ulAddr, size_t stSize, ULONG  *pulIns, BOOL  bLocal);
VOID    archDbgAbInsert(addr_t   ulAddr, ULONG  *pulIns);
VOID    archDbgBpRemove(addr_t   ulAddr, size_t stSize, ULONG   ulIns, BOOL  bLocal);
VOID    archDbgApRemove(addr_t   ulAddr, ULONG   ulIns);
VOID    archDbgBpPrefetch(addr_t ulAddr);
UINT    archDbgTrapType(addr_t   ulAddr, PVOID   pvArch);
VOID    archDbgBpAdjust(PVOID  pvDtrace, PVOID   pvtm);
BOOL    archDbgIsStep(pid_t  pid, addr_t  ulAddr);
#endif                                                                  /*  LW_CFG_GDB_EN > 0           */

/*********************************************************************************************************
  C-SKY 处理器异常接口
*********************************************************************************************************/

VOID    archIntHandle(ULONG  ulVector, BOOL  bPreemptive);              /*  bspIntHandle 需要调用此函数 */

/*********************************************************************************************************
  C-SKY 通用库
*********************************************************************************************************/

INT     archFindLsb(UINT32 ui32);
INT     archFindMsb(UINT32 ui32);
INTREG  archGetPSR(VOID);

/*********************************************************************************************************
  C-SKY 处理器标准底层库
*********************************************************************************************************/

#define KN_INT_DISABLE()            archIntDisable()
#define KN_INT_ENABLE(intLevel)     archIntEnable(intLevel)
#define KN_INT_ENABLE_FORCE()       archIntEnableForce()

INTREG  archIntDisable(VOID);
VOID    archIntEnable(INTREG  iregInterLevel);
VOID    archIntEnableForce(VOID);

#define KN_FIQ_DISABLE()            archFastIntDisable()
#define KN_FIQ_ENABLE()             archFastIntEnable()
#define KN_FIQ_AUTO_ENTRY_SET()     archFastAutoIntEntrySet()
#define KN_FIQ_VECTOR_CLAIM(vector) archFastVectorIntClaim(vector)

VOID    archFastIntDisable(VOID);
VOID    archFastIntEnable(VOID);

VOID    archFastAutoIntEntry(VOID);
VOID    archFastAutoIntRawEntry(VOID);
VOID    archFastAutoIntEntrySet(VOID);

VOID    archFastVectorIntEntry(VOID);
VOID    archFastVectorIntRawEntry(VOID);
INT     archFastVectorIntClaim(ULONG  ulVector);

VOID    archPageCopy(PVOID pvTo, PVOID pvFrom);

#define KN_COPY_PAGE(to, from)      archPageCopy(to, from)

VOID    archReboot(INT  iRebootType, addr_t  ulStartAddress);

/*********************************************************************************************************
  C-SKY 处理器 BogoMIPS 循环
*********************************************************************************************************/

VOID    archBogoMipsLoop(ULONG  ulLoop);

#define __ARCH_BOGOMIPS_LOOP            archBogoMipsLoop
#define __ARCH_BOGOMIPS_INS_PER_LOOP    8

/*********************************************************************************************************
  C-SKY 处理器 基本功能
*********************************************************************************************************/

VOID    cskyWaitForInterrupt(VOID);

/*********************************************************************************************************
  C-SKY 处理器 CACHE 操作
*********************************************************************************************************/

#define CSKY_MACHINE_AUTO   "auto"
#define CSKY_MACHINE_510    "ck510"
#define CSKY_MACHINE_610    "ck610"
#define CSKY_MACHINE_801    "ck801"
#define CSKY_MACHINE_802    "ck802"
#define CSKY_MACHINE_803    "ck803"
#define CSKY_MACHINE_807    "ck807"
#define CSKY_MACHINE_810    "ck810"
#define CSKY_MACHINE_860    "ck860"

#if LW_CFG_CACHE_EN > 0
VOID    archCacheReset(CPCHAR     pcMachineName);
VOID    archCacheInit(CACHE_MODE  uiInstruction, CACHE_MODE  uiData, CPCHAR  pcMachineName);

#define __ARCH_CACHE_INIT   archCacheInit
#endif                                                                  /*  LW_CFG_CACHE_EN > 0         */

/*********************************************************************************************************
  C-SKY 处理器 MMU 操作 (armMmu*() 可供给 bsp 使用)
*********************************************************************************************************/

#if LW_CFG_VMM_EN > 0
VOID    archMmuInit(CPCHAR  pcMachineName);

#define __ARCH_MMU_INIT     archMmuInit
#endif                                                                  /*  LW_CFG_VMM_EN > 0           */

/*********************************************************************************************************
  C-SKY 处理器 MPU 操作 (bsp 初始化阶段直接调用下面函数初始化 MPU)
*********************************************************************************************************/

#if (LW_CFG_VMM_EN == 0) && (LW_CFG_CSKY_MPU > 0)
VOID    archMpuInit(CPCHAR  pcMachineName, const CSKY_MPU_REGION  mpuregion[]);
#endif

/*********************************************************************************************************
  C-SKY 处理器多核自旋锁操作
*********************************************************************************************************/

#if LW_CFG_SMP_EN > 0
VOID    archSpinBypass(VOID);
VOID    archSpinWork(VOID);

#define __ARCH_SPIN_BYPASS  archSpinBypass
#define __ARCH_SPIN_WORK    archSpinWork

VOID    archSpinInit(spinlock_t  *psl);
VOID    archSpinDelay(VOID);
VOID    archSpinNotify(VOID);

#define __ARCH_SPIN_INIT    archSpinInit
#define __ARCH_SPIN_DELAY   archSpinDelay
#define __ARCH_SPIN_NOTIFY  archSpinNotify

INT     archSpinLock(spinlock_t  *psl, PLW_CLASS_CPU  pcpuCur, VOIDFUNCPTR  pfuncPoll, PVOID  pvArg);
INT     archSpinTryLock(spinlock_t  *psl, PLW_CLASS_CPU  pcpuCur);
INT     archSpinUnlock(spinlock_t  *psl, PLW_CLASS_CPU  pcpuCur);

#define __ARCH_SPIN_LOCK    archSpinLock
#define __ARCH_SPIN_TRYLOCK archSpinTryLock
#define __ARCH_SPIN_UNLOCK  archSpinUnlock

INT     archSpinLockRaw(spinlock_t  *psl);
INT     archSpinTryLockRaw(spinlock_t  *psl);
INT     archSpinUnlockRaw(spinlock_t  *psl);

#define __ARCH_SPIN_LOCK_RAW    archSpinLockRaw
#define __ARCH_SPIN_TRYLOCK_RAW archSpinTryLockRaw
#define __ARCH_SPIN_UNLOCK_RAW  archSpinUnlockRaw

ULONG   archMpCur(VOID);
VOID    archMpInt(ULONG  ulCPUId);
#endif                                                                  /*  LW_CFG_SMP_EN               */

/*********************************************************************************************************
  C-SKY 内存屏障
*********************************************************************************************************/

#define KN_BARRIER()            __asm__ __volatile__ ("" : : : "memory")

#if LW_CFG_CSKY_CACHE_V2 > 0

#define KN_MB()                 __asm__ __volatile__ ("bar.brwarw" : : : "memory")
#define KN_RMB()                __asm__ __volatile__ ("bar.brar"   : : : "memory")
#define KN_WMB()                __asm__ __volatile__ ("bar.bwaw"   : : : "memory")

#define KN_DMA_RMB()            KN_RMB()
#define KN_DMA_WMB()            KN_WMB()

#if LW_CFG_SMP_EN > 0
#define KN_SMP_MB()             __asm__ __volatile__ ("bar.brwarws" : : : "memory")
#define KN_SMP_RMB()            __asm__ __volatile__ ("bar.brars"   : : : "memory")
#define KN_SMP_WMB()            __asm__ __volatile__ ("bar.bwaws"   : : : "memory")

#else
#define KN_SMP_MB()             KN_BARRIER()
#define KN_SMP_RMB()            KN_BARRIER()
#define KN_SMP_WMB()            KN_BARRIER()
#endif                                                                  /*  LW_CFG_SMP_EN > 0           */

#else                                                                   /*  LW_CFG_CSKY_CACHE_V2 > 0    */

#define KN_MB()                 __asm__ __volatile__ ("sync" : : : "memory")

#define KN_RMB()                KN_MB()
#define KN_WMB()                KN_MB()

#define KN_DMA_RMB()            KN_RMB()
#define KN_DMA_WMB()            KN_WMB()

#if LW_CFG_SMP_EN > 0
#define KN_SMP_MB()             KN_MB()
#define KN_SMP_RMB()            KN_RMB()
#define KN_SMP_WMB()            KN_WMB()

#else
#define KN_SMP_MB()             KN_BARRIER()
#define KN_SMP_RMB()            KN_BARRIER()
#define KN_SMP_WMB()            KN_BARRIER()
#endif

#endif                                                                  /*  LW_CFG_CSKY_CACHE_V2 == 0   */

/*********************************************************************************************************
  C-SKY 处理器浮点运算器
*********************************************************************************************************/

#define CSKY_FPU_FPUV1    "fpuv1"
#define CSKY_FPU_FPUV2    "fpuv2"
#define CSKY_FPU_NONE     "none"

#if LW_CFG_CPU_FPU_EN > 0
VOID    archFpuPrimaryInit(CPCHAR  pcMachineName, CPCHAR  pcFpuName);
#if LW_CFG_SMP_EN > 0
VOID    archFpuSecondaryInit(CPCHAR  pcMachineName, CPCHAR  pcFpuName);
#endif                                                                  /*  LW_CFG_SMP_EN               */

VOID    archFpuCtxInit(PVOID pvFpuCtx);
VOID    archFpuEnable(VOID);
VOID    archFpuDisable(VOID);
VOID    archFpuSave(PVOID pvFpuCtx);
VOID    archFpuRestore(PVOID pvFpuCtx);

#define __ARCH_FPU_CTX_INIT     archFpuCtxInit
#define __ARCH_FPU_ENABLE       archFpuEnable
#define __ARCH_FPU_DISABLE      archFpuDisable
#define __ARCH_FPU_SAVE         archFpuSave
#define __ARCH_FPU_RESTORE      archFpuRestore

#if LW_CFG_DEVICE_EN > 0
VOID    archFpuCtxShow(INT  iFd, PVOID pvFpuCtx);
#define __ARCH_FPU_CTX_SHOW     archFpuCtxShow
#endif                                                                  /*  LW_CFG_DEVICE_EN            */

INT     archFpuUndHandle(PLW_CLASS_TCB  ptcbCur);
#endif                                                                  /*  LW_CFG_CPU_FPU_EN           */

/*********************************************************************************************************
  bsp 需要提供的接口如下:
*********************************************************************************************************/
/*********************************************************************************************************
  C-SKY 处理器异常回调 (可不定义)
*********************************************************************************************************/

#if LW_CFG_CPU_EXC_HOOK_EN > 0
#define ARCH_INSTRUCTION_EXCEPTION  0                                   /*  iExcType                    */
#define ARCH_DATA_EXCEPTION         1
#define ARCH_MACHINE_EXCEPTION      2
#define ARCH_BUS_EXCEPTION          3

INT     bspCpuExcHook(PLW_CLASS_TCB   ptcb,
                      addr_t          ulRetAddr,
                      addr_t          ulExcAddr,
                      INT             iExcType,
                      INT             iExcInfo);
#endif                                                                  /*  LW_CFG_CPU_EXC_HOOK_EN      */

/*********************************************************************************************************
  C-SKY 处理器中断向量判读
*********************************************************************************************************/

VOID    bspIntInit(VOID);
VOID    bspFastIntHandle(ULONG  ulVector);
VOID    bspIntHandle(ULONG  ulVector);

UINT32  bspIntInitEnableStatus(VOID);

VOID    bspIntVectorEnable(ULONG  ulVector);
VOID    bspIntVectorDisable(ULONG  ulVector);
BOOL    bspIntVectorIsEnable(ULONG  ulVector);

#define __ARCH_INT_VECTOR_ENABLE    bspIntVectorEnable
#define __ARCH_INT_VECTOR_DISABLE   bspIntVectorDisable
#define __ARCH_INT_VECTOR_ISENABLE  bspIntVectorIsEnable

#if LW_CFG_INTER_PRIO > 0
ULONG   bspIntVectorSetPriority(ULONG  ulVector, UINT  uiPrio);
ULONG   bspIntVectorGetPriority(ULONG  ulVector, UINT *puiPrio);

#define __ARCH_INT_VECTOR_SETPRIO   bspIntVectorSetPriority
#define __ARCH_INT_VECTOR_GETPRIO   bspIntVectorGetPriority
#endif                                                                  /*  LW_CFG_INTER_PRIO > 0       */

#if LW_CFG_INTER_TARGET > 0
ULONG   bspIntVectorSetTarget(ULONG  ulVector, size_t  stSize, const PLW_CLASS_CPUSET  pcpuset);
ULONG   bspIntVectorGetTarget(ULONG  ulVector, size_t  stSize, PLW_CLASS_CPUSET  pcpuset);

#define __ARCH_INT_VECTOR_SETTARGET bspIntVectorSetTarget
#define __ARCH_INT_VECTOR_GETTARGET bspIntVectorGetTarget
#endif                                                                  /*  LW_CFG_INTER_TARGET > 0     */

/*********************************************************************************************************
  CPU 定时器时钟
*********************************************************************************************************/

VOID    bspTickInit(VOID);
VOID    bspDelayUs(ULONG ulUs);
VOID    bspDelayNs(ULONG ulNs);

/*********************************************************************************************************
  TICK 高精度时间修正
  如未实现则可定义为空函数, 同时系统初始化前需调用 API_TimeNanoSleepMethod(LW_TIME_NANOSLEEP_METHOD_TICK)
*********************************************************************************************************/

#if LW_CFG_TIME_HIGH_RESOLUTION_EN > 0
VOID    bspTickHighResolution(struct timespec *ptv);
#endif                                                                  /*  LW_CFG_TIME_HIGH_...        */

/*********************************************************************************************************
  内核关键位置回调函数
*********************************************************************************************************/

VOID    bspTaskCreateHook(LW_OBJECT_ID  ulId);
VOID    bspTaskDeleteHook(LW_OBJECT_ID  ulId, PVOID  pvReturnVal, PLW_CLASS_TCB  ptcb);
VOID    bspTaskSwapHook(LW_OBJECT_HANDLE  hOldThread, LW_OBJECT_HANDLE  hNewThread);
VOID    bspTaskIdleHook(VOID);
VOID    bspTickHook(INT64  i64Tick);
VOID    bspWdTimerHook(LW_OBJECT_ID  ulId);
VOID    bspTCBInitHook(LW_OBJECT_ID  ulId, PLW_CLASS_TCB   ptcb);
VOID    bspKernelInitHook(VOID);

/*********************************************************************************************************
  系统重启操作 (ulStartAddress 参数用于调试, BSP 可忽略)
*********************************************************************************************************/

VOID    bspReboot(INT  iRebootType, addr_t  ulStartAddress);

/*********************************************************************************************************
  系统关键信息打印 (打印信息不可依赖任何操作系统 api)
*********************************************************************************************************/

VOID    bspDebugMsg(CPCHAR pcMsg);

/*********************************************************************************************************
  BSP 信息
*********************************************************************************************************/

CPCHAR  bspInfoCpu(VOID);
CPCHAR  bspInfoCache(VOID);
CPCHAR  bspInfoPacket(VOID);
CPCHAR  bspInfoVersion(VOID);
ULONG   bspInfoHwcap(VOID);
addr_t  bspInfoRomBase(VOID);
size_t  bspInfoRomSize(VOID);
addr_t  bspInfoRamBase(VOID);
size_t  bspInfoRamSize(VOID);

/*********************************************************************************************************
  C-SKY 处理器 MMU 操作
*********************************************************************************************************/

#if LW_CFG_VMM_EN > 0
ULONG   bspMmuPgdMaxNum(VOID);
ULONG   bspMmuPteMaxNum(VOID);
#endif                                                                  /*  LW_CFG_VMM_EN > 0           */

/*********************************************************************************************************
  C-SKY 处理器多核操作
*********************************************************************************************************/

#if LW_CFG_SMP_EN > 0
VOID    bspMpInt(ULONG  ulCPUId);
VOID    bspCpuUp(ULONG  ulCPUId);                                       /*  启动一个 CPU                */

#if LW_CFG_SMP_CPU_DOWN_EN > 0
VOID    bspCpuDown(ULONG  ulCPUId);                                     /*  停止一个 CPU                */
#endif

#if LW_CFG_CPU_ARCH_SMT > 0
ULONG   bspCpuLogic2Physical(ULONG  ulCPUId);
#endif
#endif                                                                  /*  LW_CFG_SMP_EN               */

/*********************************************************************************************************
  C-SKY 处理器 CPU 功耗 (升降主频)
*********************************************************************************************************/

#if LW_CFG_POWERM_EN > 0
VOID    bspSuspend(VOID);                                               /*  系统休眠                    */
VOID    bspCpuPowerSet(UINT  uiPowerLevel);                             /*  设置 CPU 主频等级           */
VOID    bspCpuPowerGet(UINT *puiPowerLevel);                            /*  获取 CPU 主频等级           */
#endif                                                                  /*  LW_CFG_POWERM_EN > 0        */

/*********************************************************************************************************
  Trusted computing
*********************************************************************************************************/

#if LW_CFG_TRUSTED_COMPUTING_EN > 0
VOID    bspTrustedModuleInit(VOID);
INT     bspTrustedModuleCheck(const PCHAR  pcPath);
VOID    bspTrustedModuleLoad(PVOID  pvModule);
VOID    bspTrustedModuleUnload(PVOID  pvModule);
#endif                                                                  /*  LW_CFG_TRUSTED_COMPUTING_EN */

#endif                                                                  /*  __ARCH_CSKY_SUPPORT_H       */
/*********************************************************************************************************
  END
*********************************************************************************************************/

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
** 文   件   名: arm_support.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2013 年 12 月 09 日
**
** 描        述: ARM 体系构架支持.
*********************************************************************************************************/

#ifndef __ARCH_ARM_SUPPORT_H
#define __ARCH_ARM_SUPPORT_H

#define __LW_SCHEDULER_BUG_TRACE_EN                                     /*  测试多核调度器              */
#define __LW_KERNLOCK_BUG_TRACE_EN                                      /*  测试内核锁                  */

#if LW_CFG_SPINLOCK_RESTRICT_EN > 0
#define __LW_SPINLOCK_BUG_TRACE_EN                                      /*  测试自旋锁                  */
#endif

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

VOID    archKernelParam(CPCHAR  pcParam);

/*********************************************************************************************************
  ARM 处理器断言
*********************************************************************************************************/

VOID    archAssert(CPCHAR  pcCond, CPCHAR  pcFunc, CPCHAR  pcFile, INT  iLine);

/*********************************************************************************************************
  ARM 处理器线程上下文相关接口
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

#if defined(__SYLIXOS_ARM_ARCH_M__)
VOID        archTaskCtxCopy(ARCH_REG_CTX                *pregctxDest,
                            const ARCH_SW_SAVE_REG_CTX  *pSwSaveCtx,
                            const ARCH_HW_SAVE_REG_CTX  *pHwSaveCtx);
#else
VOID        archTaskCtxCopy(ARCH_REG_CTX  *pregctxDest, const ARCH_REG_CTX  *pregctxSrc);
#endif

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
  ARM 处理器调试接口
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
  ARM 处理器异常接口
*********************************************************************************************************/

#if !defined(__SYLIXOS_ARM_ARCH_M__)
VOID    archIntEntry(VOID);
VOID    archAbtEntry(VOID);
VOID    archPreEntry(VOID);
VOID    archUndEntry(VOID);
VOID    archSwiEntry(VOID);

#else
VOID    archResetHandle(VOID);
VOID    archNMIIntEntry(VOID);
VOID    archHardFaultEntry(VOID);
VOID    archMemFaultEntry(VOID);
VOID    archBusFaultEntry(VOID);
VOID    archUsageFaultEntry(VOID);
VOID    archDebugMonitorEntry(VOID);
VOID    archPendSVEntry(VOID);
VOID    archSysTickIntEntry(VOID);
VOID    archSvcEntry(VOID);
VOID    archReservedIntEntry(VOID);
VOID    archIntEntry16(VOID);                                           /*  archIntEntry16 ~ 255        */
#endif                                                                  /*  !__SYLIXOS_ARM_ARCH_M__     */

VOID    archIntHandle(ULONG  ulVector, BOOL  bPreemptive);              /*  bspIntHandle 需要调用此函数 */

/*********************************************************************************************************
  ARM 通用库
*********************************************************************************************************/

INT     archFindLsb(UINT32 ui32);
INT     archFindMsb(UINT32 ui32);

/*********************************************************************************************************
  ARM 处理器标准底层库
*********************************************************************************************************/

#define KN_INT_DISABLE()            archIntDisable()
#define KN_INT_ENABLE(intLevel)     archIntEnable(intLevel)
#define KN_INT_ENABLE_FORCE()       archIntEnableForce()

INTREG  archIntDisable(VOID);
VOID    archIntEnable(INTREG  iregInterLevel);
VOID    archIntEnableForce(VOID);

VOID    archPageCopy(PVOID pvTo, PVOID pvFrom);

#define KN_COPY_PAGE(to, from)      archPageCopy(to, from)

VOID    archReboot(INT  iRebootType, addr_t  ulStartAddress);

#if !defined(__SYLIXOS_ARM_ARCH_M__)
INTREG  archGetCpsr(VOID);
#endif

/*********************************************************************************************************
  ARM 处理器 BogoMIPS 循环
*********************************************************************************************************/

VOID    archBogoMipsLoop(ULONG  ulLoop);

#define __ARCH_BOGOMIPS_LOOP            archBogoMipsLoop
#define __ARCH_BOGOMIPS_INS_PER_LOOP    8

/*********************************************************************************************************
  ARM CP15 基本功能
*********************************************************************************************************/

#if LW_CFG_ARM_CP15 > 0
VOID    armWaitForInterrupt(VOID);
VOID    armFastBusMode(VOID);
VOID    armAsyncBusMode(VOID);
VOID    armSyncBusMode(VOID);
#endif                                                                  /*  LW_CFG_ARM_CP15 > 0         */

/*********************************************************************************************************
  ARM 处理器 CACHE 操作
*********************************************************************************************************/

#define ARM_MACHINE_920     "920"                                       /*  ARMv4                       */
#define ARM_MACHINE_926     "926"                                       /*  ARMv5                       */
#define ARM_MACHINE_1136    "1136"                                      /*  ARMv6                       */
#define ARM_MACHINE_1176    "1176"

#define ARM_MACHINE_A5      "A5"                                        /*  ARMv7                       */
#define ARM_MACHINE_A7      "A7"
#define ARM_MACHINE_A8      "A8"
#define ARM_MACHINE_A9      "A9"
#define ARM_MACHINE_A15     "A15"
#define ARM_MACHINE_A17     "A17"
                                                                        /*  ARMv8                       */
#define ARM_MACHINE_A53     "A53"                                       /*  A35 ... compatible          */
#define ARM_MACHINE_A57     "A57"                                       /*  FT1500A/2000 ... compatible */
#define ARM_MACHINE_A72     "A72"
#define ARM_MACHINE_A73     "A73"

#define ARM_MACHINE_R4      "R4"                                        /*  ARMv7 R                     */
#define ARM_MACHINE_R5      "R5"
#define ARM_MACHINE_R7      "R7"

#define ARM_MACHINE_M3      "M3"                                        /*  ARMv7 M                     */
#define ARM_MACHINE_M4      "M4"
#define ARM_MACHINE_M7      "M7"

#if LW_CFG_CACHE_EN > 0
VOID    archCacheReset(CPCHAR     pcMachineName);
VOID    archCacheInit(CACHE_MODE  uiInstruction, CACHE_MODE  uiData, CPCHAR  pcMachineName);

#define __ARCH_CACHE_INIT   archCacheInit
#endif                                                                  /*  LW_CFG_CACHE_EN > 0         */

/*********************************************************************************************************
  ARM 处理器 MMU 操作 (armMmu*() 可供给 bsp 使用)
*********************************************************************************************************/

#if LW_CFG_VMM_EN > 0
VOID    archMmuInit(CPCHAR  pcMachineName);

#define __ARCH_MMU_INIT     archMmuInit

#if LW_CFG_CPU_PHYS_ADDR_64BIT == 0
VOID    armMmuV7ForceShare(BOOL  bEnOrDis);                             /*  强制置位共享位              */
VOID    armMmuV7ForceNonSecure(BOOL  bEnOrDis);                         /*  强制使用 Non-Secure 模式    */

#define ARM_MMU_V7_DEV_STRONGLY_ORDERED     0                           /*  设备内存强排序              */
#define ARM_MMU_V7_DEV_SHAREABLE            1                           /*  共享设备内存 (默认)         */
#define ARM_MMU_V7_DEV_NON_SHAREABLE        2                           /*  非共享设备内存              */

VOID    armMmuV7ForceDevType(UINT  uiType);                             /*  选择设备内存类型            */

#else
VOID    armMmuV7ShareableSet(UINT  uiInnerOrOuter);                     /*  MMU 系统 share 模式设置     */
UINT    armMmuV7ShareableGet(VOID);                                     /*  MMU 系统 share 模式获取     */
#endif                                                                  /*  LW_CFG_CPU_PHYS_ADDR_64BIT  */
#endif                                                                  /*  LW_CFG_VMM_EN > 0           */

/*********************************************************************************************************
  ARM 处理器 MPU 操作 (bsp 初始化阶段直接调用下面函数初始化 MPU)
  注意: archMpuInit() 操作会开启 MPU.
        Cortex-M bsp 在调用此函数前, 需设置好 MPU CTRL 寄存器 MPU HFNMI and PRIVILEGED Access control
        Cortex-M bsp 在调用此函数后, 需使能 MEM FAULT 异常, 这个异常中可选择调用操作系统相关接口打印调用栈
        以及任务上下文. 然后复位.
*********************************************************************************************************/

#if (LW_CFG_VMM_EN == 0) && (LW_CFG_ARM_MPU > 0)
VOID    archMpuInit(CPCHAR  pcMachineName, const ARM_MPU_REGION  mpuregion[]);
#endif

/*********************************************************************************************************
  ARM 处理器多核自旋锁操作
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
  ARM 内存屏障
*********************************************************************************************************/

#ifdef __GNUC__
#if __SYLIXOS_ARM_ARCH__ >= 7
#define armIsb(opt)     __asm__ __volatile__ ("isb " #opt : : : "memory")
#define armDsb(opt)     __asm__ __volatile__ ("dsb " #opt : : : "memory")
#define armDmb(opt)     __asm__ __volatile__ ("dmb " #opt : : : "memory")

#elif __SYLIXOS_ARM_ARCH__ == 6
#define armIsb(x)       __asm__ __volatile__ ("mcr p15, 0, %0, c7, c5,  4" : : "r" (0) : "memory")
#define armDsb(x)       __asm__ __volatile__ ("mcr p15, 0, %0, c7, c10, 4" : : "r" (0) : "memory")
#define armDmb(x)       __asm__ __volatile__ ("mcr p15, 0, %0, c7, c10, 5" : : "r" (0) : "memory")

#else
#define armIsb(x)       __asm__ __volatile__ ("" : : : "memory")
#define armDsb(x)       __asm__ __volatile__ ("" : : : "memory")
#define armDmb(x)       __asm__ __volatile__ ("" : : : "memory")
#endif                                                                  /*  __SYLIXOS_ARM_ARCH__        */

#else
#define armIsb(x)
#define armDsb(x)
#define armDmb(x)
#endif                                                                  /*  __GNUC__                    */

#if __SYLIXOS_ARM_ARCH__ >= 7 && LW_CFG_ARM_CACHE_L2 > 0
VOID    armL2Sync(VOID);
#define KN_HEAVY_MB(x...)   do { armDsb(x); armL2Sync(); } while(0)
#else
#define KN_HEAVY_MB(x...)   armDsb(x)
#endif                                                                  /*  __SYLIXOS_ARM_ARCH__ >= 7   */
                                                                        /*  LW_CFG_ARM_CACHE_L2 > 0     */

#define armIsbOp(x...)      armIsb(x)
#define armDsbOp(x...)      armDsb(x)
#define armDmbOp(x...)      armDmb(x)

/*********************************************************************************************************
  ARM 内存屏障标准函数
*********************************************************************************************************/

#define KN_MB()         KN_HEAVY_MB()
#define KN_RMB()        armDsbOp()
#define KN_WMB()        KN_HEAVY_MB(st)

#define KN_DMA_RMB()    armDmb(osh)
#define KN_DMA_WMB()    armDmb(oshst)

#if LW_CFG_SMP_EN > 0
#ifdef __SYLIXOS_SMPFMB                                                 /*  Fast SMP MB                 */
#define KN_SMP_MB()     armDmb(ish)
#define KN_SMP_RMB()    armDmb(ish)
#define KN_SMP_WMB()    armDmb(ishst)

#else                                                                   /*  __SYLIXOS_SMPFMB            */
#define KN_SMP_MB()     armDmbOp()
#define KN_SMP_RMB()    armDmbOp()
#define KN_SMP_WMB()    armDmbOp()
#endif                                                                  /*  !__SYLIXOS_SMPFMB           */

#else
#ifdef __GNUC__
#define KN_SMP_MB()     __asm__ __volatile__ ("" : : : "memory")
#define KN_SMP_RMB()    __asm__ __volatile__ ("" : : : "memory")
#define KN_SMP_WMB()    __asm__ __volatile__ ("" : : : "memory")

#else
#define KN_SMP_MB()     
#define KN_SMP_RMB()
#define KN_SMP_WMB()
#endif                                                                  /*  __GNUC__                    */
#endif                                                                  /*  LW_CFG_SMP_EN               */

#ifdef __GNUC__
#define KN_BARRIER()    __asm__ __volatile__ ("" : : : "memory")
#else
#define KN_BARRIER()
#endif                                                                  /*  __SYLIXOS_ARM_ARCH__ >= 7   */

/*********************************************************************************************************
  ARM 处理器浮点运算器 
  注意: neon 对应的浮点运算器为 ARM_FPU_VFPv3 或者 ARM_FPU_VFPv4
  VFP9 表示 VFPv2 for ARM9 处理器 VFP11 表示 VFPv2 for ARM11 处理器, VFP11 VFPv3 VFPv4 可自行判断寄存器数
  ARM_FPU_VFP9 需要用户选择寄存器数量是 D16 还是 D32.
  带有硬浮点支持的 Cortex-M bsp 可选择 vfpv4 即可.
*********************************************************************************************************/

#define ARM_FPU_NONE        "none"
#define ARM_FPU_VFP9_D16    "vfp9-d16"
#define ARM_FPU_VFP9_D32    "vfp9-d32"
#define ARM_FPU_VFP11       "vfp11"
#define ARM_FPU_VFPv3       "vfpv3"
#define ARM_FPU_VFPv4       "vfpv4"
#define ARM_FPU_NEONv3      ARM_FPU_VFPv3                               /*  Context same as vfpv3       */
#define ARM_FPU_NEONv4      ARM_FPU_VFPv4                               /*  Context same as vfpv4       */

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
#endif                                                                  /*  #if LW_CFG_DEVICE_EN        */

INT     archFpuUndHandle(PLW_CLASS_TCB  ptcbCur);
#endif                                                                  /*  LW_CFG_CPU_FPU_EN           */

/*********************************************************************************************************
  bsp 需要提供的接口如下:
*********************************************************************************************************/
/*********************************************************************************************************
  ARM 处理器异常回调 (可不定义)
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
  ARM 处理器中断向量判读
*********************************************************************************************************/

VOID    bspIntInit(VOID);
VOID    bspIntHandle(VOID);

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
  ARM 处理器 CACHE 操作
*********************************************************************************************************/

#if LW_CFG_ARM_CACHE_L2 > 0
INT     bspL2CBase(addr_t *pulBase);                                    /*  L2 cache 控制器基地址       */
INT     bspL2CAux(UINT32 *puiAuxVal, UINT32 *puiAuxMask);               /*  L2 cache aux                */
#endif                                                                  /*  LW_CFG_ARM_CACHE_L2 > 0     */

/*********************************************************************************************************
  ARM 处理器 MMU 操作
*********************************************************************************************************/

#if LW_CFG_VMM_EN > 0
ULONG   bspMmuPgdMaxNum(VOID);
ULONG   bspMmuPmdMaxNum(VOID);
ULONG   bspMmuPteMaxNum(VOID);
#endif                                                                  /*  LW_CFG_VMM_EN > 0           */

/*********************************************************************************************************
  ARM 处理器多核操作
*********************************************************************************************************/

#if LW_CFG_SMP_EN > 0
VOID    bspMpInt(ULONG  ulCPUId);
VOID    bspCpuUp(ULONG  ulCPUId);                                       /*  启动一个 CPU                */

#if LW_CFG_SMP_CPU_DOWN_EN > 0
VOID    bspCpuDown(ULONG  ulCPUId);                                     /*  停止一个 CPU                */
#endif                                                                  /*  LW_CFG_SMP_CPU_DOWN_EN > 0  */

#if LW_CFG_CPU_ARCH_SMT > 0
ULONG   bspCpuLogic2Physical(ULONG  ulCPUId);
#endif
#endif                                                                  /*  LW_CFG_SMP_EN               */

/*********************************************************************************************************
  ARM 处理器 CPU 功耗 (升降主频)
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

#endif                                                                  /*  __ARCH_ARM_SUPPORT_H        */
/*********************************************************************************************************
  END
*********************************************************************************************************/

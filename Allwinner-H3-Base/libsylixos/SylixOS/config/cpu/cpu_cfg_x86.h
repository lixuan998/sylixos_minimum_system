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
** 文   件   名: cpu_cfg_x86.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2015 年 11 月 20 日
**
** 描        述: x86 CPU 类型与功能配置.
*********************************************************************************************************/

#ifndef __CPU_CFG_X86_H
#define __CPU_CFG_X86_H

/*********************************************************************************************************
  CPU 体系结构
*********************************************************************************************************/

#define LW_CFG_CPU_ARCH_X86             1                               /*  CPU 架构                    */
#if defined(__x86_64__)
#define LW_CFG_CPU_ARCH_FAMILY          "x86-64(R)"                     /*  x64 family                  */
#else
#define LW_CFG_CPU_ARCH_FAMILY          "x86(R)"                        /*  x86 family                  */
#endif

/*********************************************************************************************************
  SMT 同步多线程调度优化
*********************************************************************************************************/

#define LW_CFG_CPU_ARCH_SMT             1                               /*  同步多线程优化              */

/*********************************************************************************************************
  CACHE LINE 对齐
*********************************************************************************************************/

#define LW_CFG_CPU_ARCH_CACHE_LINE      128                             /*  cache 最大行对齐属性        */

/*********************************************************************************************************
  CPU 体系结构配置
*********************************************************************************************************/

#define LW_CFG_CPU_X86_NO_BARRIER       0                               /*  不支持内存屏障指令          */
                                                                        /*  老式奔腾处理器 (1, 2, 3, 4) */
#define LW_CFG_CPU_X86_NO_PAUSE         0                               /*  不支持 PAUSE 指令           */
#define LW_CFG_CPU_X86_NO_HLT           0                               /*  不支持 HLT 指令             */
#define LW_CFG_CPU_X86_APIC_BUS_INT     0                               /*  使用 APIC BUS 而不是前端总线*/
                                                                        /*  投递中断(老式奔腾 1, 2, 3)  */
/*********************************************************************************************************
  快速获取线程上下文 (使用 %FS Platform 特性)
*********************************************************************************************************/

#define LW_CFG_CPU_FAST_TLS             0                               /*  快速获取任务上下文          */

/*********************************************************************************************************
  CPU 字长与整型大小端定义
*********************************************************************************************************/

#define LW_CFG_CPU_ENDIAN               0                               /*  0: 小端  1: 大端            */
#if defined(__x86_64__)
#define LW_CFG_CPU_WORD_LENGHT          64                              /*  CPU 字长                    */
#define LW_CFG_CPU_PHYS_ADDR_64BIT      1                               /*  物理地址 64bit 宽度         */

#else
#define LW_CFG_CPU_WORD_LENGHT          32                              /*  CPU 字长                    */
#define LW_CFG_CPU_PHYS_ADDR_64BIT      0                               /*  物理地址 64bit 宽度         */
#endif

/*********************************************************************************************************
  浮点运算单元
*********************************************************************************************************/

#define LW_CFG_CPU_FPU_EN               1                               /*  CPU 是否拥有 FPU            */
#define LW_CFG_CPU_FPU_XSAVE_SIZE       1200                            /*  XSAVE & XRSTOR 上下文大小   */

/*********************************************************************************************************
  DSP 数字信号处理器
*********************************************************************************************************/

#define LW_CFG_CPU_DSP_EN               0                               /*  CPU 是否拥有 DSP            */

/*********************************************************************************************************
  ATOMIC
*********************************************************************************************************/

#define LW_CFG_CPU_ATOMIC_EN            1

/*********************************************************************************************************
  x86 32bit CPU 的 atomic64 操作使用 cmpxchg8b 指令实现(586 和后面推出的 x86 32bit CPU 都有),
  80386 和 80486 CPU 没有 cmpxchg8b 指令, 80386 和 80486 CPU 需要将 LW_CFG_CPU_ATOMIC64_EN 配置为 0
*********************************************************************************************************/

#if defined(__i586__) || defined(__x86_64__)
#define LW_CFG_CPU_ATOMIC64_EN          1
#else
#define LW_CFG_CPU_ATOMIC64_EN          0
#endif                                                                  /*  __i586__ || __x86_64__      */

#endif                                                                  /*  __CPU_CFG_X86_H             */
/*********************************************************************************************************
  END
*********************************************************************************************************/

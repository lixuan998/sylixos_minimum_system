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
** 文   件   名: cpu_cfg_mips.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2015 年 11 月 20 日
**
** 描        述: MIPS CPU 类型与功能配置.
*********************************************************************************************************/

#ifndef __CPU_CFG_MIPS_H
#define __CPU_CFG_MIPS_H

/*********************************************************************************************************
  CPU 体系结构
*********************************************************************************************************/

#define LW_CFG_CPU_ARCH_MIPS            1                               /*  CPU 架构                    */

#if defined(_MIPS_ARCH_MIPS64R2) || (defined(_MIPS_ARCH_HR2) || defined(_MIPS_ARCH_HCW))
#define LW_CFG_CPU_ARCH_FAMILY          "MIPS64R2(R)"                   /*  MIPS64R2 family             */

#elif defined(_MIPS_ARCH_MIPS64)
#define LW_CFG_CPU_ARCH_FAMILY          "MIPS64(R)"                     /*  MIPS64 family               */

#elif defined(_MIPS_ARCH_MIPS32R2)
#define LW_CFG_CPU_ARCH_FAMILY          "MIPS32R2(R)"                   /*  MIPS32R2 family             */

#else
#define LW_CFG_CPU_ARCH_FAMILY          "MIPS32(R)"                     /*  MIPS32 family               */
#endif

/*********************************************************************************************************
  SMT 同步多线程调度优化
*********************************************************************************************************/

#define LW_CFG_CPU_ARCH_SMT             0                               /*  同步多线程优化              */

/*********************************************************************************************************
  CACHE LINE 对齐
*********************************************************************************************************/

#define LW_CFG_CPU_ARCH_CACHE_LINE      64                              /*  cache 最大行对齐属性        */

/*********************************************************************************************************
  CPU 字长与整型大小端定义
*********************************************************************************************************/

#if defined(__GNUC__)
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define LW_CFG_CPU_ENDIAN               0                               /*  0: 小端                     */
#else
#define LW_CFG_CPU_ENDIAN               1                               /*  1: 大端                     */
#endif                                                                  /*  __BYTE_ORDER__ == LITTLE    */
#else
#define LW_CFG_CPU_ENDIAN               0                               /*  0: 小端  1: 大端            */
#endif                                                                  /*  defined(__GNUC__)           */

#if defined(__mips64)
#define LW_CFG_CPU_WORD_LENGHT          64                              /*  CPU 字长                    */
#define LW_CFG_CPU_PHYS_ADDR_64BIT      1                               /*  物理地址 64bit 宽度         */
#define LW_CFG_CPU_ARCH_MIPS64          1                               /*  CPU 架构                    */

#else
#define LW_CFG_CPU_WORD_LENGHT          32                              /*  CPU 字长                    */
#define LW_CFG_CPU_PHYS_ADDR_64BIT      0                               /*  物理地址 64bit 宽度         */
#endif

/*********************************************************************************************************
  MIPS MMU 配置

  LW_CFG_MIPS_PAGE_SHIFT 可配置的值有如下:
        12 :  4K Bytes per page.
        14 : 16K Bytes per page.
        16 : 64K Bytes per page.
        18 : 256K Bytes per page.
*********************************************************************************************************/

#define LW_CFG_MIPS_PAGE_SHIFT          14                              /*  MMU 页面大小                */

/*********************************************************************************************************
  MIPS CACHE 配置
*********************************************************************************************************/

#define LW_CFG_MIPS_CACHE_L2            1                               /*  是否允许管理 MIPS 二级 CACHE*/
#define LW_CFG_MIPS_CACHE_L3            1                               /*  是否允许管理 MIPS 三级 CACHE*/

/*********************************************************************************************************
  MIPS 指令配置
*********************************************************************************************************/

#define LW_CFG_MIPS_HAS_CLZ_INSTR       1                               /*  是否支持前导零 CLZ 指令     */
#define LW_CFG_MIPS_HAS_SYNC_INSTR      1                               /*  是否支持 SYNC 指令          */
#define LW_CFG_MIPS_HAS_MSA_INSTR       0                               /*  是否支持 MSA(SIMD) 指令     */
#if defined(_MIPS_ARCH_MIPS64R2) || (defined(_MIPS_ARCH_HR2) || defined(_MIPS_ARCH_HCW)) || defined(_MIPS_ARCH_MIPS32R2)
#define LW_CFG_MIPS_HAS_RDHWR_INSTR     1                               /*  MIPS32/64R2 支持 RDHWR 指令 */
#else
#define LW_CFG_MIPS_HAS_RDHWR_INSTR     0                               /*  是否支持 RDHWR 指令         */
#endif

/*********************************************************************************************************
  解决 MIPS CP0 Hazard 指令配置

  LW_CFG_MIPS_CP0_HAZARD_INSTR 可配置的值有如下:
        0  : EHB.
        1  : SYNC.
        2  : 4 个 SSNOP.
*********************************************************************************************************/

#if defined(_MIPS_ARCH_HR2) || defined(_MIPS_ARCH_HCW)
#define LW_CFG_MIPS_CP0_HAZARD_INSTR    1                               /*  华睿2号使用 SYNC 指令       */
#else
#define LW_CFG_MIPS_CP0_HAZARD_INSTR    0                               /*  使用 EHB 指令               */
#endif

/*********************************************************************************************************
  On the Loongson-2G/2H/3A/3B there is a bug that ll / sc and lld / scd is very weak ordering.
  NOTICE: LW_CFG_MIPS_CPU_LOONGSON1 / LW_CFG_MIPS_CPU_LOONGSON2K / LW_CFG_MIPS_CPU_LOONGSON3 Only one can
          set to 1.
*********************************************************************************************************/

#define LW_CFG_MIPS_CPU_LOONGSON1       0                               /*  Loongson-1x                 */
#define LW_CFG_MIPS_CPU_LOONGSON2K      0                               /*  Loongson-2K                 */
#define LW_CFG_MIPS_CPU_LOONGSON3       1                               /*  Loongson-2G/2H/3A/3B        */

#if (LW_CFG_MIPS_CPU_LOONGSON3 > 0) || (defined(_MIPS_ARCH_HR2) || defined(_MIPS_ARCH_HCW))
#define LW_CFG_MIPS_LOONGSON_LLSC_WAR   1                               /*  龙芯3号，华睿2号 LLSC 竞争  */
#else
#define LW_CFG_MIPS_LOONGSON_LLSC_WAR   0
#endif

#if (LW_CFG_MIPS_CPU_LOONGSON3 > 0) || (LW_CFG_MIPS_CPU_LOONGSON2K > 0) || (defined(_MIPS_ARCH_HR2) || defined(_MIPS_ARCH_HCW))
#define LW_CFG_MIPS_WEAK_REORDERING             1
#define LW_CFG_MIPS_WEAK_REORDERING_BEYOND_LLSC 1
#else
#define LW_CFG_MIPS_WEAK_REORDERING             0
#define LW_CFG_MIPS_WEAK_REORDERING_BEYOND_LLSC 0
#endif

/*********************************************************************************************************
  LL/SC nesting detect fail bug
*********************************************************************************************************/

#if (LW_CFG_MIPS_CPU_LOONGSON3 > 0) || (LW_CFG_MIPS_CPU_LOONGSON2K > 0)
#define LW_CFG_MIPS_NEST_LLSC_BUG       1
#else
#define LW_CFG_MIPS_NEST_LLSC_BUG       0
#endif

/*********************************************************************************************************
  快速获取线程上下文 (使用 UserLocal 寄存器特性)
*********************************************************************************************************/

#if LW_CFG_MIPS_HAS_RDHWR_INSTR > 0
#define LW_CFG_CPU_FAST_TLS             0                               /*  此处可设置为 1              */
#else
#define LW_CFG_CPU_FAST_TLS             0
#endif

/*********************************************************************************************************
  浮点运算单元
*********************************************************************************************************/

#define LW_CFG_CPU_FPU_EN               1                               /*  CPU 是否拥有 FPU            */

/*********************************************************************************************************
  DSP 数字信号处理器
*********************************************************************************************************/

#define LW_CFG_CPU_DSP_EN               1                               /*  CPU 是否拥有 DSP            */

/*********************************************************************************************************
  ATOMIC
*********************************************************************************************************/

#define LW_CFG_CPU_ATOMIC_EN            1

#if LW_CFG_CPU_WORD_LENGHT == 64
#define LW_CFG_CPU_ATOMIC64_EN          1
#else
#define LW_CFG_CPU_ATOMIC64_EN          0
#endif

#endif                                                                  /*  __CPU_CFG_MIPS_H            */
/*********************************************************************************************************
  END
*********************************************************************************************************/

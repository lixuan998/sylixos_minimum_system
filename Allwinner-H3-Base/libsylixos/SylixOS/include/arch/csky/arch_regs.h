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
** 文   件   名: arch_regs.h
**
** 创   建   人: Wang.Xuan (王翾)
**
** 文件创建日期: 2018 年 05 月 10 日
**
** 描        述: C-SKY 寄存器相关.
*********************************************************************************************************/

#ifndef __CSKY_ARCH_REGS_H
#define __CSKY_ARCH_REGS_H

#include "asm/archprob.h"

/*********************************************************************************************************
  C-SKY CK803 体系架构
*********************************************************************************************************/
#if defined(__SYLIXOS_CSKY_ARCH_CK803__)

#include "./ck803/arch_regs.h"

#else
/*********************************************************************************************************
  C-SKY 通用体系架构
*********************************************************************************************************/
/*********************************************************************************************************
  定义
*********************************************************************************************************/

#define ARCH_GREG_NR            32                                      /*  通用寄存器数目              */

#if defined(__SYLIXOS_CSKY_ARCH_CK860__)
#define ARCH_REG_CTX_WORD_SIZE  36                                      /*  寄存器上下文字数            */
#else
#define ARCH_REG_CTX_WORD_SIZE  38                                      /*  寄存器上下文字数            */
#endif
#define ARCH_STK_MIN_WORD_SIZE  256                                     /*  堆栈最小字数                */

#define ARCH_REG_SIZE           4                                       /*  寄存器大小                  */
#define ARCH_REG_CTX_SIZE       (ARCH_REG_CTX_WORD_SIZE * ARCH_REG_SIZE)/*  寄存器上下文大小            */

#define ARCH_STK_ALIGN_SIZE     8                                       /*  堆栈对齐要求                */

#define ARCH_JMP_BUF_WORD_SIZE  38                                      /*  跳转缓冲字数(向后兼容)      */

/*********************************************************************************************************
  寄存器在 ARCH_REG_CTX 中的偏移量
*********************************************************************************************************/

#define XGREG(n)                ((n) * ARCH_REG_SIZE)
#define XPC                     ((ARCH_GREG_NR + 0) * ARCH_REG_SIZE)
#define XPSR                    ((ARCH_GREG_NR + 1) * ARCH_REG_SIZE)
#if defined(__SYLIXOS_CSKY_ARCH_CK860__)
#define XMEH                    ((ARCH_GREG_NR + 2) * ARCH_REG_SIZE)
#else
#define XLO                     ((ARCH_GREG_NR + 2) * ARCH_REG_SIZE)
#define XHI                     ((ARCH_GREG_NR + 3) * ARCH_REG_SIZE)
#define XMEH                    ((ARCH_GREG_NR + 4) * ARCH_REG_SIZE)
#endif
/*********************************************************************************************************
  寄存器表
*********************************************************************************************************/

#if !defined(__ASSEMBLY__) && !defined(ASSEMBLY)

typedef UINT32      ARCH_REG_T;

typedef struct {
    ARCH_REG_T  REG_ulReg[ARCH_GREG_NR];                                /*  32 个通用目的寄存器         */
    ARCH_REG_T  REG_ulPc;                                               /*  程序计数器寄存器            */
    ARCH_REG_T  REG_ulPsr;                                              /*  处理器状态寄存器            */
#if !defined(__SYLIXOS_CSKY_ARCH_CK860__)
    ARCH_REG_T  REG_ulLo;                                              
    ARCH_REG_T  REG_ulHi;                                               
#endif
    ARCH_REG_T  REG_ulMeh;
    ARCH_REG_T  REG_ulPad;

#ifdef __CSKYABIV2__
#define REG_A0                  0                                       /*  第一个参数或函数返回值      */
#define REG_A1                  1                                       /*  第二个参数或函数返回值      */
#define REG_A2                  2                                       /*  第三个参数                  */
#define REG_A3                  3                                       /*  第四个参数                  */
#define REG_S0                  4                                       /*  非易失性寄存器              */
#define REG_S1                  5                                       /*  非易失性寄存器              */
#define REG_S2                  6                                       /*  非易失性寄存器              */
#define REG_S3                  7                                       /*  非易失性寄存器              */
#define REG_S4                  8                                       /*  非易失性寄存器              */
#define REG_S5                  9                                       /*  非易失性寄存器              */
#define REG_S6                  10                                      /*  非易失性寄存器              */
#define REG_S7                  11                                      /*  非易失性寄存器              */
#define REG_S8                  12                                      /*  非易失性寄存器              */
#define REG_S9                  13                                      /*  非易失性寄存器              */
#define REG_SP                  14                                      /*  堆栈指针                    */

#else
#define REG_A0                  2                                       /*  第一个参数或函数返回值      */
#define REG_A1                  3                                       /*  第二个参数或函数返回值      */
#define REG_A2                  4                                       /*  第三个参数                  */
#define REG_A3                  5                                       /*  第四个参数                  */
#define REG_S0                  6                                       /*  非易失性寄存器              */
#define REG_S1                  7                                       /*  非易失性寄存器              */
#define REG_S2                  8                                       /*  非易失性寄存器              */
#define REG_S3                  9                                       /*  非易失性寄存器              */
#define REG_S4                  10                                      /*  非易失性寄存器              */
#define REG_S5                  11                                      /*  非易失性寄存器              */
#define REG_S6                  12                                      /*  非易失性寄存器              */
#define REG_S7                  13                                      /*  非易失性寄存器              */
#define REG_S8                  14                                      /*  非易失性寄存器              */
#define REG_S9                  1                                       /*  非易失性寄存器              */
#define REG_SP                  0                                       /*  堆栈指针                    */
#endif

#define REG_FP                  8                                       /*  FP                          */
#define REG_RA                  15                                      /*  链接寄存器                  */
} ARCH_REG_CTX;

/*********************************************************************************************************
  调用回溯堆栈表
*********************************************************************************************************/

typedef struct {
    ARCH_REG_T      FP_uiFp;
    ARCH_REG_T      FP_uiLr;
} ARCH_FP_CTX;

/*********************************************************************************************************
  从上下文中获取信息
*********************************************************************************************************/

#define ARCH_REG_CTX_GET_PC(ctx)    ((void *)(ctx).REG_ulPc)

#endif                                                                  /*  !defined(__ASSEMBLY__)      */
#endif                                                                  /*  __SYLIXOS_CSKY_ARCH_CK803__ */
#endif                                                                  /*  __CSKY_ARCH_REGS_H          */
/*********************************************************************************************************
  END
*********************************************************************************************************/

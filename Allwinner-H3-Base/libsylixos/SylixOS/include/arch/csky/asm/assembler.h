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
** 文   件   名: assembler.h
**
** 创   建   人: Wang.Xuan (王翾)
**
** 文件创建日期: 2018 年 05 月 10 日
**
** 描        述: C-SKY 汇编相关.
*********************************************************************************************************/

#ifndef __ASMCSKY_ASSEMBLER_H
#define __ASMCSKY_ASSEMBLER_H

#include "archprob.h"
#include "arch/csky/arch_def.h"

/*********************************************************************************************************
  C-SKY architecture assembly special code
*********************************************************************************************************/

#if defined(__ASSEMBLY__) || defined(ASSEMBLY)

#ifndef __MP_CFG_H
#include "../SylixOS/config/mp/mp_cfg.h"
#endif

/*********************************************************************************************************
  assembler define
*********************************************************************************************************/

#define EXPORT_LABEL(label)       .global label

#define IMPORT_LABEL(label)       .extern label

#define FUNC_LABEL(func)          func:
#define LINE_LABEL(line)          line:

#define FUNC_DEF(func)                  \
        .align      4;                  \
func:

#define FUNC_END(func)                  \
        .size   func, . - func

#define MACRO_DEF(mfunc...)             \
        .macro      mfunc

#define MACRO_END()                     \
        .endm

#define FILE_BEGIN()                    \
        .text;                          \
        .align      4;

#define FILE_END()

#define SECTION(sec)                    \
        .section    sec

#define WEAK(name)                      \
        .weak       name;               \
        .align      4;

/*********************************************************************************************************
  Size of a register
*********************************************************************************************************/

#define SZREG       4

/*********************************************************************************************************
  Size of a pointer
*********************************************************************************************************/

#define PTRSIZE     4

/*********************************************************************************************************
  Long define
*********************************************************************************************************/

#define LONG        .word
#define LONGSIZE    4
#define LONGMASK    3
#define LONGLOG     2

/*********************************************************************************************************
  寄存器 ABI 定义
*********************************************************************************************************/

#ifdef __CSKYABIV2__
#define SP                 r14                                          /*  堆栈指针                    */
#define A0                 r0                                           /*  第一个参数或函数返回值      */
#define A1                 r1                                           /*  第二个参数或函数返回值      */
#define A2                 r2                                           /*  第三个参数                  */
#define A3                 r3                                           /*  第四个参数                  */
#define S0                 r4                                           /*  非易失性寄存器              */
#define S1                 r5                                           /*  非易失性寄存器              */
#define S2                 r6                                           /*  非易失性寄存器              */
#define S3                 r7                                           /*  非易失性寄存器              */
#define S4                 r8                                           /*  非易失性寄存器              */
#define S5                 r9                                           /*  非易失性寄存器              */
#define S6                 r10                                          /*  非易失性寄存器              */
#define S7                 r11                                          /*  非易失性寄存器              */
#define S8                 r12                                          /*  非易失性寄存器              */
#define S9                 r13                                          /*  非易失性寄存器              */
#else
#define SP                 r0                                           /*  堆栈指针                    */
#define A0                 r2                                           /*  第一个参数或函数返回值      */
#define A1                 r3                                           /*  第二个参数或函数返回值      */
#define A2                 r4                                           /*  第三个参数                  */
#define A3                 r5                                           /*  第四个参数                  */
#define S0                 r6                                           /*  非易失性寄存器              */
#define S1                 r7                                           /*  非易失性寄存器              */
#define S2                 r8                                           /*  非易失性寄存器              */
#define S3                 r9                                           /*  非易失性寄存器              */
#define S4                 r10                                          /*  非易失性寄存器              */
#define S5                 r11                                          /*  非易失性寄存器              */
#define S6                 r12                                          /*  非易失性寄存器              */
#define G7                 r13                                          /*  非易失性寄存器              */
#define S8                 r14                                          /*  非易失性寄存器              */
#define S9                 r1                                           /*  非易失性寄存器              */
#endif
#define RA                 r15                                          /*  链接寄存器                  */
#define CR                 cr

#endif                                                                  /*  __ASSEMBLY__                */
#endif                                                                  /*  __ASMCSKY_ASSEMBLER_H       */
/*********************************************************************************************************
  END
*********************************************************************************************************/

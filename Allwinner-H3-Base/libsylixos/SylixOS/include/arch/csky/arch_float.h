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
** 文   件   名: arch_float.h
**
** 创   建   人: Wang.Xuan (王翾)
**
** 文件创建日期: 2018 年 05 月 10 日
**
** 描        述: C-SKY 浮点相关.
*********************************************************************************************************/

#ifndef __CSKY_ARCH_FLOAT_H
#define __CSKY_ARCH_FLOAT_H

#include "asm/archprob.h"

/*********************************************************************************************************
  FPU 浮点数据寄存器的相关定义
*********************************************************************************************************/
#if defined(__SYLIXOS_KERNEL) || defined(__ASSEMBLY__) || defined(ASSEMBLY)

#if defined(__SYLIXOS_CSKY_ARCH_CK860__)
#define FPU_REG_NR              32                                      /*  FPU 通用寄存器数量          */
#else
#define FPU_REG_NR              16                                      /*  FPU 通用寄存器数量          */
#endif                                                                  /*  __SYLIXOS_CSKY_ARCH_CK860__ */
#if defined(__SYLIXOS_CSKY_ARCH_CK803__)
#define FPU_REG_WIDTH           32                                      /*  浮点数据寄存器的位宽        */
#else
#define FPU_REG_WIDTH           64                                      /*  浮点数据寄存器的位宽        */
#endif                                                                  /*  __SYLIXOS_CSKY_ARCH_CK803__ */
#define FPU_REG_SIZE            (FPU_REG_WIDTH / 8)                     /*  浮点数据寄存器的大小        */

#if defined(__SYLIXOS_CSKY_ARCH_CK803__)
#define FPU_OFFSET_REG(n)       ((n) * FPU_REG_SIZE)                    /*  浮点数据寄存器偏移          */
#define FPU_OFFSET_FCR          (FPU_OFFSET_REG(FPU_REG_NR))            /*  FCR  偏移                   */
#else
#define FPU_OFFSET_REG_HI(n)    ((n) * FPU_REG_SIZE)                    /*  浮点数据寄存器偏移          */
#define FPU_OFFSET_REG_LO(n)    ((n) * FPU_REG_SIZE + FPU_REG_SIZE >> 1)/*  浮点数据寄存器偏移          */
#define FPU_OFFSET_FCR          (FPU_OFFSET_REG_HI(FPU_REG_NR))         /*  FCR  偏移                   */
#endif                                                                  /*  __SYLIXOS_CSKY_ARCH_CK803__ */
#define FPU_OFFSET_FESR         (FPU_OFFSET_FCR + 4)                    /*  FESR 偏移                   */

/*********************************************************************************************************
  汇编代码不包含以下内容
*********************************************************************************************************/

#if !defined(__ASSEMBLY__) && !defined(ASSEMBLY)

/*********************************************************************************************************
  FPU 访问时需要高 32 位和低 32 位分开访问
*********************************************************************************************************/
typedef struct fpureg {                                                 /*  FPU 寄存器类型              */
    UINT32              val32[FPU_REG_WIDTH / 32];
} ARCH_FPU_REG;

#define ARCH_FPU_CTX_ALIGN      8                                       /*  FPU CTX align size          */

struct arch_fpu_ctx {
    ARCH_FPU_REG        FPUCTX_uiDreg[FPU_REG_NR];                      /*  FPU 通用寄存器              */
    UINT32              FPUCTX_uiFpcr;                                  /*  FPU 控制寄存器              */
    UINT32              FPUCTX_uiFpesr;                                 /*  FPU 异常寄存器              */
   
} __attribute__ ((aligned(ARCH_FPU_CTX_ALIGN)));

typedef struct arch_fpu_ctx     ARCH_FPU_CTX;

/*********************************************************************************************************
  float 格式 (使用 union 类型作为中间转换, 避免 GCC 3.x.x strict aliasing warning)
*********************************************************************************************************/

#define __ARCH_FLOAT_EXP_NAN           255                              /*  NaN 或者无穷大的 Exp 值     */

#if LW_CFG_CPU_ENDIAN == 0
typedef struct __cpu_float_field {
    unsigned int        frac : 23;
    unsigned int        exp  :  8;
    unsigned int        sig  :  1;
} __CPU_FLOAT_FIELD;
#else
typedef struct __cpu_float_field {
    unsigned int        sig  :  1;
    unsigned int        exp  :  8;
    unsigned int        frac : 23;
} __CPU_FLOAT_FIELD;
#endif                                                                  /*  LW_CFG_CPU_ENDIAN           */

typedef union __cpu_float {
    __CPU_FLOAT_FIELD   fltfield;                                       /*  float 位域字段              */
    float               flt;                                            /*  float 占位                  */
} __CPU_FLOAT;

static LW_INLINE INT  __ARCH_FLOAT_ISNAN (float  x)
{
    __CPU_FLOAT     cpuflt;

    cpuflt.flt = x;

    return  ((cpuflt.fltfield.exp == __ARCH_FLOAT_EXP_NAN) && (cpuflt.fltfield.frac != 0));
}

static LW_INLINE INT  __ARCH_FLOAT_ISINF (float  x)
{
    __CPU_FLOAT     cpuflt;

    cpuflt.flt = x;

    return  ((cpuflt.fltfield.exp == __ARCH_FLOAT_EXP_NAN) && (cpuflt.fltfield.frac == 0));
}

/*********************************************************************************************************
  double 格式
*********************************************************************************************************/

#define __ARCH_DOUBLE_EXP_NAN           2047                            /*  NaN 或者无穷大的 Exp 值     */
#define __ARCH_DOUBLE_INC_FLOAT_H          0                            /*  是否引用编译器 float.h 文件 */

/*********************************************************************************************************
  csky-sylixos-elf-gcc ... GNU
*********************************************************************************************************/

#if LW_CFG_CPU_ENDIAN == 0
typedef struct __cpu_double_field {
    unsigned int        fracl : 32;

    unsigned int        frach : 20;
    unsigned int        exp   : 11;
    unsigned int        sig   :  1;
} __CPU_DOUBLE_FIELD;
#else
typedef struct __cpu_double_field {
    unsigned int        sig   :  1;
    unsigned int        exp   : 11;
    unsigned int        frach : 20;

    unsigned int        fracl : 32;
} __CPU_DOUBLE_FIELD;
#endif                                                                  /*  LW_CFG_CPU_ENDIAN           */

typedef union __cpu_double {
    __CPU_DOUBLE_FIELD  dblfield;                                       /*  float 位域字段              */
    double              dbl;                                            /*  float 占位                  */
} __CPU_DOUBLE;

static LW_INLINE INT  __ARCH_DOUBLE_ISNAN (double  x)
{
    __CPU_DOUBLE     dblflt;

    dblflt.dbl = x;

    return  ((dblflt.dblfield.exp == __ARCH_DOUBLE_EXP_NAN) &&
             ((dblflt.dblfield.fracl != 0) &&
              (dblflt.dblfield.frach != 0)));
}

static LW_INLINE INT  __ARCH_DOUBLE_ISINF (double  x)
{
    __CPU_DOUBLE     dblflt;

    dblflt.dbl = x;

    return  ((dblflt.dblfield.exp == __ARCH_DOUBLE_EXP_NAN) &&
             ((dblflt.dblfield.fracl == 0) ||
              (dblflt.dblfield.frach == 0)));
}

#endif                                                                  /*  __ASSEMBLY__                */
#endif                                                                  /*  __SYLIXOS_KERNEL            */
#endif                                                                  /*  __CSKY_ARCH_FLOAT_H         */
/*********************************************************************************************************
  END
*********************************************************************************************************/

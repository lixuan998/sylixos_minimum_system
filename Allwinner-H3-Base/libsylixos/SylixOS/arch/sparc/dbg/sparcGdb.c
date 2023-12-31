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
** 文   件   名: sparcGdb.c
**
** 创   建   人: Jiang.Taijin (蒋太金)
**
** 文件创建日期: 2017 年 10 月 16 日
**
** 描        述: SPARC 体系构架 GDB 调试接口.
*********************************************************************************************************/
#define  __SYLIXOS_STDIO
#define  __SYLIXOS_KERNEL
#include "SylixOS.h"
/*********************************************************************************************************
  裁减配置
*********************************************************************************************************/
#if LW_CFG_GDB_EN > 0
#include "dtrace.h"
#include "arch/arch_gdb.h"
#include "arch/sparc/backtrace/sparcBacktrace.h"
/*********************************************************************************************************
  Xfer:features:read:arch-core.xml 回应包
*********************************************************************************************************/
static const CHAR   _G_cSparcCoreXml[] = \
    "l<?xml version=\"1.0\"?>"
    "<!-- Copyright (C) 2006-2017 ACOINFO co.,ltd."
         "Copying and distribution of this file, with or without modification,"
         "are permitted in any medium without royalty provided the copyright"
         "notice and this notice are preserved.  -->"
    "<!DOCTYPE feature SYSTEM \"gdb-target.dtd\">"
    "<feature name=\"org.gnu.gdb.sparc.core\">"
    "</feature>";
/*********************************************************************************************************
  Xfer:features:read:target.xml 回应包
*********************************************************************************************************/
static const CHAR   _G_cSparcTargetXml[] = \
    "l<?xml version=\"1.0\"?>"
    "<!DOCTYPE target SYSTEM \"gdb-target.dtd\">"
    "<target>"
        "<xi:include href=\"arch-core.xml\"/>"
    "</target>";
/*********************************************************************************************************
  寄存器在数组中的索引
*********************************************************************************************************/
#define GDB_SPARC_PC_INDEX      68
#define GDB_SPARC_NPC_INDEX     69
#define GDB_SPARC_REG_NR        72
/*********************************************************************************************************
** 函数名称: archGdbTargetXml
** 功能描述: 获得 Xfer:features:read:target.xml 回复 XML
** 输　入  : NONE
** 输　出  : 回复 XML
** 全局变量:
** 调用模块:
*********************************************************************************************************/
CPCHAR  archGdbTargetXml (VOID)
{
    return  (_G_cSparcTargetXml);
}
/*********************************************************************************************************
** 函数名称: archGdbCoreXml
** 功能描述: 获得 Xfer:features:read:arch-core.xml 回复 XML
** 输　入  : NONE
** 输　出  : 回复 XML
** 全局变量:
** 调用模块:
*********************************************************************************************************/
CPCHAR  archGdbCoreXml (VOID)
{
    return  (_G_cSparcCoreXml);
}
/*********************************************************************************************************
** 函数名称: archGdbRegsGet
** 功能描述: 获取寄存器值
** 输　入  : pvDtrace       dtrace 节点
**           ulThread       线程句柄
**           pregset        gdb 寄存器结构
** 输　出  : ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  archGdbRegsGet (PVOID  pvDtrace, LW_OBJECT_HANDLE  ulThread, GDB_REG_SET  *pregset)
{
    ARCH_REG_CTX  regctx;
    ARCH_REG_T    regSp;
    INT           i;
    INT           iIndex = 0;

    API_DtraceGetRegs(pvDtrace, ulThread, &regctx, &regSp);

    lib_bzero(pregset, sizeof(GDB_REG_SET));

    pregset->GDBR_iRegCnt = GDB_SPARC_REG_NR;

    for (i = 0; i < 8; i++) {
        pregset->regArr[iIndex++].GDBRA_ulValue = regctx.REG_uiGlobal[i];
    }

    for (i = 0; i < 8; i++) {
        pregset->regArr[iIndex++].GDBRA_ulValue = regctx.REG_uiOutput[i];
    }

    for (i = 0; i < 8; i++) {
        pregset->regArr[iIndex++].GDBRA_ulValue = regctx.REG_uiLocal[i];
    }

    for (i = 0; i < 8; i++) {
        pregset->regArr[iIndex++].GDBRA_ulValue = regctx.REG_uiInput[i];
    }

    for (i = 0; i < 32; i++) {
        pregset->regArr[iIndex++].GDBRA_ulValue = 0;
    }

    pregset->regArr[iIndex++].GDBRA_ulValue = regctx.REG_uiY;
    pregset->regArr[iIndex++].GDBRA_ulValue = regctx.REG_uiPsr;
    iIndex++;
    iIndex++;

    pregset->regArr[iIndex++].GDBRA_ulValue = regctx.REG_uiPc;
    pregset->regArr[iIndex++].GDBRA_ulValue = regctx.REG_uiNPc;

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: archGdbRegsSet
** 功能描述: 设置寄存器值
** 输　入  : pvDtrace       dtrace 节点
**           ulThread       线程句柄
**           pregset        gdb 寄存器结构
** 输　出  : ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  archGdbRegsSet (PVOID  pvDtrace, LW_OBJECT_HANDLE  ulThread, GDB_REG_SET  *pregset)
{
    ARCH_REG_CTX  regctx;
    INT           i;
    INT           iIndex = 0;

    lib_bzero(&regctx, sizeof(ARCH_REG_CTX));

    for (i = 0; i < 8; i++) {
        regctx.REG_uiGlobal[i] = pregset->regArr[iIndex++].GDBRA_ulValue;
    }

    for (i = 0; i < 8; i++) {
        regctx.REG_uiOutput[i] = pregset->regArr[iIndex++].GDBRA_ulValue;
    }

    for (i = 0; i < 8; i++) {
        regctx.REG_uiLocal[i] = pregset->regArr[iIndex++].GDBRA_ulValue;
    }

    for (i = 0; i < 8; i++) {
        regctx.REG_uiInput[i] = pregset->regArr[iIndex++].GDBRA_ulValue;
    }

    iIndex += 32;

    regctx.REG_uiY   = pregset->regArr[iIndex++].GDBRA_ulValue;
    regctx.REG_uiPsr = pregset->regArr[iIndex++].GDBRA_ulValue;
    iIndex++;
    iIndex++;

    regctx.REG_uiPc  = pregset->regArr[iIndex++].GDBRA_ulValue;
    regctx.REG_uiNPc = pregset->regArr[iIndex++].GDBRA_ulValue;

    API_DtraceSetRegs(pvDtrace, ulThread, &regctx);

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: archGdbRegSetPc
** 功能描述: 设置 PC 寄存器值
** 输　入  : pvDtrace       dtrace 节点
**           ulThread       线程句柄
**           ulPC           PC 寄存器值
** 输　出  : ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  archGdbRegSetPc (PVOID  pvDtrace, LW_OBJECT_HANDLE  ulThread, ULONG  ulPc)
{
    ARCH_REG_CTX  regctx;
    ARCH_REG_T    regSp;

    API_DtraceGetRegs(pvDtrace, ulThread, &regctx, &regSp);

    regctx.REG_uiPc = ulPc;

    API_DtraceSetRegs(pvDtrace, ulThread, &regctx);

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: archGdbRegGetPc
** 功能描述: 获取 PC 寄存器值
** 输　入  : pRegs       寄存器数组
** 输　出  : PC 寄存器值
** 全局变量:
** 调用模块:
*********************************************************************************************************/
ULONG  archGdbRegGetPc (GDB_REG_SET  *pRegs)
{
    return  (pRegs->regArr[GDB_SPARC_PC_INDEX].GDBRA_ulValue);
}
/*********************************************************************************************************
** 函数名称: archGdbGetNextPc
** 功能描述: 获取下一条指令地址，含分支预测
** 输　入  : pRegs       寄存器数组
** 输　出  : 下一条指令地址
** 全局变量:
** 调用模块:
*********************************************************************************************************/
ULONG  archGdbGetNextPc (PVOID  pvDtrace, LW_OBJECT_HANDLE  ulThread, GDB_REG_SET  *pRegs)
{
    return  (pRegs->regArr[GDB_SPARC_NPC_INDEX].GDBRA_ulValue);
}
/*********************************************************************************************************
** 函数名称: archGdbGetStepSkip
** 功能描述: 是否忽略此单步断点
** 输　入  : pvDtrace       dtrace 句柄
**           ulThread       被调试线程
**           ulAddr         断点地址
** 输　出  : 是否忽略
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL  archGdbGetStepSkip (PVOID pvDtrace, LW_OBJECT_HANDLE ulThread, addr_t ulAddr)
{
    return  (LW_FALSE);
}

#endif                                                                  /*  LW_CFG_GDB_EN > 0           */
/*********************************************************************************************************
  END
*********************************************************************************************************/

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
** 文   件   名: csky_gdb.h
**
** 创   建   人: Wang.Xuan (王翾)
**
** 文件创建日期: 2018 年 05 月 10 日
**
** 描        述: C-SKY 体系架构 GDB 调试接口.
*********************************************************************************************************/

#ifndef __ARCH_CSKY_GDB_H
#define __ARCH_CSKY_GDB_H

/*********************************************************************************************************
  最大寄存器数
*********************************************************************************************************/

#define GDB_MAX_REG_CNT     128

/*********************************************************************************************************
  GDB 地址类型
*********************************************************************************************************/
typedef ULONG   GDB_ADDR;
typedef UINT64  GDB_ADDR64;

/*********************************************************************************************************
  寄存器集合结构
*********************************************************************************************************/

typedef struct {
    INT         GDBR_iRegCnt;                                           /*  寄存器数量                  */
    struct {
        ULONG   GDBRA_ulValue;                                          /*  寄存器值                    */
    } regArr[GDB_MAX_REG_CNT];                                          /*  寄存器数组                  */
} GDB_REG_SET;

/*********************************************************************************************************
  Xfer:features:read:target.xml 与 Xfer:features:read:mips-core.xml 回应包
*********************************************************************************************************/

CPCHAR  archGdbTargetXml(VOID);

CPCHAR  archGdbCoreXml(VOID);

/*********************************************************************************************************
  gdb 需要的和体系结构相关的功能
*********************************************************************************************************/

INT     archGdbRegsGet(PVOID               pvDtrace,
                       LW_OBJECT_HANDLE    ulThread,
                       GDB_REG_SET        *pregset);                    /*  获取系统寄存器信息          */

INT     archGdbRegsSet(PVOID               pvDtrace,
                       LW_OBJECT_HANDLE    ulThread,
                       GDB_REG_SET        *pregset);                    /*  设置系统寄存器信息          */

INT     archGdbRegSetPc(PVOID              pvDtrace,
                        LW_OBJECT_HANDLE   ulThread,
                        ULONG              uiPc);                       /*  设置 PC 寄存器              */

ULONG   archGdbRegGetPc(GDB_REG_SET       *pRegs);                      /*  获取 PC 寄存器值            */

ULONG   archGdbGetNextPc(PVOID             pvDtrace,
                         LW_OBJECT_HANDLE  ulThread,
                         GDB_REG_SET      *pRegs);                      /*  获取下一个 PC 值，含分支预测*/

BOOL    archGdbGetStepSkip(PVOID            pvDtrace,
                           LW_OBJECT_HANDLE ulThread,
                           addr_t           ulAddr);                    /*  是否忽略本次单步点          */

#endif                                                                  /*  __ARCH_CSKY_GDB_H           */
/*********************************************************************************************************
  END
*********************************************************************************************************/

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
** 文   件   名: loongarch32Mmu.h
**
** 创   建   人: Wang.Ziyang (王子阳)
**
** 文件创建日期: 2022 年 03 月 04 日
**
** 描        述: LoongArch32 体系构架 MMU 驱动.
*********************************************************************************************************/

#ifndef __ARCH_LOONGARCH32MMU_H
#define __ARCH_LOONGARCH32MMU_H

VOID  loongarch32MmuInit(LW_MMU_OP  *pmmuop, CPCHAR  pcMachineName);

#endif                                                                  /*  __ARCH_LOONGARCH32MMU_H     */
/*********************************************************************************************************
  END
*********************************************************************************************************/

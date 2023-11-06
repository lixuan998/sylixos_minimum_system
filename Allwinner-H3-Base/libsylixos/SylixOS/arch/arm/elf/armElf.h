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
** 文   件   名: armElf.h
**
** 创   建   人: Jiang.Taijin (蒋太金)
**
** 文件创建日期: 2010 年 04 月 17 日
**
** 描        述: 实现 ARM 体系结构的 ELF 文件重定位.
*********************************************************************************************************/

#ifndef __ARCH_ARMELF_H
#define __ARCH_ARMELF_H

#ifdef LW_CFG_CPU_ARCH_ARM                                              /*  ARM 体系结构                */

#define ELF_CLASS       ELFCLASS32
#define ELF_ARCH        EM_ARM

#define PT_ARM_EXIDX    0x70000001                                      /* .ARM.exidx segment           */

#endif                                                                  /*  LW_CFG_CPU_ARCH_ARM         */
#endif                                                                  /*  __ARCH_ARMELF_H             */
/*********************************************************************************************************
  END
*********************************************************************************************************/

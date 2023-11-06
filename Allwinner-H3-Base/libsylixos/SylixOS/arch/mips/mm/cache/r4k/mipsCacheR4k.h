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
** 文   件   名: mipsCacheR4k.h
**
** 创   建   人: Jiao.JinXing (焦进星)
**
** 文件创建日期: 2015 年 12 月 01 日
**
** 描        述: MIPS R4K 体系构架 CACHE 驱动.
*********************************************************************************************************/

#ifndef __ARCH_MIPSCACHER4K_H
#define __ARCH_MIPSCACHER4K_H

VOID  mipsCacheR4kInit(LW_CACHE_OP *pcacheop,
                       CACHE_MODE   uiInstruction,
                       CACHE_MODE   uiData,
                       CPCHAR       pcMachineName);
VOID  mipsCacheR4kReset(CPCHAR  pcMachineName);

#endif                                                                  /*  __ARCH_MIPSCACHER4K_H       */
/*********************************************************************************************************
  END
*********************************************************************************************************/

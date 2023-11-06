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
** 文   件   名: symBsp.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2011 年 08 月 12 日
**
** 描        述: 这里加入 BSP 中应该包含的一些重要的符号.
*********************************************************************************************************/

#ifndef __SYMBSP_H
#define __SYMBSP_H

/*********************************************************************************************************
  裁剪支持
*********************************************************************************************************/
#if LW_CFG_SYMBOL_EN > 0

INT  __symbolAddBsp(VOID);

#endif                                                                  /*  LW_CFG_SYMBOL_EN > 0        */
#endif                                                                  /*  __SYMBSP_H                  */
/*********************************************************************************************************
  END
*********************************************************************************************************/

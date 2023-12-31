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
** 文   件   名: devtree_value.h
**
** 创   建   人: Wang.Xuan (王翾)
**
** 文件创建日期: 2019 年 06 月 24 日
**
** 描        述: 设备树全局变量声明
*********************************************************************************************************/

#ifndef __DEVTREE_VALUE_H
#define __DEVTREE_VALUE_H

/*********************************************************************************************************
  外部引用声明
*********************************************************************************************************/

extern PLW_DEVTREE_NODE           _G_pdtnRoot;
extern PLW_DEVTREE_NODE           _G_pdtnAliases;
extern PLW_DEVTREE_NODE          *_G_ppdtnPhandleCache;
extern UINT32                     _G_uiPhandleCacheMask;

#endif                                                                  /*  __DEVTREE_VALUE_H           */
/*********************************************************************************************************
  END
*********************************************************************************************************/

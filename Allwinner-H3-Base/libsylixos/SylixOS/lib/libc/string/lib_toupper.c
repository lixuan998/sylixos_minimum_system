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
** 文   件   名: lib_toupper.c
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2006 年 12 月 25 日
**
** 描        述: 库
*********************************************************************************************************/
#include "ctype.h"
/*********************************************************************************************************
** 函数名称: lib_toupper
** 功能描述: 
** 输　入  : 
** 输　出  : 
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
INT  lib_toupper (INT  iC)
{    
    return  ((_toupper_tab_ + 1)[(UINT8)iC]);
}
/*********************************************************************************************************
  END
*********************************************************************************************************/

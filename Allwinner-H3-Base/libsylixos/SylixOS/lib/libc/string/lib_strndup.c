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
** 文   件   名: lib_strndup.c
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2006 年 12 月 25 日
**
** 描        述: 库
*********************************************************************************************************/
#include "../SylixOS/kernel/include/k_kernel.h"
/*********************************************************************************************************
** 函数名称: lib_strndup
** 功能描述: 
** 输　入  : 
** 输　出  : 
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
PCHAR  lib_strndup (CPCHAR pcStr, size_t  stSize)
{
    size_t  stLen;
    PCHAR   pcNew;
    
    if (pcStr == LW_NULL) {
        return  (LW_NULL);
    }
    
    stLen = lib_strnlen(pcStr, stSize);
    pcNew = (PCHAR)lib_malloc(stLen + 1);
    if (pcNew == LW_NULL) {
        return  (LW_NULL);
    }
    
    pcNew[stLen] = PX_EOS;
    
    return  (lib_memcpy(pcNew, pcStr, stLen));
}
/*********************************************************************************************************
  END
*********************************************************************************************************/

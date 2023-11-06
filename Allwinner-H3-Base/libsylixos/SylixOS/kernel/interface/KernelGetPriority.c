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
** 文   件   名: KernelGetPriority.c
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2007 年 05 月 11 日
**
** 描        述: 用户可以调用这个 API 获得内核最低优先级，即空闲线程优先级
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "../SylixOS/kernel/include/k_kernel.h"
/*********************************************************************************************************
** 函数名称: API_KernelGetPriorityMax
** 功能描述: 获得内核最低优先级
** 输　入  : NONE
** 输　出  : 空闲线程优先级
** 全局变量: 
** 调用模块: 
                                           API 函数
*********************************************************************************************************/
LW_API  
UINT8  API_KernelGetPriorityMax (VOID)
{
    return  (LW_PRIO_LOWEST);
}
/*********************************************************************************************************
** 函数名称: API_KernelGetPriorityMin
** 功能描述: 获得内核最高优先级，
** 输　入  : NONE
** 输　出  : __HIGHEST_PRIO
** 全局变量: 
** 调用模块: 
                                           API 函数
*********************************************************************************************************/
LW_API  
UINT8  API_KernelGetPriorityMin (VOID)
{
    return  (LW_PRIO_HIGHEST);
}
/*********************************************************************************************************
  END
*********************************************************************************************************/

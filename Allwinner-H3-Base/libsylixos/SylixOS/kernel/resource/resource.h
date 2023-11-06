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
** 文   件   名: resource.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2012 年 12 月 06 日
**
** 描        述: 资源管理器
*********************************************************************************************************/

#ifndef __RESOURCE_H
#define __RESOURCE_H

/*********************************************************************************************************
  原始资源类型
*********************************************************************************************************/

typedef struct {
    LW_LIST_LINE            RESRAW_lineManage;                          /*  管理链表                    */
    pid_t                   RESRAW_pid;                                 /*  进程号                      */
    PVOID                   RESRAW_pvArg[6];                            /*  最多 6 个参数               */
    VOIDFUNCPTR             RESRAW_pfuncFree;                           /*  释放函数                    */
    BOOL                    RESRAW_bIsInstall;                          /*  是否安装成功                */
} LW_RESOURCE_RAW;
typedef LW_RESOURCE_RAW    *PLW_RESOURCE_RAW;

/*********************************************************************************************************
  资源操作
*********************************************************************************************************/

#ifdef __SYLIXOS_KERNEL
#include "resourceLib.h"
#endif                                                                  /*  __SYLIXOS_KERNEL            */

#endif                                                                  /*  __RESOURCE_H                */
/*********************************************************************************************************
  END
*********************************************************************************************************/

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
** 文   件   名: tpsfs_sylixos.h
**
** 创   建   人: Jiang.Taijin (蒋太金)
**
** 文件创建日期: 2015 年 9 月 21 日
**
** 描        述: tpsfs SylixOS FS 接口.

** BUG:
*********************************************************************************************************/

#ifndef __TPSFS_SYLIXOS_H
#define __TPSFS_SYLIXOS_H

/*********************************************************************************************************
  裁剪宏
*********************************************************************************************************/
#if LW_CFG_TPSFS_EN > 0

/*********************************************************************************************************
  API
*********************************************************************************************************/
LW_API INT      API_TpsFsDrvInstall(VOID);
LW_API INT      API_TpsFsDevCreate(PCHAR   pcName, PLW_BLK_DEV  pblkd);
LW_API INT      API_TpsFsDevDelete(PCHAR   pcName);

#define tpsFsDrv                API_TpsFsDrvInstall
#define tpsFsDevCreate          API_TpsFsDevCreate
#define tpsFsDevDelete          API_TpsFsDevDelete

#endif                                                                  /* LW_CFG_TPSFS_EN > 0          */
#endif                                                                  /* __TPSFS_SYLIXOS_H            */
/*********************************************************************************************************
  END
*********************************************************************************************************/

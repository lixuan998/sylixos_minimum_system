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
** 文   件   名: random.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2020 年 10 月 04 日
**
** 描        述: 随机数发生器.
*********************************************************************************************************/

#ifndef __SYS_RANDOM_H
#define __SYS_RANDOM_H

#include "fcntl.h"
#include "stdlib.h"

/*********************************************************************************************************
  random flags
*********************************************************************************************************/
#if LW_CFG_DEVICE_EN > 0

#define GRND_NONBLOCK   O_NONBLOCK
#define GRND_RANDOM     O_SYNC

/*********************************************************************************************************
  random API
*********************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif                                                                  /*  __cplusplus                 */

ssize_t getrandom(void *buf, size_t buflen, unsigned int flags);
int     getentropy(void *buf, size_t buflen);

#ifdef __cplusplus
}
#endif                                                                  /*  __cplusplus                 */

#endif                                                                  /*  LW_CFG_DEVICE_EN > 0        */
#endif                                                                  /*  __SYS_RANDOM_H              */
/*********************************************************************************************************
  END
*********************************************************************************************************/

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
** 文   件   名: lwip_vnd.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2017 年 12 月 24 日
**
** 描        述: 虚拟网络设备接口.
*********************************************************************************************************/

#ifndef __LWIP_VND_H
#define __LWIP_VND_H

/*********************************************************************************************************
  裁剪控制
*********************************************************************************************************/
#if LW_CFG_NET_EN > 0 && LW_CFG_NET_VNETDEV_EN > 0

#ifdef __SYLIXOS_KERNEL
INT  _netVndInit(VOID);
#endif                                                                  /*  __SYLIXOS_KERNEL            */

#endif                                                                  /*  LW_CFG_NET_EN               */
                                                                        /*  LW_CFG_NET_VNETDEV_EN       */
#endif                                                                  /*  __LWIP_VND_H                */
/*********************************************************************************************************
  END
*********************************************************************************************************/

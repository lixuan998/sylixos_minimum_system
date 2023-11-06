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
** 文   件   名: keyboard.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2010 年 09 月 09 日
**
** 描        述: 标准键盘驱动.
** 注        意: 这里仅是一个设备驱动的接口规范, 所以不包含在 SylixOS.h 头文件中.
                 鼠标和键盘的读取 read() 不能产生阻塞, 如果没有事件产生, 就立即返回读取失败.
                 对鼠标和键盘的事件阻塞必须通过 select() 完成.
*********************************************************************************************************/

#include <system/device/input/keyboard.h>

/*********************************************************************************************************
  END
*********************************************************************************************************/

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
** 文   件   名: cskyParam.h
**
** 创   建   人: Wang.Xuan (王翾)
**
** 文件创建日期: 2018 年 05 月 11 日
**
** 描        述: C-SKY 体系架构启动参数.
*********************************************************************************************************/

#ifndef __ARCH_CSKYPARAM_H
#define __ARCH_CSKYPARAM_H

/*********************************************************************************************************
  启动参数
*********************************************************************************************************/

typedef struct {
    BOOL        CP_bUnalign;                                            /*  是否支持非对齐访问          */
    BOOL        CP_bMmuEnByBoot;                                        /*  BOOT 是否已经启动了 MMU     */
    BOOL        CP_bSLDepCache;                                         /*  自旋锁是否依赖 CACHE        */
    BOOL        CP_bAPGR;                                               /*  硬件是否存在可选通用寄存器  */
    ULONG       CP_ulBootCpuId;                                         /*  启动的 CPU 核心 ID 号       */
} CSKY_PARAM;

/*********************************************************************************************************
  获取启动参数
*********************************************************************************************************/

CSKY_PARAM  *archKernelParamGet(VOID);

#endif                                                                  /*  __ARCH_CSKYPARAM_H          */
/*********************************************************************************************************
  END
*********************************************************************************************************/

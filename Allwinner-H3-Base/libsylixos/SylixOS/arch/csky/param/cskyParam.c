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
** 文   件   名: cskyParam.c
**
** 创   建   人: Wang.Xuan (王翾)
**
** 文件创建日期: 2018 年 05 月 11 日
**
** 描        述: C-SKY 体系架构启动参数.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "SylixOS.h"
#include "cskyParam.h"
/*********************************************************************************************************
  启动参数
*********************************************************************************************************/
static CSKY_PARAM    cskyParam = { LW_TRUE, LW_TRUE, LW_TRUE, LW_TRUE, 0ul };
/*********************************************************************************************************
** 函数名称: archKernelParam
** 功能描述: C-SKY 体系架构启动参数设置.
** 输　入  : pcParam       启动参数
**                         unalign=yes      是否支持非对齐访问
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID  archKernelParam (CPCHAR  pcParam)
{
    if (lib_strncmp(pcParam, "unalign=", 8) == 0) {                     /*  是否支持非对齐访问          */
        if (pcParam[8] == 'n') {
            cskyParam.CP_bUnalign = LW_FALSE;
        } else {
            cskyParam.CP_bUnalign = LW_TRUE;
        }

    } else if (lib_strncmp(pcParam, "mmuenbyboot=", 12) == 0) {         /*  BOOT 是否已经启动了 MMU     */
        if (pcParam[12] == 'n') {
            cskyParam.CP_bMmuEnByBoot = LW_FALSE;
        } else {
            cskyParam.CP_bMmuEnByBoot = LW_TRUE;
        }
#if LW_CFG_SMP_EN > 0
    } else if (lib_strncmp(pcParam, "sldepcache=", 11) == 0) {          /*  自旋锁是否依赖 CACHE        */
        if (pcParam[11] == 'n') {
            cskyParam.CP_bSLDepCache = LW_FALSE;
        } else {
            cskyParam.CP_bSLDepCache = LW_TRUE;
            __ARCH_SPIN_BYPASS();
        }
#endif                                                                  /*  LW_CFG_SMP_EN > 0           */
    } else if (lib_strncmp(pcParam, "agpr=", 5) == 0) {                 /*  硬件是否存在可选通用寄存器  */
        if (pcParam[5] == 'n') {
            cskyParam.CP_bAPGR = LW_FALSE;
#if !defined(__SYLIXOS_CSKY_ARCH_CK803__)
            KN_FIQ_AUTO_ENTRY_SET();
#endif
        } else {
            cskyParam.CP_bAPGR = LW_TRUE;
        }

    } else if (lib_strncmp(pcParam, "bootcpuid=", 10) == 0) {           /*  启动的 CPU 核心 ID 号       */
        LONG   lBootCpuId = lib_atol(&pcParam[10]);

        if ((lBootCpuId >= 0) && (lBootCpuId < LW_CFG_MAX_PROCESSORS)) {
            cskyParam.CP_ulBootCpuId = lBootCpuId;
        } else {
            cskyParam.CP_ulBootCpuId = 0ul;
        }
    }
}
/*********************************************************************************************************
** 函数名称: archKernelParamGet
** 功能描述: 获取启动参数.
** 输　入  : NONE
** 输　出  : 参数
** 全局变量:
** 调用模块:
*********************************************************************************************************/
CSKY_PARAM  *archKernelParamGet (VOID)
{
    return  (&cskyParam);
}
/*********************************************************************************************************
  END
*********************************************************************************************************/

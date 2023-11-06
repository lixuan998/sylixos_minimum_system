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
** 文   件   名: mips64.c
**
** 创   建   人: Jiao.JinXing (焦进星)
**
** 文件创建日期: 2016 年 11 月 02 日
**
** 描        述: MIPS64 体系架构相关函数库.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "SylixOS.h"
#include "mips64.h"
/*********************************************************************************************************
** 函数名称: mips64MemDup
** 功能描述: 复制 64 位空间内存内容到 32 位空间
** 输　入  : ui64Addr        64 位空间内存地址
**           stLen           长度
** 输　出  : 32 位空间内存地址
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PCHAR  mips64MemDup (UINT64  ui64Addr, size_t  stLen)
{
    PCHAR   pcBuffer;

    pcBuffer = __SHEAP_ALLOC(stLen);
    if (pcBuffer) {
#if LW_CFG_CPU_WORD_LENGHT == 32
        size_t  i;
        PCHAR   pcPtr = pcBuffer;

        for (i = 0; i < stLen; i++) {
            *pcPtr++ = mips64Read8(ui64Addr++);
        }
#else
        lib_memcpy(pcBuffer, (CPVOID)ui64Addr, stLen);
#endif                                                                  /*  LW_CFG_CPU_WORD_LENGHT == 32*/
    }

    return  (pcBuffer);
}
/*********************************************************************************************************
** 函数名称: mips64MemFree
** 功能描述: 释放 mips64MemDup 分配的内存
** 输　入  : pcBuffer      mips64MemDup 分配的内存
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID  mips64MemFree (PCHAR  pcBuffer)
{
    __SHEAP_FREE(pcBuffer);
}
/*********************************************************************************************************
  END
*********************************************************************************************************/

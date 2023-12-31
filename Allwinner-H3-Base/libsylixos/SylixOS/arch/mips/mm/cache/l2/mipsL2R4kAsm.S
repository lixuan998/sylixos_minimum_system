;/*********************************************************************************************************
;**
;**                                    中国软件开源组织
;**
;**                                   嵌入式实时操作系统
;**
;**                                       SylixOS(TM)
;**
;**                               Copyright  All Rights Reserved
;**
;**--------------文件信息--------------------------------------------------------------------------------
;**
;** 文   件   名: mipsL2R4kAsm.S
;**
;** 创   建   人: Jiao.JinXing (焦进星)
;**
;** 文件创建日期: 2017 年 07 月 18 日
;**
;** 描        述: MIPS R4K 体系构架 L2-CACHE 驱动.
;*********************************************************************************************************/

#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>
#include <config/kernel/cache_cfg.h>
#include <config/cpu/cpu_cfg_mips.h>

#if LW_CFG_CACHE_EN > 0 && LW_CFG_MIPS_CACHE_L2 > 0

    EXPORT_LABEL(mipsL2R4kLineFlush)
    EXPORT_LABEL(mipsL2R4kLineClear)
    EXPORT_LABEL(mipsL2R4kLineInvalidate)
    EXPORT_LABEL(mipsL2R4kIndexClear)
    EXPORT_LABEL(mipsL2R4kIndexStoreTag)

    FILE_BEGIN()

;/*********************************************************************************************************
;  回写一个 SCACHE 行
;*********************************************************************************************************/

FUNC_DEF(mipsL2R4kLineFlush)
    CACHE   Hit_Writeback_SD , 0(A0)
    JR      RA
    NOP
    FUNC_END(mipsL2R4kLineFlush)

;/*********************************************************************************************************
;  回写并无效一个 SCACHE 行
;*********************************************************************************************************/

FUNC_DEF(mipsL2R4kLineClear)
    CACHE   Hit_Writeback_Inv_S , 0(A0)
    JR      RA
    NOP
    FUNC_END(mipsL2R4kLineClear)

;/*********************************************************************************************************
;  无效一个 SCACHE 行
;*********************************************************************************************************/

FUNC_DEF(mipsL2R4kLineInvalidate)
    CACHE   Hit_Invalidate_S , 0(A0)
    JR      RA
    NOP
    FUNC_END(mipsL2R4kLineInvalidate)

;/*********************************************************************************************************
;  通过 CACHE 存储器的地址索引回写并无效一个 SCACHE 行
;*********************************************************************************************************/

FUNC_DEF(mipsL2R4kIndexClear)
    CACHE   Index_Writeback_Inv_S , 0(A0)
    JR      RA
    NOP
    FUNC_END(mipsL2R4kIndexClear)

;/*********************************************************************************************************
;  设置 SCACHE 的 TAG
;*********************************************************************************************************/

FUNC_DEF(mipsL2R4kIndexStoreTag)
    CACHE   Index_Store_Tag_S , 0(A0)
    JR      RA
    NOP
    FUNC_END(mipsL2R4kIndexStoreTag)

    FILE_END()

#endif
;/*********************************************************************************************************
;  END
;*********************************************************************************************************/

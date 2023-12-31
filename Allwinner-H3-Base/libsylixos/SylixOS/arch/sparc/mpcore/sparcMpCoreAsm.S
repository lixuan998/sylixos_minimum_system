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
;** 文   件   名: sparcMpCoreAsm.S
;**
;** 创   建   人: Xu.Guizhou (徐贵洲)
;**
;** 文件创建日期: 2017 年 7 月 26 日
;**
;** 描        述: SPARC 体系构架多核接口驱动.
;*********************************************************************************************************/

#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>

#if LW_CFG_SMP_EN > 0

    FILE_BEGIN()
    
    IMPORT_LABEL(bspMpInt)
    
    EXPORT_LABEL(archMpCur)
    EXPORT_LABEL(archMpInt)

    WEAK(archMpCur)
    WEAK(archMpInt)

;/*********************************************************************************************************
;  获得当前核 ID
;*********************************************************************************************************/

FUNC_DEF(archMpCur)
    READ_CPUID(%o0)
    RETL
    NOP
    FUNC_END(archMpCur)

;/*********************************************************************************************************
;  产生一次核间中断
;*********************************************************************************************************/

FUNC_DEF(archMpInt)
    BA      bspMpInt
    NOP
    FUNC_END(archMpInt)
    
    FILE_END()

#endif
;/*********************************************************************************************************
;  END
;*********************************************************************************************************/

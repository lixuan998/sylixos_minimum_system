;/*********************************************************************************************************
;**
;**                                    �й�������Դ��֯
;**
;**                                   Ƕ��ʽʵʱ����ϵͳ
;**
;**                                       SylixOS(TM)
;**
;**                               Copyright  All Rights Reserved
;**
;**--------------�ļ���Ϣ--------------------------------------------------------------------------------
;**
;** ��   ��   ��: ppcMmu460RegAsm.S
;**
;** ��   ��   ��: Jiao.JinXing (������)
;**
;** �ļ���������: 2019 �� 08 �� 14 ��
;**
;** ��        ��: PowerPC 460 ��ϵ���� MMU �Ĵ���������.
;*********************************************************************************************************/

#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>
#include <config/kernel/vmm_cfg.h>

#if LW_CFG_VMM_EN > 0

#include "arch/ppc/arch_e500.h"
#include "./ppcMmu460Reg.h"

    FILE_BEGIN()

    EXPORT_LABEL(ppc460MmuGetMMUCR)
    EXPORT_LABEL(ppc460MmuSetMMUCR)
    EXPORT_LABEL(ppc460MmuGetPID)
    EXPORT_LABEL(ppc460MmuSetPID)

;/*********************************************************************************************************
;  SPR GET SET ����
;*********************************************************************************************************/

MACRO_DEF(SET_SPR, spr)
    MTSPR       \spr , R3
    ISYNC
    BLR
    MACRO_END()

MACRO_DEF(GET_SPR, spr)
    MFSPR       R3 , \spr
    ISYNC
    BLR
    MACRO_END()

FUNC_DEF(ppc460MmuGetMMUCR)
    GET_SPR     PPC460_MMUCR
    FUNC_END()

FUNC_DEF(ppc460MmuSetMMUCR)
    SYNC
    SET_SPR     PPC460_MMUCR
    FUNC_END()

FUNC_DEF(ppc460MmuGetPID)
    GET_SPR     PID
    FUNC_END()

FUNC_DEF(ppc460MmuSetPID)
    SYNC
    SET_SPR     PID
    FUNC_END()

    FILE_END()

#endif
;/*********************************************************************************************************
;  END
;*********************************************************************************************************/
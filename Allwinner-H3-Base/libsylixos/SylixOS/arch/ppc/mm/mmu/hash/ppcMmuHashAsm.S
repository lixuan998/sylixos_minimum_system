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
;** ��   ��   ��: ppcMmuHashAsm.S
;**
;** ��   ��   ��: Jiao.JinXing (������)
;**
;** �ļ���������: 2016 �� 01 �� 14 ��
;**
;** ��        ��: PowerPC ��ϵ���� HASH ҳ�� MMU ����.
;*********************************************************************************************************/

#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>
#include <config/kernel/vmm_cfg.h>

#if LW_CFG_VMM_EN > 0

    FILE_BEGIN()

    EXPORT_LABEL(ppcHashMmuEnable)
    EXPORT_LABEL(ppcHashMmuDisable)

    EXPORT_LABEL(ppcHashMmuInvalidateTLBNr)
    EXPORT_LABEL(ppcHashMmuInvalidateTLBEA)

    EXPORT_LABEL(ppcHashMmuSetSDR1)
    EXPORT_LABEL(ppcHashMmuSetSR)

    EXPORT_LABEL(ppcHashMmuGetSRR1)
    EXPORT_LABEL(ppcHashMmuGetDSISR)

    EXPORT_LABEL(ppcHashMmuHashPageTblPteSet)

;/*********************************************************************************************************
;  ʹ�� MMU
;*********************************************************************************************************/

FUNC_DEF(ppcHashMmuEnable)
    MFMSR   R3
    ISYNC
    ORI     R3 , R3 , ARCH_PPC_MSR_DR | ARCH_PPC_MSR_IR
    ISYNC
    MTMSR   R3
    ISYNC
    BLR
    FUNC_END()

;/*********************************************************************************************************
;  ���� MMU
;*********************************************************************************************************/

FUNC_DEF(ppcHashMmuDisable)
    MFMSR   R3
    ISYNC
    ARCH_PPC_MMU_MASK(R3 , R3)
    ISYNC
    MTMSR   R3
    ISYNC
    BLR
    FUNC_END()

;/*********************************************************************************************************
;  ��Ч���� TLB
;  The tlbia instruction is not implemented on 750/E600/745x/E300/603e and when its opcode is encountered,
;  an illegal instruction program exception is generated. To invalidate all entries of both TLBs,
;  64(750/E600/745x)/32(E300/603e) tlbie instructions must be executed,
;  incrementing the value in EA14�CEA19(750/E600/745x)/EA15�CEA19(E300/603e) by one each time.
;*********************************************************************************************************/

FUNC_DEF(ppcHashMmuInvalidateTLBNr)
    ISYNC
    MTCTR   R3                                                          ;/*  CTR = R3                    */
    XOR     R4 , R4 , R4                                                ;/*  R4 = 0                      */

LINE_LABEL(tlbloop)
    ISYNC
    TLBIE   R4, 0
    EIEIO
    TLBSYNC
    SYNC
    ADDI    R4 , R4 , 0x1000                                            ;/*  Increment bits 14-19        */
    BDNZ    tlbloop                                                     ;/*  Dec CTR, branch if CTR != 0 */
    BLR
    FUNC_END()

;/*********************************************************************************************************
;  ��Чָ�� EA �� TLB
;*********************************************************************************************************/

FUNC_DEF(ppcHashMmuInvalidateTLBEA)
    SYNC
    TLBIE   R3, 0
    EIEIO
    TLBSYNC
    SYNC
    BLR
    FUNC_END()

;/*********************************************************************************************************
;  ���� PTE������Ч TLB
;  VOID  ppcHashMmuHashPageTblPteSet(PTE    *pPte,
;                                    UINT32  uiWord0,
;                                    UINT32  uiWord1,
;                                    UINT32  uiEffectiveAddr);
;*********************************************************************************************************/

FUNC_DEF(ppcHashMmuHashPageTblPteSet)
    SYNC
    STW     R5, 4(R3)
    EIEIO
    STW     R4, 0(R3)
    SYNC
    TLBIE   R6, 0
    EIEIO
    TLBSYNC
    SYNC
    BLR
    FUNC_END()

;/*********************************************************************************************************
;  ���� SDR1 �Ĵ���
;*********************************************************************************************************/

FUNC_DEF(ppcHashMmuSetSDR1)
    SYNC
    MTSPR   SDR1 , R3
    ISYNC
    BLR
    FUNC_END()

;/*********************************************************************************************************
;  ���öμĴ���
;  VOID  ppcHashMmuSetSR(UINT32  uiSRn, UINT32  uiValue);
;*********************************************************************************************************/

FUNC_DEF(ppcHashMmuSetSR)
    SYNC
    RLWINM  R3 , R3 , 28 , 0 , 3
    ISYNC
    MTSRIN  R4 , R3
    ISYNC
    BLR
    FUNC_END()

;/*********************************************************************************************************
;  ��� SRR1 �Ĵ���
;*********************************************************************************************************/

FUNC_DEF(ppcHashMmuGetSRR1)
    MFSPR   R3 , SRR1
    ISYNC
    BLR
    FUNC_END()

;/*********************************************************************************************************
;  ��� DSISR �Ĵ���
;*********************************************************************************************************/

FUNC_DEF(ppcHashMmuGetDSISR)
    MFSPR   R3 , DSISR
    ISYNC
    BLR
    FUNC_END()

    FILE_END()

#endif
;/*********************************************************************************************************
;  END
;*********************************************************************************************************/
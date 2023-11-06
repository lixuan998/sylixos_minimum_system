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
;** ��   ��   ��: arm64MmuAsm.S
;**
;** ��   ��   ��: Wang.Xuan (���Q)
;**
;** �ļ���������: 2018 �� 02 �� 27 ��
;**
;** ��        ��: ARM64 ��ϵ�ܹ� MMU ����֧��.
;*********************************************************************************************************/

#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>
#include <config/kernel/vmm_cfg.h>

#if LW_CFG_VMM_EN > 0

    FILE_BEGIN()
    
    EXPORT_LABEL(arm64MmuGetMMFR0)
    EXPORT_LABEL(arm64MmuGetMMFR1)
    EXPORT_LABEL(arm64MmuInvalidateTLB)
    EXPORT_LABEL(arm64MmuInvalidateTLBMVA)
    EXPORT_LABEL(arm64MmuSetTTBR)
    EXPORT_LABEL(arm64MmuGetTTBR0)
    EXPORT_LABEL(arm64MmuEnable)
    EXPORT_LABEL(arm64MmuDisable)
    EXPORT_LABEL(arm64MmuSetTCR)
    EXPORT_LABEL(arm64MmuGetMAIR)
    EXPORT_LABEL(arm64MmuSetMAIR)
    EXPORT_LABEL(arm64MmuGetSCTLR)
    EXPORT_LABEL(arm64MmuAbtFaultAddr)
  
;/*********************************************************************************************************
;  MMU ���Ի�ȡ
;*********************************************************************************************************/

FUNC_DEF(arm64MmuGetMMFR0)
    MRS     X0 , ID_AA64MMFR0_EL1
    RET
    FUNC_END()

FUNC_DEF(arm64MmuGetMMFR1)
    MRS     X0 , ID_AA64MMFR1_EL1
    RET
    FUNC_END()
    
;/*********************************************************************************************************
;  TLB ��ز���
;*********************************************************************************************************/

FUNC_DEF(arm64MmuInvalidateTLB)                                         ;/* ����е���������Ч           */
    TLBI    VMALLE1IS                                                   ;/* TLBIAll, EL1, IS            */
    ARM_DSB()
    ARM_ISB()
    RET
    FUNC_END()
    
FUNC_DEF(arm64MmuInvalidateTLBMVA)
    LSR     X0 , X0 , #12                                               ;/* �����ַ�Ƴ�ȡ�� 12 λ       */
    TLBI    VAE1IS , X0                                                 ;/* TLBIVA, All ASID, EL1, IS   */
    ARM_DSB()
    ARM_ISB()
    RET
    FUNC_END()
    
;/*********************************************************************************************************
;  This API set TTBR register
;*********************************************************************************************************/

FUNC_DEF(arm64MmuSetTTBR)
    MSR     TTBR0_EL1 , X0
    MSR     TTBR1_EL1 , X0
    RET
    FUNC_END()
    
FUNC_DEF(arm64MmuGetTTBR0)
    MRS     X0 , TTBR0_EL1
    RET
    FUNC_END()

;/*********************************************************************************************************
;  ���������
;*********************************************************************************************************/

FUNC_DEF(arm64MmuGetMAIR)
    MRS     X0 , MAIR_EL1
    RET
    FUNC_END()
    
;/*********************************************************************************************************
;  ����������
;*********************************************************************************************************/

#define MAIR(attr, mt)      ((attr) << ((mt) * 8))
#define MT_DEVICE_nGnRnE    0
#define MT_DEVICE_nGnRE     1
#define MT_DEVICE_nGRE      2
#define MT_DEVICE_GRE       3
#define MT_NORMAL_NC        4
#define MT_NORMAL           5
#define MT_NORMAL_WT        6

FUNC_DEF(arm64MmuSetMAIR)
    LDR     X0 , =MAIR(0x00, MT_DEVICE_nGnRnE) | \
                  MAIR(0x04, MT_DEVICE_nGnRE)  | \
                  MAIR(0x08, MT_DEVICE_nGRE)   | \
                  MAIR(0x0c, MT_DEVICE_GRE)    | \
                  MAIR(0x44, MT_NORMAL_NC)     | \
                  MAIR(0xff, MT_NORMAL)        | \
                  MAIR(0xbb, MT_NORMAL_WT)
    MSR     MAIR_EL1 , X0
    RET
    FUNC_END()

;/*********************************************************************************************************
;  ���� MMU
;*********************************************************************************************************/

FUNC_DEF(arm64MmuSetTCR)
    MSR     TCR_EL1 , X0
    RET
    FUNC_END()
    
FUNC_DEF(arm64MmuGetSCTLR)
    MRS     X0 , SCTLR_EL1
    RET
    FUNC_END()
    
;/*********************************************************************************************************
;  ʹ�� MMU
;*********************************************************************************************************/

FUNC_DEF(arm64MmuEnable) 
    ARM_DSB()
    ARM_ISB()  
    MRS     X0 , SCTLR_EL1
    ORR     X0 , X0 , #1
    MSR     SCTLR_EL1 , X0
    ARM_DSB()
    ARM_ISB()
    RET
    FUNC_END()
    
FUNC_DEF(arm64MmuDisable)
    ARM_DSB()
    ARM_ISB()
    MRS     X0 , SCTLR_EL1
    BIC     X0 , X0 , #1
    MSR     SCTLR_EL1 , X0
    ARM_DSB()
    ARM_ISB()
    RET
    FUNC_END()
    
;/*********************************************************************************************************
;  MMU �쳣��ַ��ȡ
;*********************************************************************************************************/

FUNC_DEF(arm64MmuAbtFaultAddr)
    MRS     X0 , FAR_EL1
    RET
    FUNC_END()

    FILE_END()

#endif
;/*********************************************************************************************************
;  END
;*********************************************************************************************************/
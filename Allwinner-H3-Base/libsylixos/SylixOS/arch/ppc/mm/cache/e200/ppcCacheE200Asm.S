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
;** ��   ��   ��: ppcCacheE200Asm.S
;**
;** ��   ��   ��: Jiao.JinXing (������)
;**
;** �ļ���������: 2016 �� 05 �� 03 ��
;**
;** ��        ��: PowerPC E200 ��ϵ���� CACHE ����.
;*********************************************************************************************************/

#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>
#include <config/kernel/cache_cfg.h>

#if LW_CFG_CACHE_EN > 0

#define  __SYLIXOS_PPC_E200__
#include "arch/ppc/arch_e500.h"

    EXPORT_LABEL(ppcE200CacheEnable)
    EXPORT_LABEL(ppcE200CacheDisable)

    EXPORT_LABEL(ppcE200ICacheInvalidate)

    EXPORT_LABEL(ppcE200DCacheInvalidate)
    EXPORT_LABEL(ppcE200DCacheClear)
    EXPORT_LABEL(ppcE200DCacheFlush)

    EXPORT_LABEL(ppcE200TextUpdate)

    EXPORT_LABEL(ppcE200BranchPredictionDisable)
    EXPORT_LABEL(ppcE200BranchPredictionEnable)
    EXPORT_LABEL(ppcE200BranchPredictorInvalidate)

    EXPORT_LABEL(ppcE200CacheGetL1CFG0)

    IMPORT_LABEL(PPC_E200_CACHE_CNWAY)
    IMPORT_LABEL(PPC_E200_CACHE_SETS)

    FILE_BEGIN()

;/*********************************************************************************************************
;  ��� L1CFG0 �Ĵ���
;*********************************************************************************************************/

FUNC_DEF(ppcE200CacheGetL1CFG0)
    MFSPR   R3 , L1CFG0
    BLR
    FUNC_END()

;/*********************************************************************************************************
;  ʹ�� CACHE
;*********************************************************************************************************/

FUNC_DEF(ppcE200CacheEnable)
    MFSPR   R4 , L1CSR0                                         ;/*  move L1CSR0 to R4                   */
    CMPWI   R4 , ARCH_PPC_L1CSR_CE
    BEQ     ppcE200CacheRetOK                                   ;/*  enabled just return OK              */

    LI      R4 , 0
    MSYNC
    ISYNC
    MTSPR   L1CSR0 , R4
    ISYNC

    ;/* flash invalidate then enable */
    ORI     R4 , R4 , ARCH_PPC_L1CSR_CINV
    MSYNC
    ISYNC
    ;/* msync isync reqd before updating L1CSR0 register */
    MTSPR   L1CSR0 , R4                                         ;/*  flash invalidate data cache         */
                                                                ;/*  - 234 CPU cycle                     */
    MSYNC

    ;/* Wait until CINV bit is cleared */
LINE_LABEL(waitFlashInvDone)
    ISYNC
    MFSPR   R4 , L1CSR0
    CMPWI   R4 , ARCH_PPC_L1CSR_CINV
    BEQ     waitFlashInvDone
    CMPWI   R4 , ARCH_PPC_L1CSR_CABT

    BEQ     ppcE200CacheEnable

    LIS     R4 , HI(ARCH_PPC_L1CSR_CWM | ARCH_PPC_L1CSR_CE)     ;/*  Writethrough cache                  */
    ORI     R4 , R4 , LO(ARCH_PPC_L1CSR_CWM | ARCH_PPC_L1CSR_CE)
    ;/* msync isync reqd before updating L1CSR0 register */
    MSYNC
    ISYNC

    MTSPR   L1CSR0 , R4                                         ;/*  enable data cache                   */

    MSYNC
    ISYNC
    B       ppcE200CacheRetOK                                   ;/*  return OK                           */
    FUNC_END()

;/*********************************************************************************************************
;  ���� CACHE
;*********************************************************************************************************/

FUNC_DEF(ppcE200CacheDisable)
    MFSPR   R4 , L1CSR0
    ANDI.   R4 , R4 , ARCH_PPC_L1CSR_CE
    CMPWI   R4 , ARCH_PPC_L1CSR_CE
    BNE     ppcE200CacheRetOK                                   ;/*  not enabled just return OK          */

    LIS     R9 , HIADJ(PPC_E200_CACHE_CNWAY)
    LWZ     R9 , LO(PPC_E200_CACHE_CNWAY)(R9)                   ;/*  R9 - ctr for ways                   */
    LI      R6 , 0x0                                            ;/*  R6 = Way                            */

LINE_LABEL(nextWay)
    LI      R5 , 0x0                                            ;/*  R5 - set                            */
    LIS     R3 , HIADJ(PPC_E200_CACHE_SETS)
    LWZ     R3 , LO(PPC_E200_CACHE_SETS)(R3)                    ;/*  R3 - ctr for sets                   */
    MTCTR   R3                                                  ;/*  Load counter                        */

LINE_LABEL(nextSet)
    SLWI    R7 , R5 , 5                                         ;/*  move set to correct bits            */
    SLWI    R4 , R6 , 24                                        ;/*  move way to correct bits            */
    OR      R4 , R7 , R4                                        ;/*  or together set and way             */
    ORI     R4 , R4 , 0x2                                       ;/*  or in command fl and invalidate     */
    ISYNC
    MSYNC
    MTSPR   L1FINV0 , R4                                        ;/*  flush and invalidate way/set        */
    ADDI    R5 , R5 , 1                                         ;/*  increment set                       */
    BDNZ    nextSet                                             ;/*  redo until all sets done            */
    ADDI    R6 , R6 , 1                                         ;/*  increment way                       */
    CMPW    R6 , R9                                             ;/*  compare with max                    */
    BNE     nextWay                                             ;/*  redo until max reached              */

    LIS     R4 , HIADJ(~ARCH_PPC_L1CSR_CE)
    ADDI    R4 , R4 , LO(~ARCH_PPC_L1CSR_CE)
    AND     R5 , R5 , R4                                        ;/*  Mask out the cache enable bit       */
    ;/* msync isync reqd before updating L1CSR0 register */
    ISYNC
    MSYNC
    MTSPR   L1CSR0 , R5                                         ;/*  disable the data cache              */
    B       ppcE200CacheRetOK                                   ;/*  return OK                           */
    FUNC_END()

;/*********************************************************************************************************
;  ��Ч ICACHE R3=void *start; R4=void *end; R5=cache line size
;*********************************************************************************************************/

FUNC_DEF(ppcE200ICacheInvalidate)
    XOR     R0 , R0 , R0
    ICBI    R0 , R3
    ADD     R3 , R3 , R5
    CMPLW   R3 , R4
    BLT     ppcE200ICacheInvalidate

    B       ppcE200CacheRetOK                                   ;/*  return OK                           */
    FUNC_END()

;/*********************************************************************************************************
;  ��ָ�������ַ�� DCACHE ��Ч  R3=void *start; R4=void *end; R5=cache line size
;*********************************************************************************************************/

FUNC_DEF(ppcE200DCacheInvalidate)
    XOR     R0 , R0 , R0
    DCBI    R0 , R3
    ADD     R3 , R3 , R5
    CMPLW   R3 , R4
    BLT     ppcE200DCacheInvalidate

    B       ppcE200CacheRetOK                                   ;/*  return OK                           */
    FUNC_END()

;/*********************************************************************************************************
;  ��ָ�������ַ�� DCACHE ��д����Ч  R3=void *start; R4=void *end; R5=cache line size
;*********************************************************************************************************/

FUNC_DEF(ppcE200DCacheClear)
    XOR     R0 , R0 , R0
    DCBF    R0 , R3                                             ;/*  PPC "flush" == SylixOS "clear"      */
    ADD     R3 , R3 , R5
    CMPLW   R3 , R4
    BLT     ppcE200DCacheClear

    B       ppcE200CacheRetOK                                   ;/*  return OK                           */
    FUNC_END()

;/*********************************************************************************************************
;  ��ָ�������ַ�� DCACHE ��д  R3=void *start; R4=void *end; R5=cache line size
;*********************************************************************************************************/

FUNC_DEF(ppcE200DCacheFlush)
    XOR     R0 , R0 , R0
    DCBST   R0 , R3                                             ;/*  PPC "store" == SylixOS "flush"      */
    ADD     R3 , R3 , R5
    CMPLW   R3 , R4
    BLT     ppcE200DCacheFlush

    B       ppcE200CacheRetOK                                   ;/*  return OK                           */
    FUNC_END()

;/*********************************************************************************************************
;  Text update
;*********************************************************************************************************/

FUNC_DEF(ppcE200TextUpdate)
    XOR     R0 , R0 , R0
    DCBST   R0 , R3
    SYNC
    ICBI    R0 , R3
    ADD     R3 , R3 , R6
    CMPLW   R3 , R4
    BLT     ppcE200TextUpdate

    B       ppcE200CacheRetOK                                   ;/*  return OK                           */
    FUNC_END()

;/*********************************************************************************************************
;  ���سɹ�
;*********************************************************************************************************/

FUNC_DEF(ppcE200CacheRetOK)
    ISYNC
    SYNC
    BLR
    FUNC_END()

;/*********************************************************************************************************
;  ���ܷ�֧Ԥ��
;*********************************************************************************************************/

FUNC_DEF(ppcE200BranchPredictionDisable)
    BLR
    FUNC_END()

;/*********************************************************************************************************
;  ʹ�ܷ�֧Ԥ��
;*********************************************************************************************************/

FUNC_DEF(ppcE200BranchPredictionEnable)
    BLR
    FUNC_END()

;/*********************************************************************************************************
;  ��Ч��֧Ԥ��
;*********************************************************************************************************/

FUNC_DEF(ppcE200BranchPredictorInvalidate)
    BLR
    FUNC_END()

    FILE_END()

#endif
;/*********************************************************************************************************
;  END
;*********************************************************************************************************/
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
;** ��   ��   ��: ppcVfpAsm.S
;**
;** ��   ��   ��: Jiao.JinXing (������)
;**
;** �ļ���������: 2015 �� 12 �� 21 ��
;**
;** ��        ��: PowerPC ��ϵ�ܹ� FPU ֧��.
;*********************************************************************************************************/

#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>

#if LW_CFG_CPU_FPU_EN > 0
#include "arch/ppc/arch_float.h"

    FILE_BEGIN()

    EXPORT_LABEL(ppcVfpEnable)
    EXPORT_LABEL(ppcVfpDisable)
    EXPORT_LABEL(ppcVfpIsEnable)
    EXPORT_LABEL(ppcVfpSave)
    EXPORT_LABEL(ppcVfpRestore)

;/*********************************************************************************************************
;  FPU �� MSR �е�ʹ������
;*********************************************************************************************************/

#define ARCH_PPC_MSR_BIT_FP     18

#define FPU_MASK(src, des) \
    RLWINM  des, src, 0, ARCH_PPC_MSR_BIT_FP+1, ARCH_PPC_MSR_BIT_FP-1

;/*********************************************************************************************************
;  ʹ�� FPU ��
;*********************************************************************************************************/

MACRO_DEF(ENABLE_VFP, Rn)
    MFMSR   \Rn                                                         ;/*  ��ȡ MSR                    */
    ISYNC
    ORI     \Rn , \Rn , ARCH_PPC_MSR_FP                                 ;/*  ���� FP λ                  */
    ISYNC
    MTMSR   \Rn                                                         ;/*  ���� MSR                    */
    ISYNC

#ifdef  ARCH_PPC_CCR0_BIT_DAPUIB_U
    ;/*
    ; * ȷ�����ô����� (���� FP) ��ʹ�ܣ�ͨ����� CCR0[DAPUIB] λ�����
    ; */
    MFSPR   \Rn , CCR0
    ISYNC
    RLWINM  \Rn , \Rn , 0 , ARCH_PPC_CCR0_BIT_DAPUIB_U+1 , ARCH_PPC_CCR0_BIT_DAPUIB_U-1
    ISYNC
    MTSPR   CCR0 , \Rn
    ISYNC
#endif                                                                  /*  ARCH_PPC_CCR0_BIT_DAPUIB_U  */
    MACRO_END()

;/*********************************************************************************************************
;  ʹ�� FPU
;*********************************************************************************************************/

FUNC_DEF(ppcVfpEnable)
    ENABLE_VFP  R3
    BLR
    FUNC_END()

;/*********************************************************************************************************
;  �ر� FPU
;*********************************************************************************************************/

FUNC_DEF(ppcVfpDisable)
    MFMSR   R3
    ISYNC
    FPU_MASK(R3 , R3)
    ISYNC
    MTMSR   R3
    ISYNC
    BLR
    FUNC_END()

;/*********************************************************************************************************
;  �ж� FPU �Ƿ�ʹ��
;*********************************************************************************************************/

FUNC_DEF(ppcVfpIsEnable)
    MFMSR   R3
    ISYNC
    RLWINM  R3 , R3 , 0 , ARCH_PPC_MSR_BIT_FP , ARCH_PPC_MSR_BIT_FP
    CMPWI   CR0, R3 , 0
    BNE     CR0, __ppcVfpIsEnable
    BLR
LINE_LABEL(__ppcVfpIsEnable)
    LI      R3 , 1
    BLR
    FUNC_END()

;/*********************************************************************************************************
;  ����Ĵ���
;*********************************************************************************************************/

FUNC_DEF(ppcVfpSave)
    ENABLE_VFP  R4

    STFD    FPR0  ,  XFPR(0)(R3)                                        ;/*  ���� FPR0-FPR31             */
    STFD    FPR1  ,  XFPR(1)(R3)
    STFD    FPR2  ,  XFPR(2)(R3)
    STFD    FPR3  ,  XFPR(3)(R3)
    STFD    FPR4  ,  XFPR(4)(R3)
    STFD    FPR5  ,  XFPR(5)(R3)
    STFD    FPR6  ,  XFPR(6)(R3)
    STFD    FPR7  ,  XFPR(7)(R3)
    STFD    FPR8  ,  XFPR(8)(R3)
    STFD    FPR9  ,  XFPR(9)(R3)
    STFD    FPR10 , XFPR(10)(R3)
    STFD    FPR11 , XFPR(11)(R3)
    STFD    FPR12 , XFPR(12)(R3)
    STFD    FPR13 , XFPR(13)(R3)
    STFD    FPR14 , XFPR(14)(R3)
    STFD    FPR15 , XFPR(15)(R3)
    STFD    FPR16 , XFPR(16)(R3)
    STFD    FPR17 , XFPR(17)(R3)
    STFD    FPR18 , XFPR(18)(R3)
    STFD    FPR19 , XFPR(19)(R3)
    STFD    FPR20 , XFPR(20)(R3)
    STFD    FPR21 , XFPR(21)(R3)
    STFD    FPR22 , XFPR(22)(R3)
    STFD    FPR23 , XFPR(23)(R3)
    STFD    FPR24 , XFPR(24)(R3)
    STFD    FPR25 , XFPR(25)(R3)
    STFD    FPR26 , XFPR(26)(R3)
    STFD    FPR27 , XFPR(27)(R3)
    STFD    FPR28 , XFPR(28)(R3)
    STFD    FPR29 , XFPR(29)(R3)
    STFD    FPR30 , XFPR(30)(R3)
    STFD    FPR31 , XFPR(31)(R3)

    SYNC

    MFFS    FPR28                                                       ;/*  ʹ�� FPR28 ȥ���� FPSCR     */
    ISYNC
    STFD    FPR28 , XFPSCR(R3)                                          ;/*  �� double ���� FPSCR        */
    LWZ     R4    , XFPSCR_COPY(R3)                                     ;/*  �����ʵ�� FPSCR            */
    STW     R4    , XFPSCR(R3)                                          ;/*  ���浽��������λ��          */

    LFD     FPR28 , XFPR(28)(R3)                                        ;/*  �ָ� FPR28                  */
    SYNC

    BLR
    FUNC_END()

;/*********************************************************************************************************
;  �ָ��Ĵ���
;*********************************************************************************************************/

FUNC_DEF(ppcVfpRestore)
    ENABLE_VFP  R4

    LWZ     R4    , XFPSCR(R3)                                          ;/*  ��ȡ�Ѿ������ FPSCR        */
    STW     R4    , XFPSCR_COPY(R3)                                     ;/*  �������� FpscrCopy          */
    LFD     FPR1  , XFPSCR(R3)                                          ;/*  �� double ���� FPSCR        */
    SYNC
    MTFSF   255   , FPR1                                                ;/*  �ָ� FPSCR                  */
    ISYNC

    LFD     FPR0  ,  XFPR(0)(R3)                                        ;/*  �ָ� FPR0-FPR31             */
    LFD     FPR1  ,  XFPR(1)(R3)
    LFD     FPR2  ,  XFPR(2)(R3)
    LFD     FPR3  ,  XFPR(3)(R3)
    LFD     FPR4  ,  XFPR(4)(R3)
    LFD     FPR5  ,  XFPR(5)(R3)
    LFD     FPR6  ,  XFPR(6)(R3)
    LFD     FPR7  ,  XFPR(7)(R3)
    LFD     FPR8  ,  XFPR(8)(R3)
    LFD     FPR9  ,  XFPR(9)(R3)
    LFD     FPR10 , XFPR(10)(R3)
    LFD     FPR11 , XFPR(11)(R3)
    LFD     FPR12 , XFPR(12)(R3)
    LFD     FPR13 , XFPR(13)(R3)
    LFD     FPR14 , XFPR(14)(R3)
    LFD     FPR15 , XFPR(15)(R3)
    LFD     FPR16 , XFPR(16)(R3)
    LFD     FPR17 , XFPR(17)(R3)
    LFD     FPR18 , XFPR(18)(R3)
    LFD     FPR19 , XFPR(19)(R3)
    LFD     FPR20 , XFPR(20)(R3)
    LFD     FPR21 , XFPR(21)(R3)
    LFD     FPR22 , XFPR(22)(R3)
    LFD     FPR23 , XFPR(23)(R3)
    LFD     FPR24 , XFPR(24)(R3)
    LFD     FPR25 , XFPR(25)(R3)
    LFD     FPR26 , XFPR(26)(R3)
    LFD     FPR27 , XFPR(27)(R3)
    LFD     FPR28 , XFPR(28)(R3)
    LFD     FPR29 , XFPR(29)(R3)
    LFD     FPR30 , XFPR(30)(R3)
    LFD     FPR31 , XFPR(31)(R3)
    SYNC

    BLR
    FUNC_END()

    FILE_END()

#endif
;/*********************************************************************************************************
;  END
;*********************************************************************************************************/
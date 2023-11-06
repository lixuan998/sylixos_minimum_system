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
;** ��   ��   ��: riscvExcAsm.S
;**
;** ��   ��   ��: Jiao.JinXing (������)
;**
;** �ļ���������: 2018 �� 03 �� 20 ��
;**
;** ��        ��: RISC-V ��ϵ�����쳣����.
;*********************************************************************************************************/

#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>
#include <config/kernel/kernel_cfg.h>
#include <config/mp/mp_cfg.h>
#include "riscvContextAsm.h"

    FILE_BEGIN()

    IMPORT_LABEL(archTrapHandle)
    EXPORT_LABEL(archTrapEntry)

;/*********************************************************************************************************
;  �쳣�����
;*********************************************************************************************************/

EXCE_DEF(archTrapEntry)
    ;/*
    ; * ʹ���쳣��ʱջ, �����쳣��ʱջ������ʱ�����ı�����, �� volatile �Ĵ������浽��ʱ�����ı�����
    ; */
    EXC_SAVE_VOLATILE

    ;/*
    ; * ���¼��� GP
    ; */
    LA      GP , _gp

    ;/*
    ; * API_InterEnter()
    ; */
    CALL    API_InterEnter                                              ;/*  ��ȡ�жϲ����ŵ�����ֵ RV0  */

    LI      T0 , 1
    BGTU    RV0 , T0 , 1f

    ;/*
    ; * ��õ�ǰ���� TCB �� ARCH_REG_CTX ��ַ
    ; */
    CALL    API_ThreadTcbInter                                          ;/*  get current tcb             */

    ;/*
    ; * ���� volatile �Ĵ�������ǰ���� TCB �� ARCH_REG_CTX ��
    ; */
    EXC_COPY_VOLATILE

    ;/*
    ; * ���� non volatile �Ĵ�������ǰ���� TCB �� ARCH_REG_CTX ��
    ; */
    EXC_SAVE_NON_VOLATILE

    MOVE    S0 , RV0

    ;/*
    ; * ��һ�ν����ж�: ��õ�ǰ CPU �ж϶�ջջ��, ������ SP
    ; */
    CALL    API_InterStackBaseGet

    MOVE    SP , RV0

    MOVE    A0 , S0
2:
    ;/*
    ; * handle(ARCH_REG_CTX ָ��)
    ; */
    CALL    archTrapHandle                                              ;/*  ���ô�������                */

    ;/*
    ; * API_InterExit()
    ; * ���û�з����ж�Ƕ��, �� API_InterExit ����� archIntCtxLoad ����
    ; */
    CALL    API_InterExit

    ;/*
    ; * ��������, ˵���������ж�Ƕ��
    ; */
    MOVE    T0 , SP
    RESTORE_BIG_REG_CTX                                                 ;/*  �ָ����мĴ���              */

1:
    ;/*
    ; * ���ǵ�һ�ν����ж�
    ; */
    REG_L   RV0 , SP_OFFSET(SP)                                         ;/*  �쳣ǰ SP(XSCRATCH)         */
    ADDI    RV0 , RV0 , -ARCH_REG_CTX_SIZE                              ;/*  ���쳣��ջ���������ı�����  */

    ;/*
    ; * ���� volatile �Ĵ������쳣��ջ��������ı�����
    ; */
    EXC_COPY_VOLATILE

    ;/*
    ; * ���� non volatile �Ĵ������쳣��ջ��
    ; */
    EXC_SAVE_NON_VOLATILE

    MOVE    SP , RV0                                                    ;/*  ʹ���쳣��ջ                */
    MOVE    A0 , RV0

    J       2b
    FUNC_END(archTrapEntry)

    FILE_END()

;/*********************************************************************************************************
;  END
;*********************************************************************************************************/
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
;** ��   ��   ��: x86ContextAsm.S
;**
;** ��   ��   ��: Jiao.JinXing (������)
;**
;** �ļ���������: 2016 �� 07 �� 04 ��
;**
;** ��        ��: x86 ��ϵ�����������л�.
;*********************************************************************************************************/

#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>
#include <config/kernel/kernel_cfg.h>
#include <config/mp/mp_cfg.h>
#include "x86ContextAsm.h"

    FILE_BEGIN()

    IMPORT_LABEL(_SchedSwp)
#if LW_CFG_SMP_EN > 0
    IMPORT_LABEL(_SchedSafeStack)
#endif
#if LW_CFG_COROUTINE_EN > 0
    IMPORT_LABEL(_SchedCrSwp)
#endif

    EXPORT_LABEL(archTaskCtxStart)
    EXPORT_LABEL(archTaskCtxSwitch)
#if LW_CFG_COROUTINE_EN > 0
    EXPORT_LABEL(archCrtCtxSwitch)
#endif
    EXPORT_LABEL(archIntCtxLoad)
    EXPORT_LABEL(archSigCtxLoad)

;/*********************************************************************************************************
;  archResumePc
;*********************************************************************************************************/

FUNC_DEF(archResumePc)
    RET
    FUNC_END(archResumePc)

;/*********************************************************************************************************
;  ��ǰ CPU ��ʼ���ж�����ִ��
;  ����Ϊ��ǰ CPU ���ƿ�, �� X86_SP_ARG1 Ϊ��ǰ CPU ���ƿ�ָ��
;  ���� ABI��%EDI �ɱ����ú������棬����ʹ�� %EDI ���浱ǰ CPU ���ƿ�ָ��
;*********************************************************************************************************/

FUNC_DEF(archTaskCtxStart)
    MOVL    X86_SP_ARG1(%ESP) , %EDX                                    ;/*  EDX = ��ǰ CPU ���ƿ�ָ��   */

    MOVL    0(%EDX) , %EAX                                              ;/*  EAX = ��ǰTCB�� REG_CTX ��ַ*/

    ;/*
    ; * һ���ǻָ� RING0 ������
    ; */
    KERN_RESTORE_REGS                                                   ;/*  �ָ����мĴ���              */
    FUNC_END(archTaskCtxStart)

;/*********************************************************************************************************
;  �߳��л� 
;  ����Ϊ��ǰ CPU ���ƿ�, �� X86_SP_ARG1 Ϊ��ǰ CPU ���ƿ�ָ��
;*********************************************************************************************************/

FUNC_DEF(archTaskCtxSwitch)
    MOVL    X86_SP_ARG1(%ESP) , %EDX                                    ;/*  EDX = ��ǰ CPU ���ƿ�ָ��   */

    MOVL    0(%EDX) , %EAX                                              ;/*  EAX = ��ǰTCB�� REG_CTX ��ַ*/

    ;/*
    ; * һ���� RING0
    ; */
    KERN_SAVE_REGS                                                      ;/*  �������мĴ���              */

    MOVL    %EDX , %EDI                                                 ;/*  EDI = ��ǰ CPU ���ƿ�ָ��   */
#if LW_CFG_SMP_EN > 0
    PUSHL   %EDI                                                        ;/*  ���� = ��ǰ CPU ���ƿ�ָ��  */
    CALL    _SchedSafeStack                                             ;/*  _SchedSafeStack();          */
    MOVL    %EAX , %ESP
#endif

    PUSHL   %EDI                                                        ;/*  ���� = ��ǰ CPU ���ƿ�ָ��  */
    CALL    _SchedSwp                                                   ;/*  _SchedSwp();                */
    ADDL    $ARCH_REG_SIZE , %ESP

    MOVL    0(%EDI) , %EAX                                              ;/*  EAX = ��ǰTCB�� REG_CTX ��ַ*/

    ;/*
    ; * �Ժ���ܻ᷵�ص� RING3
    ; */
    KERN_RESTORE_REGS                                                   ;/*  �ָ����мĴ���              */
    FUNC_END(archTaskCtxSwitch)

;/*********************************************************************************************************
;  Э���л� 
;  ����Ϊ��ǰ CPU ���ƿ�, �� X86_SP_ARG1 Ϊ��ǰ CPU ���ƿ�ָ��
;*********************************************************************************************************/

#if LW_CFG_COROUTINE_EN > 0
FUNC_DEF(archCrtCtxSwitch)
    MOVL    X86_SP_ARG1(%ESP) , %EDX                                    ;/*  EDX = ��ǰ CPU ���ƿ�ָ��   */

    MOVL    8(%EDX) , %EAX                                              ;/*  EAX = ��ǰCCB�� REG_CTX ��ַ*/

    KERN_SAVE_REGS                                                      ;/*  �������мĴ���              */

    MOVL    %EDX , %EDI                                                 ;/*  EDI = ��ǰ CPU ���ƿ�ָ��   */

    PUSHL   %EDI                                                        ;/*  ���� = ��ǰ CPU ���ƿ�ָ��  */
    CALL    _SchedCrSwp                                                 ;/*  _SchedCrSwp();              */
    ADDL    $ARCH_REG_SIZE , %ESP

    MOVL    8(%EDI) , %EAX                                              ;/*  EAX = ��ǰCCB�� REG_CTX ��ַ*/

    ;/*
    ; * һ���ǻָ�ͬ��������
    ; */
    KERN_RESTORE_REGS                                                   ;/*  �ָ����мĴ���              */
    FUNC_END(archCrtCtxSwitch)
#endif

;/*********************************************************************************************************
;  �жϷ���ʱ, �߳�װ�� 
;  ����Ϊ��ǰ CPU ���ƿ�, �� X86_SP_ARG1 Ϊ��ǰ CPU ���ƿ�ָ��
;*********************************************************************************************************/

FUNC_DEF(archIntCtxLoad)
    MOVL    X86_SP_ARG1(%ESP) , %EDX                                    ;/*  EDX = ��ǰ CPU ���ƿ�ָ��   */

    MOVL    0(%EDX) , %EAX                                              ;/*  EAX = ��ǰTCB�� REG_CTX ��ַ*/

    ;/*
    ; * �Ժ���ܻ᷵�ص� RING3
    ; */
    KERN_RESTORE_REGS                                                   ;/*  �ָ����мĴ���              */
    FUNC_END(archIntCtxLoad)

;/*********************************************************************************************************
;  �ź������ķ���
;  ����Ϊ ARCH_REG_CTX ָ��
;*********************************************************************************************************/

FUNC_DEF(archSigCtxLoad)
    MOVL    X86_SP_ARG1(%ESP) , %EAX                                    ;/*  EAX = ARCH_REG_CTX ָ��     */

    ;/*
    ; * һ���ǻָ�ͬ��������
    ; */
    KERN_RESTORE_REGS                                                   ;/*  �ָ����мĴ���              */
    FUNC_END(archSigCtxLoad)

    FILE_END()

;/*********************************************************************************************************
;  END
;*********************************************************************************************************/
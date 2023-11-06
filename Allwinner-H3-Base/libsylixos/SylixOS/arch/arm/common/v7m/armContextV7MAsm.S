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
;** ��   ��   ��: armContextV7MAsm.S
;**
;** ��   ��   ��: Jiao.JinXing (������)
;**
;** �ļ���������: 2017 �� 11 �� 14 ��
;**
;** ��        ��: ARMv7M ��ϵ�����������л�.
;*********************************************************************************************************/

#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>
#include <config/kernel/kernel_cfg.h>
#include <config/mp/mp_cfg.h>

#include "arch/arm/arch_regs.h"

#if defined(__SYLIXOS_ARM_ARCH_M__)

#include "armSvcV7M.h"

    FILE_BEGIN()

;/*********************************************************************************************************
;  ARMv7M ��ϵ��������������л�����
;*********************************************************************************************************/

    IMPORT_LABEL(_SchedSwp)
#if LW_CFG_SMP_EN > 0
    IMPORT_LABEL(_SchedSafeStack)
#endif
#if LW_CFG_COROUTINE_EN > 0
    IMPORT_LABEL(_SchedCrSwp)
#endif

    EXPORT_LABEL(archTaskCtxStart)
    EXPORT_LABEL(archTaskCtxSwitch)
    EXPORT_LABEL(archTaskCtxCopy)
#if LW_CFG_COROUTINE_EN > 0
    EXPORT_LABEL(archCrtCtxSwitch)
#endif
    EXPORT_LABEL(archIntCtxLoad)
    EXPORT_LABEL(archSigCtxLoad)

;/*********************************************************************************************************
;  ��ǰ CPU ��ʼ���ж�����ִ��
;  ����Ϊ��ǰ CPU ���ƿ�, �� R0 Ϊ��ǰ CPU ���ƿ�ָ��
;*********************************************************************************************************/

FUNC_DEF(archTaskCtxStart)
#if LW_CFG_CORTEX_M_SVC_SWITCH > 0
    MOV     R1 , #LW_CFG_CORTEX_M_INT_MASK
    MSR     BASEPRI , R1                                        ;/*  ���γ� SVC ����ж�                 */
    MOV     R1 , #SVC_archTaskCtxStart                          ;/*  ʹ�� R1 ���� SVC �����             */
    CPSIE   I                                                   ;/*  ʹ���ж�                            */
    SVC     0
    BX      LR

#else
    LDR     R0 , =CORTEX_M_NVIC_SYSPRI2                         ;/*  ���� PendSV �쳣���ȼ�              */
    LDR     R1 , =CORTEX_M_NVIC_PENDSV_PRI
    STRB    R1 , [R0]

    MOVS    R0 , #0                                             ;/*  ���� PSP Ϊ 0 ��ʶ���һ�������ļ���*/
    MSR     PSP, R0

    LDR     R0 , =API_InterStackBaseGet                         ;/*  ��õ�ǰ CPU �ж϶�ջջ��,������ MSP*/
    BLX     R0
    MSR     MSP , R0

    LDR     R0 , =CORTEX_M_NVIC_INT_CTRL                        ;/*  ���� PendSV �쳣�����������л�      */
    LDR     R1 , =CORTEX_M_NVIC_PENDSVSET
    STR     R1 , [R0]

    CPSIE   I                                                   ;/*  ʹ���ж�                            */
    B       .
#endif
    FUNC_END()

;/*********************************************************************************************************
;  �߳��л�
;  ����Ϊ��ǰ CPU ���ƿ�, �� R0 Ϊ��ǰ CPU ���ƿ�ָ��
;*********************************************************************************************************/

FUNC_DEF(archTaskCtxSwitch)
#if LW_CFG_CORTEX_M_SVC_SWITCH > 0
    MOV     R1 , #SVC_archTaskCtxSwitch                         ;/*  ʹ�� R1 ���� SVC �����             */
    SVC     0

#else
    LDR     R0 , =CORTEX_M_NVIC_INT_CTRL                        ;/*  ���� PendSV �쳣�����������л�      */
    LDR     R1 , =CORTEX_M_NVIC_PENDSVSET
    STR     R1 , [R0]
#endif
    BX      LR
    FUNC_END()

;/*********************************************************************************************************
;  Э���л�
;  ����Ϊ��ǰ CPU ���ƿ�, �� R0 Ϊ��ǰ CPU ���ƿ�ָ��
;*********************************************************************************************************/

#if LW_CFG_COROUTINE_EN > 0
FUNC_DEF(archCrtCtxSwitch)
#if LW_CFG_CORTEX_M_SVC_SWITCH > 0
    MOV     R1 , #SVC_archCrtCtxSwitch                          ;/*  ʹ�� R1 ���� SVC �����             */
    SVC     0
    BX      LR

#else
    #error "No support coroutine when uses PendSV mode for task context switch!"
#endif
    FUNC_END()
#endif

;/*********************************************************************************************************
;  �жϷ���ʱ, �߳�װ��
;  ����Ϊ��ǰ CPU ���ƿ�, �� R0 Ϊ��ǰ CPU ���ƿ�ָ��
;*********************************************************************************************************/

FUNC_DEF(archIntCtxLoad)
#if LW_CFG_CORTEX_M_SVC_SWITCH > 0
    LDR     R0 , [R0]                                           ;/*  ��ȡ��ǰ TCB �� REG_CTX ��ַ        */
    ADD     R12 , R0 , #4                                       ;/*  ������� SP �Ļָ�                */

    LDR     R2 , [R0]                                           ;/*  ���� SP �� R2                       */
    SUB     R2 , R2 , #(ARCH_HW_SAVE_CTX_SIZE)                  ;/*  �� SP �����Զ���ջ�Ĵ����ռ�        */
    MSR     PSP , R2                                            ;/*  ���� PSP                            */

    ADD     R0 , R0 , #(ARCH_SW_SAVE_CTX_SIZE)                  ;/*  R0 ָ�� REG_CTX ���Զ���ջ�Ĵ���    */
    LDM     R0 , {R3-R10}                                       ;/*  �� REG_CTX ���Զ���ջ�Ĵ������Ƶ�   */
    STM     R2 , {R3-R10}                                       ;/*  SP ���Զ���ջ�Ĵ����ռ�             */

    LDM     R12 , {R3-R11, LR}                                  ;/*  �ָ� BASEPRI, R4-R11, EXC_RETURN    */
    MSR     BASEPRI , R3                                        ;/*  �ָ� BASEPRI                        */

    ;/*
    ; * �� SP ָ��Ķ�ջ�ָ� xPSR, PC, LR, R12 �Լ� R3-R0 �Ĵ���
    ; */
    BX      LR

#else
    LDR     R0 , =CORTEX_M_NVIC_INT_CTRL                        ;/*  ���� PendSV �쳣�����������л�      */
    LDR     R1 , =CORTEX_M_NVIC_PENDSVSET
    STR     R1 , [R0]

    BX      LR
#endif
    FUNC_END()

;/*********************************************************************************************************
;  �ź������ķ���
;  ����Ϊ ARCH_REG_CTX ָ��
;*********************************************************************************************************/

FUNC_DEF(archSigCtxLoad)
#if LW_CFG_CORTEX_M_SVC_SWITCH > 0
    MOV     R1 , #SVC_archSigCtxLoad                            ;/*  ʹ�� R1 ���� SVC �����             */
    SVC     0
    BX      LR

#else
    ;/*
    ; * PendSV ��ʽ�����л���֧���ź�, �쳣����Ҳ������ API_VmmAbortIsr
    ; * ���Բ������е�����
    ; */
    B       .
#endif
    FUNC_END()

;/*********************************************************************************************************
;  ��������������
;  ���� R0 ΪĿ�� ARCH_REG_CTX ָ��, R1 ΪԴ ARCH_SW_SAVE_REG_CTX ָ��, R2 ΪԴ ARCH_HW_SAVE_REG_CTX ָ��
;*********************************************************************************************************/

FUNC_DEF(archTaskCtxCopy)
    STMFD   SP! , {R4-R10}

    LDMIA   R1! , {R3-R10}
    STMIA   R0! , {R3-R10}
    LDMIA   R1! , {R3-R5}
    STMIA   R0! , {R3-R5}

    LDMIA   R2! , {R3-R10}
    STMIA   R0! , {R3-R10}

    LDMFD   SP! , {R4-R10}
    BX      LR
    FUNC_END()

    FILE_END()

#endif
;/*********************************************************************************************************
;  END
;*********************************************************************************************************/
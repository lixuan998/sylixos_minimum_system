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
;** ��   ��   ��: armContextAsm.S
;**
;** ��   ��   ��: Han.Hui (����)
;**
;** �ļ���������: 2013 �� 12 �� 07 ��
;**
;** ��        ��: ARM ��ϵ�����������л�.
;*********************************************************************************************************/

#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>
#include <config/kernel/kernel_cfg.h>
#include <config/mp/mp_cfg.h>

#include "arch/arm/arch_regs.h"

#if !defined(__SYLIXOS_ARM_ARCH_M__)

    FILE_BEGIN()

;/*********************************************************************************************************
;  ARM ��ϵ��������쳣��������
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
    LDR     R0 , [R0]                                                   ;/*  ��ȡ��ǰ TCB �� REG_CTX ��ַ*/
    
LINE_LABEL(archTaskCtxLoad)
    LDMIA   R0!, {R2-R4}                                                ;/*  ��ȡ CPSR LR SP             */

    MSR     CPSR_c , #(DIS_INT | SYS32_MODE)                            ;/*  ���� SYS ģʽ, ���ж�       */
    MOV     SP , R4                                                     ;/*  �ָ� SP_sys                 */
    MOV     LR , R3                                                     ;/*  �ָ� LR_sys                 */

    MSR     CPSR_c, #(DIS_INT | SVC32_MODE)                             ;/*  ���� SVC ģʽ, ���ж�       */
    MSR     SPSR_cxsf , R2                                              ;/*  CPSR_sys -> SPSR_svc        */
    LDMIA   R0 , {R0-R12, PC}^                                          ;/*  �ָ����� PC �����мĴ���,   */
                                                                        ;/*  ͬʱ���� CPSR               */
    FUNC_END()

;/*********************************************************************************************************
;  �߳��л� 
;  ����Ϊ��ǰ CPU ���ƿ�, �� R0 Ϊ��ǰ CPU ���ƿ�ָ��
;*********************************************************************************************************/

FUNC_DEF(archTaskCtxSwitch)
    LDR     R1 , [R0]                                                   ;/*  ��ȡ��ǰ TCB                */
    ADD     R1 , R1 , #(ARCH_REG_CTX_SIZE)                              ;/*  ��ǰ TCB �� REG_CTX ���˵�ַ*/

    STMFD   R1!, {LR}                                                   ;/*  ���淵�ص�ַ                */
    STMFD   R1 , {R0-R12}                                               ;/*  ����Ĵ���                  */
    SUB     R1 , R1 , #(13 * 4)                                         ;/*  ���� R1                     */
    STMFD   R1!, {SP}                                                   ;/*  ���� SP                     */
    STMFD   R1!, {LR}                                                   ;/*  ���� LR                     */
    MRS     R2 , CPSR                                                   ;/*  ���� CPSR                   */
    STMFD   R1!, {R2}

    MOV     R9 , R0                                                     ;/*  ���� R0                     */
#if LW_CFG_SMP_EN > 0
    LDR     R1 , =_SchedSafeStack                                       ;/*  _SchedSafeStack();          */
    MOV     LR , PC
    BX      R1
    MOV     SP , R0                                                     ;/*  ���� SP                     */
    MOV     R0 , R9                                                     ;/*  �ָ� R0                     */
#endif

    LDR     R1 , =_SchedSwp                                             ;/*  _SchedSwp();                */
    MOV     LR , PC
    BX      R1
    MOV     R0 , R9                                                     ;/*  �ָ� R0                     */

    B       archTaskCtxStart
    FUNC_END()

;/*********************************************************************************************************
;  Э���л� 
;  ����Ϊ��ǰ CPU ���ƿ�, �� R0 Ϊ��ǰ CPU ���ƿ�ָ��
;*********************************************************************************************************/

#if LW_CFG_COROUTINE_EN > 0
FUNC_DEF(archCrtCtxSwitch)
    ADD     R1 , R0 , #8
    LDR     R2 , [R1]                                                   ;/*  ��ȡ��ǰ CCB                */
    ADD     R1 , R2 , #(ARCH_REG_CTX_SIZE)                              ;/*  ��ǰ CCB �� REG_CTX ���˵�ַ*/

    STMFD   R1!, {LR}                                                   ;/*  ���淵�ص�ַ                */
    STMFD   R1 , {R0-R12}                                               ;/*  ����Ĵ���                  */
    SUB     R1 , R1 , #(13 * 4)                                         ;/*  ���� R1                     */
    STMFD   R1!, {SP}                                                   ;/*  ���� SP                     */
    STMFD   R1!, {LR}                                                   ;/*  ���� LR                     */
    MRS     R2 , CPSR                                                   ;/*  ���� CPSR                   */
    STMFD   R1!, {R2}

    MOV     R9 , R0                                                     ;/*  ���� R0                     */
    LDR     R1 , =_SchedCrSwp                                           ;/*  _SchedCrSwp();              */
    MOV     LR , PC
    BX      R1
    MOV     R0 , R9                                                     ;/*  �ָ� R0                     */

    ADD     R1 , R0 , #8
    LDR     R0 , [R1]                                                   ;/*  ��ȡ��ǰ CCB �� REG_CTX ��ַ*/
    B       archTaskCtxLoad
    FUNC_END()
#endif

;/*********************************************************************************************************
;  �жϷ���ʱ, �߳�װ�� 
;  ����Ϊ��ǰ CPU ���ƿ�, �� R0 Ϊ��ǰ CPU ���ƿ�ָ��
;*********************************************************************************************************/

FUNC_DEF(archIntCtxLoad)
    B       archTaskCtxStart
    FUNC_END()

;/*********************************************************************************************************
;  �ź������ķ���
;  ����Ϊ ARCH_REG_CTX ָ��
;*********************************************************************************************************/

FUNC_DEF(archSigCtxLoad)
    B       archTaskCtxLoad
    FUNC_END()

;/*********************************************************************************************************
;  ��������������
;  ���� R0 ΪĿ�� ARCH_REG_CTX ָ��, R1 ΪԴ ARCH_REG_CTX ָ��
;*********************************************************************************************************/

FUNC_DEF(archTaskCtxCopy)
    STMFD   SP! , {R4-R10}

    LDMIA   R1! , {R2-R10}
    STMIA   R0! , {R2-R10}
    LDMIA   R1! , {R2-R9}
    STMIA   R0! , {R2-R9}

    LDMFD   SP! , {R4-R10}
    BX      LR
    FUNC_END()

    FILE_END()

#endif
;/*********************************************************************************************************
;  END
;*********************************************************************************************************/
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
;** ��   ��   ��: cskyContextCK803Asm.S
;**
;** ��   ��   ��: Wang.Xuan (���Q)
;**
;** �ļ���������: 2019 �� 04 �� 26 ��
;**
;** ��        ��: C-SKY CK803 ��ϵ�ܹ��������л�.
;*********************************************************************************************************/

#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>
#include <config/kernel/kernel_cfg.h>
#include <config/mp/mp_cfg.h>

#if defined(__SYLIXOS_CSKY_ARCH_CK803__)

#include "cskyContextCK803Asm.h"

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
    EXPORT_LABEL(archTaskCtxCopy)
#if LW_CFG_COROUTINE_EN > 0
    EXPORT_LABEL(archCrtCtxSwitch)
#endif
    EXPORT_LABEL(archIntCtxLoad)
    EXPORT_LABEL(archSigCtxLoad)
    EXPORT_LABEL(archExcOldStack)
    EXPORT_LABEL(archExcIrqStack)

;/*********************************************************************************************************
;  �쳣ջ
;*********************************************************************************************************/

    .balign     ARCH_STK_ALIGN_SIZE
LINE_LABEL(archExcOldStack)
    .long       archExcOldStack

LINE_LABEL(archExcIrqStack)
    .long       archExcIrqStack

;/*********************************************************************************************************
;  ��ǰ CPU ��ʼ���ж�����ִ��
;  ����Ϊ��ǰ CPU ���ƿ�, �� A0 Ϊ��ǰ CPU ���ƿ�ָ��
;*********************************************************************************************************/

FUNC_DEF(archTaskCtxStart)
    SUBI        SP , 1024                                               ;/*  ������ջ�����쳣��ʱջ      */
    ANDNI       SP , SP , 0x7                                           ;/*  SP ���� 8 �ֽڶ���          */
    LRW         A1 , archExcIrqStack
    ST.W        SP , (A1)

    LD.W        A1 , (A0 , 0)                                           ;/*  ��ȡ��ǰ TCB �� REG_CTX ��ַ*/

    RESTORE_REGS                                                        ;/*  �ָ��Ĵ���                  */
    FUNC_END(archTaskCtxStart)

;/*********************************************************************************************************
;  �߳��л�
;  ����Ϊ��ǰ CPU ���ƿ�, �� A0 Ϊ��ǰ CPU ���ƿ�ָ��
;*********************************************************************************************************/

FUNC_DEF(archTaskCtxSwitch)
    LD.W        A1 , (A0 , 0)                                           ;/*  ��ȡ��ǰ TCB �� REG_CTX ��ַ*/

    SAVE_REGS                                                           ;/*  ��������Ĵ���              */

    MOV         S0 , A0                                                 ;/*  S0 �ݴ� A0                  */

#if LW_CFG_SMP_EN > 0
    JSRI        _SchedSafeStack                                         ;/*  _SchedSafeStack();          */
    MOV         SP , A0
    MOV         A0 , S0                                                 ;/*  �ָ� A0                     */
#endif

    JSRI        _SchedSwp                                               ;/*  _SchedSwp();                */
    
    LD.W        A1 , (S0 , 0)                                           ;/*  ��ȡ��ǰ TCB �� REG_CTX ��ַ*/
    
    RESTORE_REGS                                                        ;/*  �ָ�����Ĵ���              */
    FUNC_END(archTaskCtxSwitch)

;/*********************************************************************************************************
;  Э���л�
;  ����Ϊ��ǰ CPU ���ƿ�, �� A0 Ϊ��ǰ CPU ���ƿ�ָ��
;*********************************************************************************************************/

#if LW_CFG_COROUTINE_EN > 0
FUNC_DEF(archCrtCtxSwitch)
    LD.W        A1 , (A0 , (2 * PTRSIZE))                               ;/*  ��ǰ CCB �� REG_CTX ��ַ    */
    
    SAVE_REGS                                                           ;/*  ����Э�̼Ĵ���              */
    
    MOV         S0 , A0                                                 ;/*  S0 �ݴ� A0                  */

    JSRI        _SchedCrSwp                                             ;/*  _SchedCrSwp();              */
    
    LD.W        A1 , (S0 , (2 * PTRSIZE))                               ;/*  ��ǰ CCB �� REG_CTX ��ַ    */

    RESTORE_REGS                                                        ;/*  �ָ�Э�̼Ĵ���              */
    FUNC_END(archCrtCtxSwitch)
#endif

;/*********************************************************************************************************
;  �жϷ���ʱ, �߳�װ��
;  ����Ϊ��ǰ CPU ���ƿ�, �� A0 Ϊ��ǰ CPU ���ƿ�ָ��
;*********************************************************************************************************/

FUNC_DEF(archIntCtxLoad)
    LD.W        A1 , (A0 , 0)                                           ;/*  ��ȡ��ǰ TCB �� REG_CTX ��ַ*/

    RESTORE_REGS                                                        ;/*  �ָ�����Ĵ���              */
    FUNC_END(archIntCtxLoad)

;/*********************************************************************************************************
;  �ź������ķ���
;  ����Ϊ ARCH_REG_CTX ָ��, �� A0 Ϊ ARCH_REG_CTX ָ��
;*********************************************************************************************************/

FUNC_DEF(archSigCtxLoad)
    MOV         A1 , A0
    
    RESTORE_REGS                                                        ;/*  �ָ�����Ĵ���              */
    FUNC_END(archSigCtxLoad)

;/*********************************************************************************************************
;  ��������������
;  ���� A0 ΪĿ�� ARCH_REG_CTX ָ��, A1 ΪԴ ARCH_REG_CTX ָ��
;*********************************************************************************************************/

FUNC_DEF(archTaskCtxCopy)
    PUSH        R4-R8

    LDM         R4-R8 , (A1)
    STM         R4-R8 , (A0)
    ADDI        A0 , (5 * ARCH_REG_SIZE)
    ADDI        A1 , (5 * ARCH_REG_SIZE)

    LDM         R4-R8 , (A1)
    STM         R4-R8 , (A0)
    ADDI        A0 , (5 * ARCH_REG_SIZE)
    ADDI        A1 , (5 * ARCH_REG_SIZE)

    LDM         R4-R8 , (A1)
    STM         R4-R8 , (A0)
    ADDI        A0 , (5 * ARCH_REG_SIZE)
    ADDI        A1 , (5 * ARCH_REG_SIZE)

    LDM         R4-R8 , (A1)
    STM         R4-R8 , (A0)
    ADDI        A0 , (5 * ARCH_REG_SIZE)
    ADDI        A1 , (5 * ARCH_REG_SIZE)
    
    LDM         R4-R8 , (SP)
    ADDI        SP , (5 * ARCH_REG_SIZE)

    RTS
    FUNC_END(archTaskCtxCopy)
    
    FILE_END()

#endif
;/*********************************************************************************************************
;  END
;*********************************************************************************************************/
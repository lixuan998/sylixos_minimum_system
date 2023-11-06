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
;** ��   ��   ��: ppcExcE500Asm.S
;**
;** ��   ��   ��: Jiao.JinXing (������)
;**
;** �ļ���������: 2016 �� 05 �� 04 ��
;**
;** ��        ��: PowerPC E500 ��ϵ�����쳣����.
;*********************************************************************************************************/

#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>
#include "arch/ppc/arch_e500.h"
#include "../ppcContextAsm.h"
#include "ppcContextE500Asm.h"

    FILE_BEGIN()

    IMPORT_LABEL(API_ThreadTcbInter)
    IMPORT_LABEL(API_InterStackBaseGet)
    IMPORT_LABEL(API_InterEnter)
    IMPORT_LABEL(API_InterExit)
    IMPORT_LABEL(bspIntHandle)

    IMPORT_LABEL(archE500CriticalInputExceptionHandle)
    IMPORT_LABEL(archE500MachineCheckExceptionHandle)
    IMPORT_LABEL(archE500DataStorageExceptionHandle)
    IMPORT_LABEL(archE500InstructionStorageExceptionHandle)
    IMPORT_LABEL(archE500AlignmentExceptionHandle)
    IMPORT_LABEL(archE500ProgramExceptionHandle)
    IMPORT_LABEL(archE500FpuUnavailableExceptionHandle)
    IMPORT_LABEL(archE500SystemCallHandle)
    IMPORT_LABEL(archE500ApUnavailableExceptionHandle)
    IMPORT_LABEL(archE500DecrementerInterruptHandle)
    IMPORT_LABEL(archE500TimerInterruptHandle)
    IMPORT_LABEL(archE500WatchdogInterruptHandle)
#if LW_CFG_VMM_EN == 0
    IMPORT_LABEL(archE500DataTLBErrorHandle)
    IMPORT_LABEL(archE500InstructionTLBErrorHandle)
#endif                                                                  /*  LW_CFG_VMM_EN == 0          */
    IMPORT_LABEL(archE500DebugExceptionHandle)
    IMPORT_LABEL(archE500SpeUnavailableExceptionHandle)
    IMPORT_LABEL(archE500FpDataExceptionHandle)
    IMPORT_LABEL(archE500FpRoundExceptionHandle)
    IMPORT_LABEL(archE500AltiVecUnavailableExceptionHandle)
    IMPORT_LABEL(archE500AltiVecAssistExceptionHandle)
    IMPORT_LABEL(archE500PerfMonitorExceptionHandle)
    IMPORT_LABEL(archE500DoorbellExceptionHandle)
    IMPORT_LABEL(archE500DoorbellCriticalExceptionHandle)

    EXPORT_LABEL(archE500CriticalInputExceptionEntry)
    EXPORT_LABEL(archE500MachineCheckExceptionEntry)
    EXPORT_LABEL(archE500DataStorageExceptionEntry)
    EXPORT_LABEL(archE500InstructionStorageExceptionEntry)
    EXPORT_LABEL(archE500ExternalInterruptEntry)
    EXPORT_LABEL(archE500AlignmentExceptionEntry)
    EXPORT_LABEL(archE500ProgramExceptionEntry)
    EXPORT_LABEL(archE500FpuUnavailableExceptionEntry)
    EXPORT_LABEL(archE500SystemCallEntry)
    EXPORT_LABEL(archE500ApUnavailableExceptionEntry)
    EXPORT_LABEL(archE500DecrementerInterruptEntry)
    EXPORT_LABEL(archE500TimerInterruptEntry)
    EXPORT_LABEL(archE500WatchdogInterruptEntry)
#if LW_CFG_VMM_EN == 0
    EXPORT_LABEL(archE500DataTLBErrorEntry)
    EXPORT_LABEL(archE500InstructionTLBErrorEntry)
#endif                                                                  /*  LW_CFG_VMM_EN == 0          */
    EXPORT_LABEL(archE500DebugExceptionEntry)
    EXPORT_LABEL(archE500SpeUnavailableExceptionEntry)
    EXPORT_LABEL(archE500FpDataExceptionEntry)
    EXPORT_LABEL(archE500FpRoundExceptionEntry)
    EXPORT_LABEL(archE500AltiVecUnavailableExceptionEntry)
    EXPORT_LABEL(archE500AltiVecAssistExceptionEntry)
    EXPORT_LABEL(archE500PerfMonitorExceptionEntry)
    EXPORT_LABEL(archE500DoorbellExceptionEntry)
    EXPORT_LABEL(archE500DoorbellCriticalExceptionEntry)

;/*********************************************************************************************************
;  �쳣�������򶼷��� .vector ����
;*********************************************************************************************************/

    SECTION(.vector)

;/*********************************************************************************************************
;  �쳣��ں�
;  ע��: E500 �� 3 ���쳣: ��ͨ�쳣, ��������쳣, �ٽ������쳣(���������쳣�Ϳ��Ź��жϼ������ٽ��쳣)
;  �� 3 ���쳣ʹ�õļĴ�����һ��
;  ��ͨ�쳣ʹ��     SRR0   SRR1
;  ��������쳣ʹ�� MCSRR0 MCSRR1
;  �ٽ������쳣ʹ�� CSRR0  CSRR1
;  ���Զ����� 3 ���쳣��ں�
;*********************************************************************************************************/

MACRO_DEF(E500_EXC_ENTRY, handle)
    ;/*
    ; * ʹ���쳣��ʱջ, �����쳣��ʱջ������ʱ�����ı�����, �� volatile �Ĵ������浽��ʱ�����ı�����
    ; */
    E500_EXC_SAVE_VOLATILE

    SUBI    SP , SP , 16                                                ;/*  ���� C ����ջ֡             */

    ;/*
    ; * API_InterEnter
    ; */
    BL      API_InterEnter                                              ;/*  ��ȡ�жϲ����ŵ�����ֵ R3   */

    CMPWI   CR0, R3 , 1
    BNE     CR0, 1f

    ;/*
    ; * ��õ�ǰ���� TCB �� ARCH_REG_CTX ��ַ
    ; */
    BL      API_ThreadTcbInter                                          ;/*  get current tcb             */

    ;/*
    ; * ���� volatile �Ĵ�������ǰ���� TCB �� ARCH_REG_CTX ��
    ; */
    ADDI    SP , SP , 16                                                ;/*  ���� C ����ջ֡             */

    EXC_COPY_VOLATILE

    ;/*
    ; * ���� non volatile �Ĵ�������ǰ���� TCB �� ARCH_REG_CTX ��
    ; */
    EXC_SAVE_NON_VOLATILE

    OR      R14 , R3 , R3                                               ;/*  R14 = R3 = ARCH_REG_CTX ָ��*/

    SUBI    SP , SP , 16                                                ;/*  ���� C ����ջ֡             */

    ;/*
    ; * ��һ�ν����ж�: ��õ�ǰ CPU �ж϶�ջջ��, ������ SP
    ; */
    BL      API_InterStackBaseGet
    SUBI    SP , R3 , 16                                                ;/*  ���� C ����ջ֡             */

    OR      R4 , R14 , R14                                              ;/*  R4 = R14 = ARCH_REG_CTX ָ��*/
2:
    ;/*
    ; * handle(���ص�ַ, ARCH_REG_CTX ָ��)
    ; */
    MFSPR   R3 , SRR0                                                   ;/*  ���ݷ��ص�ַ                */
    ISYNC
    BL      \handle                                                     ;/*  ���ô�������                */

    ;/*
    ; * API_InterExit()
    ; * ���û�з����ж�Ƕ��, �� API_InterExit ����� archIntCtxLoad ����
    ; */
    BL      API_InterExit

    ;/*
    ; * ��������, ˵���������ж�Ƕ��
    ; */
    ADDI    R4 , SP , 16                                                ;/*  ���� C ����ջ֡             */
    RESTORE_REGS                                                        ;/*  �ָ����мĴ���              */

1:
    ;/*
    ; * ���ǵ�һ�ν����ж�
    ; */
    ADDI    SP , SP , 16                                                ;/*  ���� C ����ջ֡             */

    MFSPR   R3 , SPRG0                                                  ;/*  �쳣ǰ SP(R1)               */
    SUBI    R3 , R3 , ARCH_REG_CTX_SIZE                                 ;/*  ���쳣��ջ���������ı�����  */

    ;/*
    ; * ���� volatile �Ĵ������쳣��ջ��������ı�����
    ; */
    EXC_COPY_VOLATILE

    ;/*
    ; * ���� non volatile �Ĵ������쳣��ջ��
    ; */
    EXC_SAVE_NON_VOLATILE

    SUBI    SP , R3 , 16                                                ;/*  ʹ���쳣��ջ������C����ջ֡ */
    OR      R4 , R3 , R3                                                ;/*  R4 = R3 = ARCH_REG_CTX ָ�� */
    B       2b
    MACRO_END()

;/*********************************************************************************************************
;  �ٽ������쳣��ں�
;*********************************************************************************************************/

MACRO_DEF(E500_CI_EXC_ENTRY, handle)
    ;/*
    ; * ʹ���쳣��ʱջ, �����쳣��ʱջ������ʱ�����ı�����, �� volatile �Ĵ������浽��ʱ�����ı�����
    ; */
    E500_CI_EXC_SAVE_VOLATILE

    SUBI    SP , SP , 16                                                ;/*  ���� C ����ջ֡             */

    ;/*
    ; * API_InterEnter
    ; */
    BL      API_InterEnter                                              ;/*  ��ȡ�жϲ����ŵ�����ֵ R3   */

    CMPWI   CR0, R3 , 1
    BNE     CR0, 1f

    ;/*
    ; * ��õ�ǰ���� TCB �� ARCH_REG_CTX ��ַ
    ; */
    BL      API_ThreadTcbInter                                          ;/*  get current tcb             */

    ;/*
    ; * ���� volatile �Ĵ�������ǰ���� TCB �� ARCH_REG_CTX ��
    ; */
    ADDI    SP , SP , 16                                                ;/*  ���� C ����ջ֡             */

    EXC_COPY_VOLATILE

    ;/*
    ; * ���� non volatile �Ĵ�������ǰ���� TCB �� ARCH_REG_CTX ��
    ; */
    EXC_SAVE_NON_VOLATILE

    OR      R14 , R3 , R3                                               ;/*  R14 = R3 = ARCH_REG_CTX ָ��*/

    SUBI    SP , SP , 16                                                ;/*  ���� C ����ջ֡             */

    ;/*
    ; * ��һ�ν����ж�: ��õ�ǰ CPU �ж϶�ջջ��, ������ SP
    ; */
    BL      API_InterStackBaseGet
    SUBI    SP , R3 , 16                                                ;/*  ���� C ����ջ֡             */

    OR      R4 , R14 , R14                                              ;/*  R4 = R14 = ARCH_REG_CTX ָ��*/

2:
    ;/*
    ; * handle(���ص�ַ, ARCH_REG_CTX ָ��)
    ; */
    MFSPR   R3 , CSRR0                                                  ;/*  ���ݷ��ص�ַ                */
    ISYNC
    BL      \handle                                                     ;/*  ���ô�������                */

    ;/*
    ; * API_InterExit()
    ; * ���û�з����ж�Ƕ��, �� API_InterExit ����� archIntCtxLoad ����
    ; */
    BL      API_InterExit

    ;/*
    ; * ��������, ˵���������ж�Ƕ��
    ; */
    ADDI    R4 , SP , 16                                                ;/*  ���� C ����ջ֡             */
    RESTORE_REGS                                                        ;/*  �ָ����мĴ���              */

1:
    ;/*
    ; * ���ǵ�һ�ν����ж�
    ; */
    ADDI    SP , SP , 16                                                ;/*  ���� C ����ջ֡             */

    MFSPR   R3 , SPRG0                                                  ;/*  �쳣ǰ SP(R1)               */
    SUBI    R3 , R3 , ARCH_REG_CTX_SIZE                                 ;/*  ���쳣��ջ���������ı�����  */

    ;/*
    ; * ���� volatile �Ĵ������쳣��ջ��������ı�����
    ; */
    EXC_COPY_VOLATILE

    ;/*
    ; * ���� non volatile �Ĵ������쳣��ջ��
    ; */
    EXC_SAVE_NON_VOLATILE

    SUBI    SP , R3 , 16                                                ;/*  ʹ���쳣��ջ������C����ջ֡ */
    OR      R4 , R3 , R3                                                ;/*  R4 = R3 = ARCH_REG_CTX ָ�� */
    B       2b
    MACRO_END()

;/*********************************************************************************************************
;  ��������쳣��ں�
;*********************************************************************************************************/

MACRO_DEF(E500_MC_EXC_ENTRY, handle)
    ;/*
    ; * ʹ���쳣��ʱջ, �����쳣��ʱջ������ʱ�����ı�����, �� volatile �Ĵ������浽��ʱ�����ı�����
    ; */
    E500_MC_EXC_SAVE_VOLATILE

    SUBI    SP , SP , 16                                                ;/*  ���� C ����ջ֡             */

    ;/*
    ; * API_InterEnter
    ; */
    BL      API_InterEnter                                              ;/*  ��ȡ�жϲ����ŵ�����ֵ R3   */

    CMPWI   CR0, R3 , 1
    BNE     CR0, 1f

    ;/*
    ; * ��õ�ǰ���� TCB �� ARCH_REG_CTX ��ַ
    ; */
    BL      API_ThreadTcbInter                                          ;/*  get current tcb             */

    ;/*
    ; * ���� volatile �Ĵ�������ǰ���� TCB �� ARCH_REG_CTX ��
    ; */
    ADDI    SP , SP , 16                                                ;/*  ���� C ����ջ֡             */

    EXC_COPY_VOLATILE

    ;/*
    ; * ���� non volatile �Ĵ�������ǰ���� TCB �� ARCH_REG_CTX ��
    ; */
    EXC_SAVE_NON_VOLATILE

    OR      R14 , R3 , R3                                               ;/*  R14 = R3 = ARCH_REG_CTX ָ��*/

    SUBI    SP , SP , 16                                                ;/*  ���� C ����ջ֡             */

    ;/*
    ; * ��һ�ν����ж�: ��õ�ǰ CPU �ж϶�ջջ��, ������ SP
    ; */
    BL      API_InterStackBaseGet
    SUBI    SP , R3 , 16                                                ;/*  ���� C ����ջ֡             */

    OR      R4 , R14 , R14                                              ;/*  R4 = R14 = ARCH_REG_CTX ָ��*/

2:
    ;/*
    ; * handle(���ص�ַ, ARCH_REG_CTX ָ��)
    ; */
    MFSPR   R3 , MCSRR0                                                 ;/*  ���ݷ��ص�ַ                */
    ISYNC
    BL      \handle                                                     ;/*  ���ô�������                */

    ;/*
    ; * API_InterExit()
    ; * ���û�з����ж�Ƕ��, �� API_InterExit ����� archIntCtxLoad ����
    ; */
    BL      API_InterExit

    ;/*
    ; * ��������, ˵���������ж�Ƕ��
    ; */
    ADDI    R4 , SP , 16                                                ;/*  ���� C ����ջ֡             */
    RESTORE_REGS                                                        ;/*  �ָ����мĴ���              */

1:
    ;/*
    ; * ���ǵ�һ�ν����ж�
    ; */
    ADDI    SP , SP , 16                                                ;/*  ���� C ����ջ֡             */

    MFSPR   R3 , SPRG0                                                  ;/*  �쳣ǰ SP(R1)               */
    SUBI    R3 , R3 , ARCH_REG_CTX_SIZE                                 ;/*  ���쳣��ջ���������ı�����  */

    ;/*
    ; * ���� volatile �Ĵ������쳣��ջ��������ı�����
    ; */
    EXC_COPY_VOLATILE

    ;/*
    ; * ���� non volatile �Ĵ������쳣��ջ��
    ; */
    EXC_SAVE_NON_VOLATILE

    SUBI    SP , R3 , 16                                                ;/*  ʹ���쳣��ջ������C����ջ֡ */
    OR      R4 , R3 , R3                                                ;/*  R4 = R3 = ARCH_REG_CTX ָ�� */
    B       2b
    MACRO_END()

;/*********************************************************************************************************
;  Embedded Category�CDefined IVORs
;*********************************************************************************************************/

;/*********************************************************************************************************
;  �ٽ������쳣���(Vector IVOR0)
;*********************************************************************************************************/

EXCE_DEF(archE500CriticalInputExceptionEntry)
    E500_CI_EXC_ENTRY   archE500CriticalInputExceptionHandle
    EXCE_END()

;/*********************************************************************************************************
;  ��������쳣���(Vector IVOR1)
;*********************************************************************************************************/

EXCE_DEF(archE500MachineCheckExceptionEntry)
    E500_MC_EXC_ENTRY   archE500MachineCheckExceptionHandle
    EXCE_END()

;/*********************************************************************************************************
;  ���ݴ洢�쳣���(Vector IVOR2)
;*********************************************************************************************************/

EXCE_DEF(archE500DataStorageExceptionEntry)
    E500_EXC_ENTRY      archE500DataStorageExceptionHandle
    EXCE_END()

;/*********************************************************************************************************
;  ָ������쳣���(Vector IVOR3)
;*********************************************************************************************************/

EXCE_DEF(archE500InstructionStorageExceptionEntry)
    E500_EXC_ENTRY      archE500InstructionStorageExceptionHandle
    EXCE_END()

;/*********************************************************************************************************
;  �ⲿ�ж����(Vector IVOR4)
;*********************************************************************************************************/

EXCE_DEF(archE500ExternalInterruptEntry)
    E500_EXC_ENTRY      bspIntHandle
    EXCE_END()

;/*********************************************************************************************************
;  �Ƕ����쳣���(Vector IVOR5)
;*********************************************************************************************************/

EXCE_DEF(archE500AlignmentExceptionEntry)
    E500_EXC_ENTRY      archE500AlignmentExceptionHandle
    EXCE_END()

;/*********************************************************************************************************
;  �����쳣���(Vector IVOR6)
;*********************************************************************************************************/

EXCE_DEF(archE500ProgramExceptionEntry)
    E500_EXC_ENTRY      archE500ProgramExceptionHandle
    EXCE_END()

;/*********************************************************************************************************
;  FPU �������쳣���(Vector IVOR7)
;*********************************************************************************************************/

EXCE_DEF(archE500FpuUnavailableExceptionEntry)
    E500_EXC_ENTRY      archE500FpuUnavailableExceptionHandle
    FUNC_END()

;/*********************************************************************************************************
;  ϵͳ�������(Vector IVOR8)
;*********************************************************************************************************/

EXCE_DEF(archE500SystemCallEntry)
    E500_EXC_ENTRY      archE500SystemCallHandle
    EXCE_END()

;/*********************************************************************************************************
;  AP ������ (not defined by Embedded Implementation Standards)�����(Vector IVOR9)
;*********************************************************************************************************/

EXCE_DEF(archE500ApUnavailableExceptionEntry)
    E500_EXC_ENTRY      archE500ApUnavailableExceptionHandle
    EXCE_END()

;/*********************************************************************************************************
;  Decrementer �ж����(Vector IVOR10)
;*********************************************************************************************************/

EXCE_DEF(archE500DecrementerInterruptEntry)
    E500_EXC_ENTRY      archE500DecrementerInterruptHandle
    EXCE_END()

;/*********************************************************************************************************
;  Fixed-interval timer �ж����(Vector IVOR11)
;*********************************************************************************************************/

EXCE_DEF(archE500TimerInterruptEntry)
    E500_EXC_ENTRY      archE500TimerInterruptHandle
    EXCE_END()

;/*********************************************************************************************************
;  Watchdog timer �ж����(Vector IVOR12)
;*********************************************************************************************************/

EXCE_DEF(archE500WatchdogInterruptEntry)
    E500_CI_EXC_ENTRY   archE500WatchdogInterruptHandle
    EXCE_END()

;/*********************************************************************************************************
;  ���� TLB �����쳣���(Vector IVOR13)
;*********************************************************************************************************/

#if LW_CFG_VMM_EN == 0

EXCE_DEF(archE500DataTLBErrorEntry)
    E500_EXC_ENTRY      archE500DataTLBErrorHandle
    EXCE_END()

;/*********************************************************************************************************
;  ָ�� TLB �����쳣���(Vector IVOR14)
;*********************************************************************************************************/

EXCE_DEF(archE500InstructionTLBErrorEntry)
    E500_EXC_ENTRY      archE500InstructionTLBErrorHandle
    EXCE_END()

#endif                                                                  /*  LW_CFG_VMM_EN == 0           */

;/*********************************************************************************************************
;  Debug �쳣���(Vector IVOR15)
;*********************************************************************************************************/

EXCE_DEF(archE500DebugExceptionEntry)
    E500_CI_EXC_ENTRY   archE500DebugExceptionHandle
    EXCE_END()

;/*********************************************************************************************************
;  e500-Specific IVORs
;*********************************************************************************************************/

;/*********************************************************************************************************
;  SPE �������쳣���(Vector IVOR32)
;*********************************************************************************************************/

EXCE_DEF(archE500SpeUnavailableExceptionEntry)
    E500_EXC_ENTRY      archE500SpeUnavailableExceptionHandle
    EXCE_END()

;/*********************************************************************************************************
;  SPE floating-point data �쳣���(Vector IVOR33)
;*********************************************************************************************************/

EXCE_DEF(archE500FpDataExceptionEntry)
    E500_EXC_ENTRY      archE500FpDataExceptionHandle
    EXCE_END()

;/*********************************************************************************************************
;  SPE floating-point round �쳣���(Vector IVOR34)
;*********************************************************************************************************/

EXCE_DEF(archE500FpRoundExceptionEntry)
    E500_EXC_ENTRY      archE500FpRoundExceptionHandle
    EXCE_END()

;/*********************************************************************************************************
;  AltiVec �������쳣���(Vector IVOR32)
;*********************************************************************************************************/

EXCE_DEF(archE500AltiVecUnavailableExceptionEntry)
    E500_EXC_ENTRY      archE500AltiVecUnavailableExceptionHandle
    EXCE_END()

;/*********************************************************************************************************
;  AltiVec Assist �쳣���(Vector IVOR33)
;*********************************************************************************************************/

EXCE_DEF(archE500AltiVecAssistExceptionEntry)
    E500_EXC_ENTRY      archE500AltiVecAssistExceptionHandle
    EXCE_END()

;/*********************************************************************************************************
;  Performance monitor �쳣���(Vector IVOR35)
;*********************************************************************************************************/

EXCE_DEF(archE500PerfMonitorExceptionEntry)
    E500_EXC_ENTRY      archE500PerfMonitorExceptionHandle
    EXCE_END()

;/*********************************************************************************************************
;  Processor doorbell �쳣���(Vector IVOR36)
;*********************************************************************************************************/

EXCE_DEF(archE500DoorbellExceptionEntry)
    E500_EXC_ENTRY      archE500DoorbellExceptionHandle
    EXCE_END()

;/*********************************************************************************************************
;  Processor doorbell critical �쳣���(Vector IVOR37)
;*********************************************************************************************************/

EXCE_DEF(archE500DoorbellCriticalExceptionEntry)
    E500_CI_EXC_ENTRY   archE500DoorbellCriticalExceptionHandle
    EXCE_END()

    FILE_END()

;/*********************************************************************************************************
;  END
;*********************************************************************************************************/
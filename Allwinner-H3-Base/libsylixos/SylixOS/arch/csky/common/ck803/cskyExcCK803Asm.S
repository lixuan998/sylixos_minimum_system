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
;** ��   ��   ��: cskyExcCK803Asm.S
;**
;** ��   ��   ��: Wang.Xuan (���Q)
;**
;** �ļ���������: 2018 �� 11 �� 12 ��
;**
;** ��        ��: C-SKY CK803 ��ϵ�ܹ��쳣����.
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

    IMPORT_LABEL(API_ThreadTcbInter)
    IMPORT_LABEL(API_InterStackBaseGet)
    IMPORT_LABEL(API_InterEnter)
    IMPORT_LABEL(API_InterExit)
    IMPORT_LABEL(bspIntHandle)

    IMPORT_LABEL(archResetHandle)
    IMPORT_LABEL(archUnalignedExceptHandle)
    IMPORT_LABEL(archAccessExceptHandle)
    IMPORT_LABEL(archDivideZeroExceptHandle)
    IMPORT_LABEL(archIllegalInstExceptHandle)
    IMPORT_LABEL(archPrivilegeExceptHandle)
    IMPORT_LABEL(archTrapExceptHandle)
    IMPORT_LABEL(archBreakPointExceptHandle)
    IMPORT_LABEL(archFatalErrExceptHandle)
    IMPORT_LABEL(archIdlyExceptHandle)
    IMPORT_LABEL(archAutoIntHandle)
    IMPORT_LABEL(archReservedExceptHandle)
    IMPORT_LABEL(archTrap0ExceptHandle)
    IMPORT_LABEL(archTrap1ExceptHandle)
    IMPORT_LABEL(archTrap2ExceptHandle)
    IMPORT_LABEL(archTrap3ExceptHandle)
    IMPORT_LABEL(archTspendIntHandle)
    IMPORT_LABEL(archFpuExceptHandle)
    IMPORT_LABEL(archExcOldStack)
    IMPORT_LABEL(archExcIrqStack)
    
    EXPORT_LABEL(archUnalignedExceptEntry)
    EXPORT_LABEL(archAccessExceptEntry)
    EXPORT_LABEL(archDivideZeroExceptEntry)
    EXPORT_LABEL(archIllegalInstExceptEntry)
    EXPORT_LABEL(archPrivilegeExceptEntry)
    EXPORT_LABEL(archTrapExceptEntry)
    EXPORT_LABEL(archBreakPointExceptEntry)
    EXPORT_LABEL(archFatalErrExceptEntry)
    EXPORT_LABEL(archIdlyExceptEntry)
    EXPORT_LABEL(archAutoIntEntry)
    EXPORT_LABEL(archReservedExceptEntry)
    EXPORT_LABEL(archTrap0ExceptEntry)
    EXPORT_LABEL(archTrap1ExceptEntry)
    EXPORT_LABEL(archTrap2ExceptEntry)
    EXPORT_LABEL(archTrap3ExceptEntry)
    EXPORT_LABEL(archTspendIntEntry)
    EXPORT_LABEL(archFpuExceptEntry)

    EXPORT_LABEL(vector)

;/*********************************************************************************************************
;  ������쳣������ vector ����Ϊ������, BSP ��������ʵ���쳣������
;*********************************************************************************************************/

    WEAK(vector)

;/*********************************************************************************************************
;  �쳣/�ж���ں�
;*********************************************************************************************************/

    SECTION(.text)

MACRO_DEF(IRQ_ENTRY  irq  handle)
    ;/*
    ; * ʹ���쳣��ʱջ, �����쳣��ʱջ������ʱ�����ı�����, ���Ĵ������浽��ʱ�����ı�����
    ; */
    PUSH        R4 , R5                                                 ;/*  �Ƚ� R4, R5 ��ջ            */
    LRW         R4 , archExcOldStack                                    ;/*  ��ȡ�洢�쳣ǰ SP ��λ��    */
    ADDI        R5 , SP , 2 * ARCH_REG_SIZE                             ;/*  ��ȡ�쳣ǰ SP ָ��          */
    ST.W        R5 , (R4)                                               ;/*  ���쳣ǰ SP �����Ӧλ��    */
    LD.W        R4 , (SP)                                               ;/*  ����ʹ�� POP ָ��           */
    LD.W        R5 , (SP, ARCH_REG_SIZE)                                ;/*  POP ָ���ͬʱ�ı� PC       */

    LRW         SP , archExcIrqStack                                    ;/*  ��ȡ�쳣��ʱջ SP ��λ��    */
    LD.W        SP , (SP)                                               ;/*  �õ��쳣ջָ��              */
    SUBI        SP , ARCH_REG_CTX_SIZE                                  ;/*  ����ʱ��ջ���������ı�����  */

    STM         R0-R15 , (SP)
    ST.W        R28 , (SP , XGREG28)

    MFCR        R0 , EPC                                                ;/*  EPC ���� PC ����            */
    STW         R0 , (SP , XPC)

    MFCR        R0 , EPSR                                               ;/*  ���� PSR �Ĵ���             */
    ST.W        R0 , (SP , XPSR)

    LRW         R0 , archExcOldStack                                    ;/*  �����쳣ǰ SP               */
    LD.W        R0 , (R0)
    ST.W        R0 , (SP , XGREG(14))                                   ;/*  �����쳣ǰ SP               */

    ;/*
    ; * API_InterEnter(�Ĵ���������)
    ; */
    MOV         A0 , SP
    JSRI        API_InterEnter

    CMPNEI      A0 , 1
    BT          3f

    ;/*
    ; * ��һ�ν����ж�: ��õ�ǰ CPU �ж϶�ջջ��, ������ SP
    ; */
    JSRI        API_InterStackBaseGet
    MOV         SP , A0

    ;/*
    ; * ��õ�ǰ���� TCB �� ARCH_REG_CTX ��ַ
    ; */
    JSRI        API_ThreadTcbInter
    MOV         A1 , A0

2:
    PSRSET      EE

    ;/*
    ; * handle(vector, �Ĵ���������)
    ; */
    MOVI        A0 , \irq

    JSRI        \handle

    ;/*
    ; * API_InterExit()
    ; * ���û�з����ж�Ƕ��, �� API_InterExit ����� archIntCtxLoad ����
    ; */
    JSRI        API_InterExit

    ;/*
    ; * ��������, ˵���������ж�Ƕ��
    ; */
    MOV         A1 , SP
    RESTORE_REGS

3:
    ;/*
    ; * ���ǵ�һ�ν����ж�
    ; */
    LRW         SP , archExcOldStack
    LD.W        SP , (SP)                                               ;/*  ��ȡ�쳣ǰ SP ��λ��        */
    SUBI        SP , ARCH_REG_CTX_SIZE
    MOV         A1 , SP
    BR          2b
    MACRO_END()

;/*********************************************************************************************************
;  δ��������쳣��ں���
;*********************************************************************************************************/

FUNC_DEF(archUnalignedExceptEntry)
    IRQ_ENTRY   1   archUnalignedExceptHandle
    FUNC_END(archUnalignedExceptEntry)

;/*********************************************************************************************************
;  ���ʴ����쳣��ں���
;*********************************************************************************************************/

FUNC_DEF(archAccessExceptEntry)
    IRQ_ENTRY   2   archAccessExceptHandle
    FUNC_END(archAccessExceptEntry)

;/*********************************************************************************************************
;  �����쳣��ں���
;*********************************************************************************************************/

FUNC_DEF(archDivideZeroExceptEntry)
    IRQ_ENTRY   3   archDivideZeroExceptHandle
    FUNC_END(archDivideZeroExceptEntry)

;/*********************************************************************************************************
;  �Ƿ�ָ���쳣��ں���
;*********************************************************************************************************/

FUNC_DEF(archIllegalInstExceptEntry)
    IRQ_ENTRY   4   archIllegalInstExceptHandle
    FUNC_END(archIllegalInstExceptEntry)

;/*********************************************************************************************************
;  ��ȨΥ���쳣��ں���
;*********************************************************************************************************/

FUNC_DEF(archPrivilegeExceptEntry)
    IRQ_ENTRY   5   archPrivilegeExceptHandle
    FUNC_END(archPrivilegeExceptEntry)

;/*********************************************************************************************************
;  �����쳣��ں���
;*********************************************************************************************************/

FUNC_DEF(archTrapExceptEntry)
    IRQ_ENTRY   6   archTrapExceptHandle
    FUNC_END(archTrapExceptEntry)

;/*********************************************************************************************************
;  �ϵ��쳣��ں���
;*********************************************************************************************************/

FUNC_DEF(archBreakPointExceptEntry)
    IRQ_ENTRY   7   archBreakPointExceptHandle
    FUNC_END(archBreakPointExceptEntry)

;/*********************************************************************************************************
;  ���ɻָ������쳣��ں���
;*********************************************************************************************************/

FUNC_DEF(archFatalErrExceptEntry)
    IRQ_ENTRY   8   archFatalErrExceptHandle
    FUNC_END(archFatalErrExceptEntry)

;/*********************************************************************************************************
;  Idly �쳣��ں���
;*********************************************************************************************************/

FUNC_DEF(archIdlyExceptEntry)
    IRQ_ENTRY   9   archIdlyExceptHandle
    FUNC_END(archIdlyExceptEntry)

;/*********************************************************************************************************
;  ��ͨ�ж���ں���
;*********************************************************************************************************/

FUNC_DEF(archAutoIntEntry)
    IRQ_ENTRY   10  archAutoIntHandle
    FUNC_END(archAutoIntEntry)

;/*********************************************************************************************************
;  Reserved �쳣��ں���
;*********************************************************************************************************/

FUNC_DEF(archReservedExceptEntry)
    IRQ_ENTRY   12  archReservedExceptHandle
    FUNC_END(archReservedExceptEntry)

;/*********************************************************************************************************
;  ����ָ���쳣 0 ��ں���
;*********************************************************************************************************/

FUNC_DEF(archTrap0ExceptEntry)
    IRQ_ENTRY   16  archTrap0ExceptHandle
    FUNC_END(archTrap0ExceptEntry)

;/*********************************************************************************************************
;  ����ָ���쳣 1 ��ں���
;*********************************************************************************************************/

FUNC_DEF(archTrap1ExceptEntry)
    IRQ_ENTRY   17  archTrap1ExceptHandle
    FUNC_END(archTrap1ExceptEntry)

;/*********************************************************************************************************
;  ����ָ���쳣 2 ��ں���
;*********************************************************************************************************/

FUNC_DEF(archTrap2ExceptEntry)
    IRQ_ENTRY   18  archTrap2ExceptHandle
    FUNC_END(archTrap2ExceptEntry)

;/*********************************************************************************************************
;  ����ָ���쳣 3 ��ں���
;*********************************************************************************************************/

FUNC_DEF(archTrap3ExceptEntry)
    IRQ_ENTRY   19  archTrap3ExceptHandle
    FUNC_END(archTrap3ExceptEntry)

;/*********************************************************************************************************
;  TSPEND �ж���ں���
;*********************************************************************************************************/

FUNC_DEF(archTspendIntEntry)
    IRQ_ENTRY   22  archTspendIntHandle
    FUNC_END(archTspendIntEntry)

;/*********************************************************************************************************
;  ���������쳣��ں���
;*********************************************************************************************************/

FUNC_DEF(archFpuExceptEntry)
    IRQ_ENTRY   30  archFpuExceptHandle
    FUNC_END(archFpuExceptEntry)

;/*********************************************************************************************************
;  32 ~ 255 ���ж���ں���
;*********************************************************************************************************/

    .irp        Id,                                   \
               32,  33,  34,  35,  36,  37,  38,  39, \
     40,  41,  42,  43,  44,  45,  46,  47,  48,  49, \
     50,  51,  52,  53,  54,  55,  56,  57,  58,  59, \
     60,  61,  62,  63,  64,  65,  66,  67,  68,  69, \
     70,  71,  72,  73,  74,  75,  76,  77,  78,  79, \
     80,  81,  82,  83,  84,  85,  86,  87,  88,  89, \
     90,  91,  92,  93,  94,  95,  96,  97,  98,  99, \
    100, 101, 102, 103, 104, 105, 106, 107, 108, 109, \
    110, 111, 112, 113, 114, 115, 116, 117, 118, 119, \
    120, 121, 122, 123, 124, 125, 126, 127, 128, 129, \
    130, 131, 132, 133, 134, 135, 136, 137, 138, 139, \
    140, 141, 142, 143, 144, 145, 146, 147, 148, 149, \
    150, 151, 152, 153, 154, 155, 156, 157, 158, 159, \
    160, 161, 162, 163, 164, 165, 166, 167, 168, 169, \
    170, 171, 172, 173, 174, 175, 176, 177, 178, 179, \
    180, 181, 182, 183, 184, 185, 186, 187, 188, 189, \
    190, 191, 192, 193, 194, 195, 196, 197, 198, 199, \
    200, 201, 202, 203, 204, 205, 206, 207, 208, 209, \
    210, 211, 212, 213, 214, 215, 216, 217, 218, 219, \
    220, 221, 222, 223, 224, 225, 226, 227, 228, 229, \
    230, 231, 232, 233, 234, 235, 236, 237, 238, 239, \
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, \
    250, 251, 252, 253, 254, 255
    EXPORT_LABEL(archIntEntry\Id)
FUNC_DEF(archIntEntry\Id)
    IRQ_ENTRY   \Id  bspIntHandle
    FUNC_END(archIntEntry\Id)
    .endr

;/*********************************************************************************************************
;  �쳣������
;*********************************************************************************************************/

    .align  10
FUNC_DEF(vector)
    .long   archResetEntry                                              ;/*  0  - Reset                  */
    .long   archUnalignedExceptEntry                                    ;/*  1  - Unaligned              */
    .long   archAccessExceptEntry                                       ;/*  2  - Access                 */
    .long   archDivideZeroExceptEntry                                   ;/*  3  - Divide By Zero         */
    .long   archIllegalInstExceptEntry                                  ;/*  4  - Illegal Instruction    */
    .long   archPrivilegeExceptEntry                                    ;/*  5  - Privilege              */
    .long   archTrapExceptEntry                                         ;/*  6  - Trap                   */
    .long   archBreakPointExceptEntry                                   ;/*  7  - BreakPoint             */
    .long   archFatalErrExceptEntry                                     ;/*  8  - Fatal Err              */
    .long   archIdlyExceptEntry                                         ;/*  9  - Idly                   */
    .long   archAutoIntEntry                                            ;/*  10 - Auto                   */
    .long   archReservedExceptEntry                                     ;/*  11 - Reserved               */
    .long   archReservedExceptEntry                                     ;/*  12 - Reserved               */
    .long   archReservedExceptEntry                                     ;/*  13 - Reserved               */
    .long   archReservedExceptEntry                                     ;/*  14 - Reserved               */
    .long   archReservedExceptEntry                                     ;/*  15 - Reserved               */
    .long   archTrap0ExceptEntry                                        ;/*  16 - Trap0                  */
    .long   archTrap1ExceptEntry                                        ;/*  17 - Trap1                  */
    .long   archTrap2ExceptEntry                                        ;/*  18 - Trap2                  */
    .long   archTrap3ExceptEntry                                        ;/*  19 - Trap3                  */
    .long   archReservedExceptEntry                                     ;/*  20 - Reserved               */
    .long   archReservedExceptEntry                                     ;/*  21 - Reserved               */
    .long   archTspendIntEntry                                          ;/*  22 - TSPEND                 */
    .long   archReservedExceptEntry                                     ;/*  23 - Reserved               */
    .long   archReservedExceptEntry                                     ;/*  24 - Reserved               */
    .long   archReservedExceptEntry                                     ;/*  25 - Reserved               */
    .long   archReservedExceptEntry                                     ;/*  26 - Reserved               */
    .long   archReservedExceptEntry                                     ;/*  27 - Reserved               */
    .long   archReservedExceptEntry                                     ;/*  28 - Reserved               */
    .long   archReservedExceptEntry                                     ;/*  29 - Reserved               */
    .long   archFpuExceptEntry                                          ;/*  30 - FPU                    */
    .long   archReservedExceptEntry                                     ;/*  31 - Reserved               */

    .irp    Id,                                       \
               32,  33,  34,  35,  36,  37,  38,  39, \
     40,  41,  42,  43,  44,  45,  46,  47,  48,  49, \
     50,  51,  52,  53,  54,  55,  56,  57,  58,  59, \
     60,  61,  62,  63,  64,  65,  66,  67,  68,  69, \
     70,  71,  72,  73,  74,  75,  76,  77,  78,  79, \
     80,  81,  82,  83,  84,  85,  86,  87,  88,  89, \
     90,  91,  92,  93,  94,  95,  96,  97,  98,  99, \
    100, 101, 102, 103, 104, 105, 106, 107, 108, 109, \
    110, 111, 112, 113, 114, 115, 116, 117, 118, 119, \
    120, 121, 122, 123, 124, 125, 126, 127, 128, 129, \
    130, 131, 132, 133, 134, 135, 136, 137, 138, 139, \
    140, 141, 142, 143, 144, 145, 146, 147, 148, 149, \
    150, 151, 152, 153, 154, 155, 156, 157, 158, 159, \
    160, 161, 162, 163, 164, 165, 166, 167, 168, 169, \
    170, 171, 172, 173, 174, 175, 176, 177, 178, 179, \
    180, 181, 182, 183, 184, 185, 186, 187, 188, 189, \
    190, 191, 192, 193, 194, 195, 196, 197, 198, 199, \
    200, 201, 202, 203, 204, 205, 206, 207, 208, 209, \
    210, 211, 212, 213, 214, 215, 216, 217, 218, 219, \
    220, 221, 222, 223, 224, 225, 226, 227, 228, 229, \
    230, 231, 232, 233, 234, 235, 236, 237, 238, 239, \
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, \
    250, 251, 252, 253, 254, 255
    .long   archIntEntry\Id
    .endr
    FUNC_END(vector)

    FILE_END()

#endif
;/*********************************************************************************************************
;  END
;*********************************************************************************************************/
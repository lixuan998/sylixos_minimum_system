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
;** ��   ��   ��: x86LibAsm.S
;**
;** ��   ��   ��: Jiao.JinXing (������)
;**
;** �ļ���������: 2016 �� 07 �� 04 ��
;**
;** ��        ��: x86 ��ϵ�����ڲ���.
;*********************************************************************************************************/

#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>
#include <config/kernel/cache_cfg.h>

    FILE_BEGIN()

    IMPORT_LABEL(__setjmpSetup)
    IMPORT_LABEL(__sigsetjmpSetup)
    IMPORT_LABEL(__longjmpSetup)
    IMPORT_LABEL(__siglongjmpSetup)
    IMPORT_LABEL(bspReboot)
    
    EXPORT_LABEL(setjmp)
    EXPORT_LABEL(sigsetjmp)
    EXPORT_LABEL(longjmp)
    EXPORT_LABEL(siglongjmp)
    
    EXPORT_LABEL(archFindLsb)
    EXPORT_LABEL(archFindMsb)

    EXPORT_LABEL(archIntDisable)
    EXPORT_LABEL(archIntEnable)
    EXPORT_LABEL(archIntEnableForce)
    EXPORT_LABEL(archReboot)
    EXPORT_LABEL(archBogoMipsLoop)

    WEAK(archIntDisable)
    WEAK(archIntEnable)
    WEAK(archIntEnableForce)

;/*********************************************************************************************************
;  x86 �ر����ж�
;*********************************************************************************************************/

FUNC_DEF(archIntDisable)
    PUSHFL
    CLI
    POPL    %EAX
    RET
    FUNC_END(archIntDisable)
    
FUNC_DEF(archIntEnable)
    MOVL    X86_SP_ARG1(%ESP) , %EAX
    TESTL   $X86_EFLAGS_IF , %EAX
    JZ      __archIntEnable
    STI
LINE_LABEL(__archIntEnable)
    RET
    FUNC_END(archIntEnable)
    
FUNC_DEF(archIntEnableForce)
    STI
    RET
    FUNC_END(archIntEnableForce)
    
;/*********************************************************************************************************
;  x86 ǰ��������
;*********************************************************************************************************/

FUNC_DEF(archFindLsb)
    MOVL    X86_SP_ARG1(%ESP) , %EDX                                    ;/*  %EDX = i                    */

    BSFL    %EDX , %EAX                                                 ;/*  �� 0 λ��ʼ����             */
    JNE     __archFindLsb                                               ;/*  0 ��ζ��û��λ������        */

    XORL    %EAX , %EAX                                                 ;/*  return 0                    */
    RET

LINE_LABEL(__archFindLsb)                                               ;/*  �ҵ���                      */
    INCL    %EAX                                                        ;/*  +1                          */
    RET
    FUNC_END(archFindLsb)


FUNC_DEF(archFindMsb)
    MOVL    X86_SP_ARG1(%ESP) , %EDX                                    ;/*  %EDX = i                    */

    BSRL    %EDX , %EAX                                                 ;/*  �� 31 λ��ʼ����            */
    JNE     __archFindMsb                                               ;/*  0 ��ζ��û��λ������        */

    XORL    %EAX , %EAX                                                 ;/*  return 0                    */
    RET

LINE_LABEL(__archFindMsb)                                               ;/*  �ҵ���                      */
    INCL    %EAX                                                        ;/*  +1                          */
    RET
    FUNC_END(archFindMsb)

;/*********************************************************************************************************
;  ע��: setjmp �� longjmp �����Ľṹ���߳������Ľṹ��ͬ
;*********************************************************************************************************/

#define _X86_EIP_OFFSET        0x00
#define _X86_EFLAGS_OFFSET     0x04
#define _X86_EDI_OFFSET        0x08
#define _X86_ESI_OFFSET        0x0c
#define _X86_EBP_OFFSET        0x10
#define _X86_EBX_OFFSET        0x14
#define _X86_ECX_OFFSET        0x18
#define _X86_CS_OFFSET         0x1c
#define _X86_ESP_OFFSET        0x20

;/*********************************************************************************************************
;  ����Ĵ�����
;*********************************************************************************************************/

MACRO_DEF(SAVE_NON_VOLATILE_REG)
    MOVL    X86_SP_ARG1(%ESP) , %EDX

    MOVL    0(%ESP) , %EAX                                              ;/*  ջ�������� EIP              */
    MOVL    %EAX , _X86_EIP_OFFSET(%EDX)                                ;/*  ���� EIP                    */

    PUSHFL
    POPL    _X86_EFLAGS_OFFSET(%EDX)                                    ;/*  ���� EFLAGS                 */

    MOVL    %EDI , _X86_EDI_OFFSET(%EDX)                                ;/*  ���� nonvolatile �Ĵ���     */
    MOVL    %ESI , _X86_ESI_OFFSET(%EDX)

    MOVL    %EBP , _X86_EBP_OFFSET(%EDX)

    MOVL    %EBX , _X86_EBX_OFFSET(%EDX)
    MOVL    %ECX , _X86_ECX_OFFSET(%EDX)

    MOVL    %CS  , %EAX
    MOVL    %EAX , _X86_CS_OFFSET(%EDX)                                 ;/*  ���� CS                     */

    MOVL    %ESP , %EAX
    ADDL    $4   , %EAX                                                 ;/*  EAX = EIP ��ջ��� ESP      */
    MOVL    %EAX , _X86_ESP_OFFSET(%EDX)                                ;/*  ���� ESP                    */
    MACRO_END()

;/*********************************************************************************************************
;  �ָ��Ĵ�����
;*********************************************************************************************************/

MACRO_DEF(RESTORE_NON_VOLATILE_REG)
    CLI                                                                 ;/*  �ر��ж�(�����ָ� EFLAGS) */

    MOVL    X86_SP_ARG1(%ESP) , %EDX
    MOVL    X86_SP_ARG2(%ESP) , %EAX

    MOVL    _X86_EDI_OFFSET(%EDX) , %EDI                                ;/*  �ָ� nonvolatile �Ĵ���     */
    MOVL    _X86_ESI_OFFSET(%EDX) , %ESI

    MOVL    _X86_EBP_OFFSET(%EDX) , %EBP

    MOVL    _X86_EBX_OFFSET(%EDX) , %EBX
    MOVL    _X86_ECX_OFFSET(%EDX) , %ECX

    MOVL    _X86_ESP_OFFSET(%EDX) , %ESP                                ;/*  �ָ� ESP                    */

    PUSHL   _X86_EFLAGS_OFFSET(%EDX)                                    ;/*  ѹջ����� EFLAGS           */
    PUSHL   _X86_CS_OFFSET(%EDX)                                        ;/*  ѹջ����� CS               */
    PUSHL   _X86_EIP_OFFSET(%EDX)                                       ;/*  ѹջ����� EIP(����� IRET) */
    MACRO_END()

;/*********************************************************************************************************
;  sigsetjmp (����Ϊ jmp_buf, mask_saved)
;*********************************************************************************************************/

FUNC_DEF(sigsetjmp)
    ;/*
    ; * VOID  __sigsetjmpSetup (sigjmp_buf sigjmpEnv, INT iSaveSigs)
    ; */
    PUSHL   X86_SP_ARG2(%ESP)
    PUSHL   (X86_SP_ARG1 + 4)(%ESP)
    CALL    __sigsetjmpSetup
    ADDL    $8 , %ESP

    SAVE_NON_VOLATILE_REG

    XORL    %EAX , %EAX                                                 ;/*  return 0                    */
    RET
    FUNC_END(sigsetjmp)

;/*********************************************************************************************************
;  siglongjmp (����Ϊ jmp_buf, retval)
;*********************************************************************************************************/

FUNC_DEF(siglongjmp)
    ;/*
    ; * VOID  __siglongjmpSetup (sigjmp_buf sigjmpEnv, INT iVal)
    ; */
    PUSHL   X86_SP_ARG2(%ESP)
    PUSHL   (X86_SP_ARG1 + 4)(%ESP)
    CALL    __siglongjmpSetup
    ADDL    $8 , %ESP

    RESTORE_NON_VOLATILE_REG

    CMPL    $0 , %EAX
    JNE     siglongjmpnotzero
    MOVL    $1 , %EAX

LINE_LABEL(siglongjmpnotzero)
    IRET                                                                ;/*  IRET ���ڵ��� CS EIP EFLAGS */
    FUNC_END(siglongjmp)

;/*********************************************************************************************************
;  setjmp (����Ϊ jmp_buf)
;*********************************************************************************************************/

FUNC_DEF(setjmp)
    ;/*
    ; * void __setjmpSetup (jmp_buf jmpbuf)
    ; */
    PUSHL   X86_SP_ARG1(%ESP)
    CALL    __setjmpSetup
    ADDL    $4 , %ESP

    SAVE_NON_VOLATILE_REG

    XORL    %EAX , %EAX                                                 ;/*  return 0                    */
    RET
    FUNC_END(setjmp)

;/*********************************************************************************************************
;  longjmp (����Ϊ jmp_buf, retval)
;*********************************************************************************************************/

FUNC_DEF(longjmp)
    ;/*
    ; * void __longjmpSetup (jmp_buf jmpbuf, int val)
    ; */
    PUSHL   X86_SP_ARG2(%ESP)
    PUSHL   (X86_SP_ARG1 + 4)(%ESP)
    CALL    __longjmpSetup
    ADDL    $8 , %ESP

    RESTORE_NON_VOLATILE_REG

    CMPL    $0 , %EAX
    JNE     longjmpnotzero
    MOVL    $1 , %EAX

LINE_LABEL(longjmpnotzero)
    IRET                                                                ;/*  IRET ���ڵ��� CS EIP EFLAGS */
    FUNC_END(longjmp)
    
;/*********************************************************************************************************
;  ϵͳ����
;*********************************************************************************************************/

FUNC_DEF(archReboot)
    JMP     bspReboot
    FUNC_END(archReboot)
    
;/*********************************************************************************************************
;  Bogo ѭ��
;*********************************************************************************************************/

FUNC_DEF(archBogoMipsLoop)
    MOVL    X86_SP_ARG1(%ESP) , %ECX
1:
    MOVL    $1 , %EAX
    MOVL    $2 , %EDX
    MOVL    $1 , %EAX
    MOVL    $2 , %EDX
    MOVL    $1 , %EAX
    MOVL    $2 , %EDX
    DEC     %ECX
    JNS     1b
    RET
    FUNC_END(archBogoMipsLoop)

    FILE_END()
   
;/*********************************************************************************************************
;  END
;*********************************************************************************************************/
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
;** ��   ��   ��: x64LibAsm.S
;**
;** ��   ��   ��: Jiao.JinXing (������)
;**
;** �ļ���������: 2017 �� 06 �� 05 ��
;**
;** ��        ��: x86-64 ��ϵ�����ڲ���.
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

    EXPORT_LABEL(archPageCopy)

    WEAK(archIntDisable)
    WEAK(archIntEnable)
    WEAK(archIntEnableForce)

;/*********************************************************************************************************
;  x86-64 �ر����ж�
;*********************************************************************************************************/

FUNC_DEF(archIntDisable)
    PUSHFQ
    CLI
    POP     X86_64_RETREG
    RET
    FUNC_END(archIntDisable)
    
FUNC_DEF(archIntEnable)
    TESTL   $X86_EFLAGS_IF , X86_64_ARG0DW
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
;  x86-64 ǰ��������
;*********************************************************************************************************/

FUNC_DEF(archFindLsb)
    BSFL    X86_64_ARG0DW , %EAX                                        ;/*  �� 0 λ��ʼ����             */
    JNE     __archFindLsb                                               ;/*  0 ��ζ��û��λ������        */

    XORL    %EAX , %EAX                                                 ;/*  return 0                    */
    RET

LINE_LABEL(__archFindLsb)                                               ;/*  �ҵ���                      */
    INCL    %EAX                                                        ;/*  +1                          */
    RET
    FUNC_END(archFindLsb)


FUNC_DEF(archFindMsb)
    BSRL    X86_64_ARG0DW , %EAX                                        ;/*  �� 31 λ��ʼ����            */
    JNE     __archFindMsb                                               ;/*  0 ��ζ��û��λ������        */

    XORL    %EAX , %EAX                                                 ;/*  return 0                    */
    RET

LINE_LABEL(__archFindMsb)                                               ;/*  �ҵ���                      */
    INCL    %EAX                                                        ;/*  +1                          */
    RET
    FUNC_END(archFindMsb)

;/*********************************************************************************************************
;  x86-64 ҳ����
;*********************************************************************************************************/

FUNC_DEF(archPageCopy)
    MOV      $512 /* 4096/8 */, %RCX
    REP
    MOVSQ
    RET
    FUNC_END(archPageCopy)

;/*********************************************************************************************************
;  ע��: setjmp �� longjmp �����Ľṹ���߳������Ľṹ��ͬ
;*********************************************************************************************************/

#define _X86_64_RIP_OFFSET      0x00
#define _X86_64_RFLAGS_OFFSET   0x08
#define _X86_64_CS_OFFSET       0x10
#define _X86_64_SS_OFFSET       0x18
#define _X86_64_RSP_OFFSET      0x20
#define _X86_64_RBP_OFFSET      0x28
#define _X86_64_RBX_OFFSET      0x30
#define _X86_64_R12_OFFSET      0x38
#define _X86_64_R13_OFFSET      0x40
#define _X86_64_R14_OFFSET      0x48
#define _X86_64_R15_OFFSET      0x50

;/*********************************************************************************************************
;  ����Ĵ�����
;*********************************************************************************************************/

MACRO_DEF(SAVE_NON_VOLATILE_REG)
    MOV     0(%RSP) , %RAX                                              ;/*  ջ�������� RIP              */
    MOV     %RAX , _X86_64_RIP_OFFSET(%RDI)                             ;/*  ���� RIP                    */

    PUSHFQ
    POP     _X86_64_RFLAGS_OFFSET(%RDI)                                 ;/*  ���� RFLAGS                 */

    MOV     %RBP , _X86_64_RBP_OFFSET(%RDI)                             ;/*  ���� nonvolatile �Ĵ���     */

    MOV     %RBX , _X86_64_RBX_OFFSET(%RDI)

    MOV     %R12 , _X86_64_R12_OFFSET(%RDI)
    MOV     %R13 , _X86_64_R13_OFFSET(%RDI)
    MOV     %R14 , _X86_64_R14_OFFSET(%RDI)
    MOV     %R15 , _X86_64_R15_OFFSET(%RDI)

    MOV     %CS  , %RAX
    MOV     %RAX , _X86_64_CS_OFFSET(%RDI)                              ;/*  ���� CS                     */

    MOV     %SS  , %RAX
    MOV     %RAX , _X86_64_SS_OFFSET(%RDI)                              ;/*  ���� SS                     */

    MOV     %RSP , %RAX
    ADD     $8   , %RAX                                                 ;/*  RAX = RIP ��ջ��� RSP      */
    MOV     %RAX , _X86_64_RSP_OFFSET(%RDI)                             ;/*  ���� RSP                    */
    MACRO_END()

;/*********************************************************************************************************
;  �ָ��Ĵ�����
;*********************************************************************************************************/

MACRO_DEF(RESTORE_NON_VOLATILE_REG)
    CLI                                                                 ;/*  �ر��ж�(�����ָ� RFLAGS) */

    MOV     X86_64_ARG0 , %RDX
    MOV     X86_64_ARG1 , %RAX

    MOV     _X86_64_RBP_OFFSET(%RDX) , %RBP                             ;/*  �ָ� nonvolatile �Ĵ���     */

    MOV     _X86_64_RBX_OFFSET(%RDX) , %RBX

    MOV     _X86_64_R12_OFFSET(%RDX) , %R12
    MOV     _X86_64_R13_OFFSET(%RDX) , %R13
    MOV     _X86_64_R14_OFFSET(%RDX) , %R14
    MOV     _X86_64_R15_OFFSET(%RDX) , %R15

    AND     $~15 , %RSP                                                 ;/*  RSP ���� 16 �ֽڶ���        */

    PUSH    _X86_64_SS_OFFSET(%RDX)                                     ;/*  ѹջ����� SS               */
    PUSH    _X86_64_RSP_OFFSET(%RDX)                                    ;/*  ѹջ����� RSP              */
    PUSH    _X86_64_RFLAGS_OFFSET(%RDX)                                 ;/*  ѹջ����� RFLAGS           */
    PUSH    _X86_64_CS_OFFSET(%RDX)                                     ;/*  ѹջ����� CS               */
    PUSH    _X86_64_RIP_OFFSET(%RDX)                                    ;/*  ѹջ����� RIP(����� IRETQ)*/
    MACRO_END()

;/*********************************************************************************************************
;  sigsetjmp (����Ϊ jmp_buf, mask_saved)
;*********************************************************************************************************/

FUNC_DEF(sigsetjmp)
    ;/*
    ; * VOID  __sigsetjmpSetup (sigjmp_buf sigjmpEnv, INT iSaveSigs)
    ; */
    PUSH    X86_64_ARG0
    PUSH    X86_64_ARG1
    CALL    __sigsetjmpSetup
    POP     X86_64_ARG1
    POP     X86_64_ARG0

    SAVE_NON_VOLATILE_REG

    MOVL    $0 , %EAX                                                   ;/*  return 0                    */
    RET
    FUNC_END(sigsetjmp)

;/*********************************************************************************************************
;  siglongjmp (����Ϊ jmp_buf, retval)
;*********************************************************************************************************/

FUNC_DEF(siglongjmp)
    ;/*
    ; * VOID  __siglongjmpSetup (sigjmp_buf sigjmpEnv, INT iVal)
    ; */
    PUSH    X86_64_ARG0
    PUSH    X86_64_ARG1
    CALL    __siglongjmpSetup
    POP     X86_64_ARG1
    POP     X86_64_ARG0

    RESTORE_NON_VOLATILE_REG

    CMPL    $0 , %EAX
    JNE     siglongjmpnotzero
    MOVL    $1 , %EAX

LINE_LABEL(siglongjmpnotzero)
    IRETQ                                                               ;/*  IRETQ ���ڵ��� CS RIP RFLAGS*/
    FUNC_END(siglongjmp)

;/*********************************************************************************************************
;  setjmp (����Ϊ jmp_buf)
;*********************************************************************************************************/

FUNC_DEF(setjmp)
    ;/*
    ; * void __setjmpSetup (jmp_buf jmpbuf)
    ; */
    PUSH    X86_64_ARG0
    CALL    __setjmpSetup
    POP     X86_64_ARG0

    SAVE_NON_VOLATILE_REG

    MOVL    $0 , %EAX                                                   ;/*  return 0                    */
    RET
    FUNC_END(setjmp)

;/*********************************************************************************************************
;  longjmp (����Ϊ jmp_buf, retval)
;*********************************************************************************************************/

FUNC_DEF(longjmp)
    ;/*
    ; * void __longjmpSetup (jmp_buf jmpbuf, int val)
    ; */
    PUSH    X86_64_ARG0
    PUSH    X86_64_ARG1
    CALL    __longjmpSetup
    POP     X86_64_ARG1
    POP     X86_64_ARG0

    RESTORE_NON_VOLATILE_REG

    CMPL    $0 , %EAX
    JNE     longjmpnotzero
    MOVL    $1 , %EAX

LINE_LABEL(longjmpnotzero)
    IRETQ                                                               ;/*  IRETQ ���ڵ��� CS RIP RFLAGS*/
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
1:
    MOV     $1 , X86_64_ARG1
    MOV     $2 , X86_64_ARG2
    MOV     $3 , X86_64_ARG3
    MOV     $1 , X86_64_ARG1
    MOV     $2 , X86_64_ARG2
    MOV     $3 , X86_64_ARG3
    DEC     X86_64_ARG0
    JNS     1b
    RET
    FUNC_END(archBogoMipsLoop)

    FILE_END()
   
;/*********************************************************************************************************
;  END
;*********************************************************************************************************/
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
;** ��   ��   ��: armExcV7MSvcAsm.S
;**
;** ��   ��   ��: Jiao.JinXing (������)
;**
;** �ļ���������: 2017 �� 11 �� 14 ��
;**
;** ��        ��: ARMv7M ��ϵ�����쳣����(SVC ��ʽ�����л�).
;*********************************************************************************************************/

#ifndef __ARMEXCV7MSVCASM_H
#define __ARMEXCV7MSVCASM_H

;/*********************************************************************************************************
;  �ж���ں�
;*********************************************************************************************************/

;/*********************************************************************************************************
;                                              IRQ_ENTRY
;                                                  +
;                                                  |
;                                                  |
;                                                  |
;                                                  v
;                                           API_InterEnter
;                                                  +
;                                                  |  �ж�Ƕ��
;                                                  |
;                           ���ж�ǰ               +--------------------------+
;                           ��                     |                          |
;                           �����л�ʱ����ǿռ     |                          |
;                         +------------------------+                          v
;                         |                        |                    SAVE_CTX_REG
;                         |                        v                          +
;                         |               API_InterStackBaseGet               |
;                         |                        +                          |
;                         v                        |                          v
;                   SAVE_CTX_REG                   |                     ISR_HANDLE
;                         +                        v                          +
;                         |                API_ThreadTcbInter                 |
;                         |                        +                          |
;                         v                        |                          |
;                    ISR_HANDLE                    |                          v
;                         +                        v                    API_InterExit
;                         |                  SAVE_CTX_REG                     +
;                         |                        +                          |
;                         |                        |                          |
;                         v                        |                          |
;               API_InterExitNoSched               v                          v
;                         +                   ISR_HANDLE                LOAD_CTX_REG
;                         |                        +
;                         |                        |
;                         v                        |
;                    SEND PendSV                   v
;                         +                  API_InterExit
;                         |
;                         |
;                         |
;                         v
;                   LOAD_CTX_REG
;*********************************************************************************************************/

MACRO_DEF(IRQ_ENTRY  irq  handle)
    ;/*
    ; * �쳣���Զ��� xPSR, PC, LR, R12 �Լ� R3-R0 ����ѹ���ʵ��Ķ�ջ�У�
    ; * ����쳣ǰ�Ĵ�������ʹ�� PSP, ��ѹ�� PSP, Ҳ����ʹ�ý��̶�ջ��
    ; * �����ѹ�� MSP, ʹ������ջ.
    ; * һ�������˷�������, �ͽ�һֱʹ������ջ
    ; */

    ;/*
    ; * R1 ��¼ BASEPRI, �����ж�(�������ȼ������ж�, ����ʹ�� SVC)
    ; */
    CPSID   I
    MRS     R1 , BASEPRI                                        ;/*  Set prio int mask to mask all       */
    MOV     R2 , #LW_CFG_CORTEX_M_INT_MASK                      ;/*  mask prio >= 2                      */
    MSR     BASEPRI , R2
    CPSIE   I

    ;/*
    ; * R0 �����쳣�Զ�ѹջǰʹ�õ� SP, ͨ�� EXC_RETURN(������ LR) �ж��쳣ǰʹ�ú��� SP
    ; */
    TST     LR , #CORTEX_M_EXC_RETURN_MODE_MASK                 ;/*  nonzero if context on process stack */
    ITE     EQ                                                  ;/*  next two instructions conditional   */
    MRSEQ   R0 , MSP
    MRSNE   R0 , PSP
    ADD     R0 , R0 , #(ARCH_HW_SAVE_CTX_SIZE)

    ;/*
    ; * ������ MSP, ��������ж�Ƕ��, ��Ҫ����Ĵ��������ĵ� MSP ָ���ջ�ռ���, Ԥ�� SW SAVE �Ĵ��������Ŀռ�
    ; */
    MRS     R2 , MSP
    SUB     R2 , R2 , #(ARCH_SW_SAVE_CTX_SIZE)
    MSR     MSP, R2

    PUSH    {R0 , LR}

    ;/*
    ; * API_InterEnter(�쳣�Զ�ѹջǰʹ�õ� SP, BASEPRI, EXC_RETURN) �����ж�,
    ; * ����ǵ�һ���ж�,   �Ὣ �쳣ǰ SP, BASEPRI, EXC_RETURN ���浽��ǰ���� TCB �� ARCH_REG_CTX ��
    ; * ������ǵ�һ���ж�, �Ὣ �쳣ǰ SP, BASEPRI, EXC_RETURN ���浽����Ԥ����ջ�ռ���
    ; */
    MOV     R2 , LR
    LDR     R3 , =API_InterEnter
    BLX     R3

    ;/*
    ; * ������ǵ�һ�ν����ж�, ��ô��һ���ж��Ѿ����� MSP
    ; */
    CMP     R0 , #1
    BNE     2f

    ;/*
    ; * �������ʹ�� R12����ΪӲ���Զ�������
    ; * �ж��Ƿ��ڹ��ж�ǰ���������л�ʱ������ռ
    ; */
    POP     {R0 , R12}
    TST     R12 , #CORTEX_M_EXC_RETURN_MODE_MASK
    BEQ     1f

    ;/*
    ; * ��һ�ν����ж�: ��õ�ǰ CPU �ж϶�ջջ��, ������ MSP
    ; */
    LDR     R3 , =API_InterStackBaseGet
    BLX     R3
    MSR     MSP , R0

    ;/*
    ; * ��õ�ǰ���� TCB �� ARCH_REG_CTX ��ַ
    ; */
    LDR     R3 , =API_ThreadTcbInter
    BLX     R3

    ;/*
    ; * ���� R4-R11 ����ǰ���� TCB �� ARCH_REG_CTX ��
    ; */
    ADD     R1 , R0 , #(2 * 4)                                  ;/*  ���� SP, BASEPRI �ı���             */
    STM     R1 , {R4-R11}                                       ;/*  ���� R4-R11, ���� EXC_RETURN �ı��� */

    LDR     R1 , [R0]                                           ;/*  R1 = �쳣�Զ�ѹջǰʹ�õ� SP        */
    SUB     R1 , R1 , #(ARCH_HW_SAVE_CTX_SIZE)                  ;/*  R1 = �쳣�Զ�ѹջ��ʹ�õ� SP        */

    ADD     R2 , R0 , #(ARCH_SW_SAVE_CTX_SIZE)

    LDM     R1 , {R4-R11}
    STM     R2 , {R4-R11}

    MOV     R1 , R0
3:
    ;/*
    ; * handle(vector, �Ĵ���������)
    ; */
    MOV     R0 , \irq
    LDR     R3 , =\handle
    BLX     R3

    ;/*
    ; * API_InterExit()
    ; * ���û�з����ж�Ƕ��, �� API_InterExit ����� archIntCtxLoad ����,
    ; * ע�� MSP ��û�е���, ������������(��Ϊ�´ν����ж�ʱ MSP ������ʹ���쳣��ȫѹջ��� SP)
    ; */
    LDR     R3 , =API_InterExit
    BLX     R3

    ;/*
    ; * ��������, ˵���������ж�Ƕ��
    ; */
    ;/*
    ; * �� MSP ָ��Ķ�ջ�ָ�ԭ SP, BASEPRI, R4-R11, EXC_RETURN(������ LR)�Ĵ���
    ; */
    MRS     R0 , MSP
    LDM     R0 , {R2-R11, LR}
    ADD     R0 , R0 , #(ARCH_SW_SAVE_CTX_SIZE)
    MSR     MSP , R0

    MSR     BASEPRI , R3

    ;/*
    ; * �� MSP ָ��Ķ�ջ�ָ� xPSR, PC, LR, R12 �Լ� R3-R0 �Ĵ���
    ; */
    BX      LR

2:
    POP     {R0 , R12}
    ;/*
    ; * �ж�Ƕ��
    ; */
    MRS     R1 , MSP
    ADD     R0 , R1 , #(2 * 4)                                  ;/*  ���� SP, BASEPRI �ı���             */
    STM     R0 , {R4-R11}                                       ;/*  ���� R4-R11, ���� EXC_RETURN �ı��� */

    B       3b

1:
    ;/*
    ; * �������˵���ڹ��ж�ǰ���������л�ʱ������ռ
    ; */
    MRS     R1 , MSP
    ADD     R0 , R1 , #(2 * 4)                                  ;/*  ���� SP, BASEPRI �ı���             */
    STM     R0 , {R4-R11}                                       ;/*  ���� R4-R11, ���� EXC_RETURN �ı��� */

    ;/*
    ; * handle(vector, �Ĵ���������)
    ; */
    MOV     R0 , \irq
    LDR     R3 , =\handle
    BLX     R3

    LDR     R3 , =API_InterExitNoSched
    BLX     R3

    ;/*
    ; * �� MSP ָ��Ķ�ջ�ָ�ԭ SP, BASEPRI, R4-R11, EXC_RETURN(������ LR)�Ĵ���
    ; */
    MRS     R1 , MSP
    LDM     R1 , {R2-R11, LR}
    ADD     R1 , R1 , #(ARCH_SW_SAVE_CTX_SIZE)
    MSR     MSP , R1

    CBZ     R0 , 4f
    ;/*
    ; * Send PendSV
    ; */
    LDR     R0 , =CORTEX_M_NVIC_INT_CTRL
    LDR     R1 , =CORTEX_M_NVIC_PENDSVSET
    STR     R1 , [R0]

4:
    MSR     BASEPRI , R3
    ;/*
    ; * �� MSP ָ��Ķ�ջ�ָ� xPSR, PC, LR, R12 �Լ� R3-R0 �Ĵ���
    ; */
    BX      LR
    MACRO_END()

;/*********************************************************************************************************
;  �쳣��ں�
;*********************************************************************************************************/

#define TRAP_ENTRY      IRQ_ENTRY

;/*********************************************************************************************************
;  SVC ��ں���
;*********************************************************************************************************/

FUNC_DEF(archSvcEntry)
    ;/*
    ; * R0 ���ڼ�¼�쳣��
    ; */

    ;/*
    ; * �쳣���Զ��� xPSR, PC, LR, R12 �Լ� R3-R0 ����ѹ���ʵ��Ķ�ջ�У�
    ; * ����쳣ǰ�Ĵ�������ʹ�� PSP, ��ѹ�� PSP, Ҳ����ʹ�ý��̶�ջ��
    ; * �����ѹ�� MSP, ʹ������ջ.
    ; * һ�������˷�������, �ͽ�һֱʹ������ջ
    ; */

    ;/*
    ; * R3 ��¼ BASEPRI, �����ж�(�������ȼ������ж�, ����ʹ�� SVC)
    ; */
    MRS     R3 , BASEPRI                                        ;/*  Set prio int mask to mask all       */
    MOV     R2 , #LW_CFG_CORTEX_M_INT_MASK                      ;/*  mask prio >= 2                      */
    MSR     BASEPRI , R2

    ;/*
    ; * R0 �����쳣�Զ�ѹջ��ʹ�õ� SP
    ; * R2 �����쳣�Զ�ѹջǰʹ�õ� SP, ͨ�� EXC_RETURN(������ LR) �ж��쳣ǰʹ�ú��� SP
    ; */
    TST     LR , #CORTEX_M_EXC_RETURN_MODE_MASK                 ;/*  nonzero if context on process stack */
    ITE     EQ                                                  ;/*  next two instructions conditional   */
    MRSEQ   R0 , MSP
    MRSNE   R0 , PSP
    ADD     R2 , R0 , #(ARCH_HW_SAVE_CTX_SIZE)

    ;/*
    ; * ������ MSP, ��Ҫ����Ĵ��������ĵ� MSP ָ���ջ�ռ���, Ԥ�� SW SAVE �Ĵ��������Ŀռ�
    ; */
    MRS     R1 , MSP
    SUB     R1 , R1 , #(ARCH_SW_SAVE_CTX_SIZE)
    MSR     MSP , R1

    ;/*
    ; * ����ԭ SP, BASEPRI, R4-R11, EXC_RETURN(������ LR)�Ĵ�����Ԥ�� SW SAVE �Ĵ��������Ŀռ�
    ; */
    STM     R1 , {R2-R11, LR}

    ;/*
    ; * armv7mSvcHandle(HW SAVE �Ĵ��������Ŀռ�, SW SAVE �Ĵ��������Ŀռ�)
    ; */
    LDR     R3 , =armv7mSvcHandle
    BLX     R3

    MRS     R1 , MSP
    ADD     R1 , R1 , #(ARCH_SW_SAVE_CTX_SIZE)
    MSR     MSP , R1

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
    FUNC_END()

;/*********************************************************************************************************
;  PendSV ��ں���
;*********************************************************************************************************/

FUNC_DEF(archPendSVEntry)
    ;/*
    ; * �쳣���Զ��� xPSR, PC, LR, R12 �Լ� R3-R0 ����ѹ���ʵ��Ķ�ջ�У�
    ; * ����쳣ǰ�Ĵ�������ʹ�� PSP, ��ѹ�� PSP, Ҳ����ʹ�ý��̶�ջ��
    ; * �����ѹ�� MSP, ʹ������ջ.
    ; * һ�������˷�������, �ͽ�һֱʹ������ջ
    ; */

    ;/*
    ; * R1 ��¼ BASEPRI, �����ж�(�������ȼ������ж�, ����ʹ�� SVC)
    ; */
    CPSID   I
    MRS     R1 , BASEPRI                                        ;/*  Set prio int mask to mask all       */
    MOV     R2 , #LW_CFG_CORTEX_M_INT_MASK                      ;/*  mask prio >= 2                      */
    MSR     BASEPRI , R2
    CPSIE   I

    ;/*
    ; * R0 �����쳣�Զ�ѹջǰʹ�õ� SP, ͨ�� EXC_RETURN(������ LR) �ж��쳣ǰʹ�ú��� SP
    ; */
    TST     LR , #CORTEX_M_EXC_RETURN_MODE_MASK                 ;/*  nonzero if context on process stack */
    ITE     EQ                                                  ;/*  next two instructions conditional   */
    MRSEQ   R0 , MSP
    MRSNE   R0 , PSP
    ADD     R0 , R0 , #(ARCH_HW_SAVE_CTX_SIZE)

    ;/*
    ; * API_InterEnter(�쳣�Զ�ѹջǰʹ�õ� SP, BASEPRI, EXC_RETURN) �����ж�,
    ; * ����ǵ�һ���ж�,   �Ὣ �쳣ǰ SP, BASEPRI, EXC_RETURN ���浽��ǰ���� TCB �� ARCH_REG_CTX ��
    ; * ������ǵ�һ���ж�, �Ὣ �쳣ǰ SP, BASEPRI, EXC_RETURN ���浽����Ԥ����ջ�ռ���
    ; */
    MOV     R2 , LR
    LDR     R3 , =API_InterEnter
    BLX     R3

    ;/*
    ; * ��õ�ǰ���� TCB �� ARCH_REG_CTX ��ַ
    ; */
    LDR     R3 , =API_ThreadTcbInter
    BLX     R3

    ;/*
    ; * ���� R4-R11 ����ǰ���� TCB �� ARCH_REG_CTX ��
    ; */
    ADD     R1 , R0 , #(2 * 4)                                  ;/*  ���� SP, BASEPRI �ı���             */
    STM     R1 , {R4-R11}                                       ;/*  ���� R4-R11, ���� EXC_RETURN �ı��� */

    LDR     R1 , [R0]                                           ;/*  R1 = �쳣�Զ�ѹջǰʹ�õ� SP        */
    SUB     R1 , R1 , #(ARCH_HW_SAVE_CTX_SIZE)                  ;/*  R1 = �쳣�Զ�ѹջ��ʹ�õ� SP        */

    ADD     R2 , R0 , #(ARCH_SW_SAVE_CTX_SIZE)

    LDM     R1 , {R4-R11}
    STM     R2 , {R4-R11}

    ;/*
    ; * ��һ�ν����ж�: ��õ�ǰ CPU �ж϶�ջջ��, ������ MSP
    ; */
    LDR     R3 , =API_InterStackBaseGet
    BLX     R3
    MSR     MSP , R0

    ;/*
    ; * API_InterExit()
    ; * ���û�з����ж�Ƕ��, �� API_InterExit ����� archIntCtxLoad ����,
    ; * ע�� MSP ��û�е���, ������������(��Ϊ�´ν����ж�ʱ MSP ������ʹ���쳣��ȫѹջ��� SP)
    ; */
    LDR     R3 , =API_InterExit
    BLX     R3

    ;/*
    ; * �޷�ִ�е���������л�
    ; */
    BX      LR
    FUNC_END()

#endif
;/*********************************************************************************************************
;  END
;*********************************************************************************************************/
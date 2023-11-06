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
;** ��   ��   ��: ppcLibAsm.S
;**
;** ��   ��   ��: Jiao.JinXing (������)
;**
;** �ļ���������: 2015 �� 12 �� 15 ��
;**
;** ��        ��: PowerPC ��ϵ�����ڲ���.
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
;  PowerPC �ر����ж�
;  ע��: ����û�йرջ�������쳣���ٽ������쳣(E500 ����)
;*********************************************************************************************************/

FUNC_DEF(archIntDisable)
    MFMSR   R3
    ISYNC
    OR      R4 , R3 , R3
    ARCH_PPC_INT_MASK(R4, R4)
    ISYNC
    MTMSR   R4
    ISYNC
    BLR
    FUNC_END()
    
FUNC_DEF(archIntEnable)
    SYNC
    RLWINM  R4 , R3 , 0 , ARCH_PPC_MSR_BIT_EE , ARCH_PPC_MSR_BIT_EE
    CMPWI   CR0, R4 , 0
    BEQ     CR0, __archIntIsDisable
    MFMSR   R3
    ISYNC
    ORI     R3 , R3 , ARCH_PPC_MSR_EE
    ISYNC
    MTMSR   R3
    ISYNC
LINE_LABEL(__archIntIsDisable)
    BLR
    FUNC_END()
    
FUNC_DEF(archIntEnableForce)
    MFMSR   R3
    ISYNC
    ORI     R3 , R3 , ARCH_PPC_MSR_EE
    ISYNC
    MTMSR   R3
    ISYNC
    BLR
    FUNC_END()
    
;/*********************************************************************************************************
;  PowerPC ǰ��������
;*********************************************************************************************************/

FUNC_DEF(archFindLsb)
    NEG     R4 , R3
    AND     R3 , R3 , R4
    CNTLZW  R3 , R3                                                     ;/*  R3 = # leading zeros        */
    SUBFIC  R3 , R3 , 32                                                ;/*  R3 = 32 - R3                */
    BLR
    FUNC_END()

FUNC_DEF(archFindMsb)
    CNTLZW  R3 , R3                                                     ;/*  R3 = # leading zeros        */
    SUBFIC  R3 , R3 , 32                                                ;/*  R3 = 32 - R3                */
    BLR
    FUNC_END()

;/*********************************************************************************************************
;  ע��: setjmp �� longjmp �����Ľṹ���߳������Ľṹ��ͬ
;
;  �����ĵ� <PowerPC-eabi_app.pdf>, һ�ε��ù���,  non volatile(����ʧ��)�ļĴ���������:
;  CR��CR2-CR4��  R14-R31  R1(SP)  F14-F31
;
;  setjmp �� longjmp �����Ľṹ�������:
;
;  �ߵ�ַ: +---------------+
;          |      CR       | (1 regs)
;          +---------------+
;          |   R14 - R31   | (18 regs)
;          +---------------+
;          |     SP(R1)    | (1 regs)
;          +---------------+
;          |      LR       | (1 regs)
;  �͵�ַ: +---------------+
;
;  F14-F31 ������, �� 21 ���Ĵ���
;
;*********************************************************************************************************/

;/*********************************************************************************************************
;  �������ú�����
;*********************************************************************************************************/

MACRO_DEF(CALL_SETUP, setup)
    SUBI    SP , SP , 32

    STW     R3 , 28(SP)
    STW     R4 , 24(SP)

    MFLR    R5
    ISYNC
    STW     R5 , 20(SP)
    SYNC

    BL      \setup

    LWZ     R5 , 20(SP)
    SYNC
    MTLR    R5
    ISYNC

    LWZ     R4 , 24(SP)
    LWZ     R3 , 28(SP)

    ADDI    SP , SP , 32
    MACRO_END()

;/*********************************************************************************************************
;  ����Ĵ�����
;*********************************************************************************************************/

MACRO_DEF(SAVE_NON_VOLATILE_REG)
    MFLR    R5
    ISYNC
    STW     R5 , 0(R3)
    SYNC

    STW     SP , 4(R3)

    STMW    R14, 8(R3)

    MFCR    R5
    ISYNC
    STW     R5 , 80(R3)
    SYNC
    MACRO_END()

;/*********************************************************************************************************
;  �ָ��Ĵ�����
;*********************************************************************************************************/

MACRO_DEF(RESTORE_NON_VOLATILE_REG)
    LWZ     R5 , 0(R3)
    SYNC
    MTLR    R5
    ISYNC

    LWZ     SP , 4(R3)

    LMW     R14, 8(R3)

    LWZ     R5 , 80(R3)
    SYNC
    MTCR    R5
    ISYNC
    MACRO_END()

;/*********************************************************************************************************
;  sigsetjmp (����Ϊ jmp_buf, mask_saved)
;*********************************************************************************************************/

FUNC_DEF(sigsetjmp)
    CALL_SETUP  __sigsetjmpSetup

    SAVE_NON_VOLATILE_REG

    XOR     R3 , R3 , R3
    BLR
    FUNC_END()

;/*********************************************************************************************************
;  setjmp (����Ϊ jmp_buf)
;*********************************************************************************************************/

FUNC_DEF(setjmp)
    CALL_SETUP  __setjmpSetup

    SAVE_NON_VOLATILE_REG

    XOR     R3 , R3 , R3
    BLR
    FUNC_END()
    
;/*********************************************************************************************************
;  siglongjmp (����Ϊ jmp_buf, retval)
;*********************************************************************************************************/

FUNC_DEF(siglongjmp)
    CALL_SETUP  __siglongjmpSetup

    RESTORE_NON_VOLATILE_REG

    OR      R3 , R4 , R4
    CMPWI   CR0, R4 , 0
    BNE     CR0, siglongjmpnotzero
    LI      R3 , 1
LINE_LABEL(siglongjmpnotzero)
    BLR
    FUNC_END()

;/*********************************************************************************************************
;  longjmp (����Ϊ jmp_buf, retval)
;*********************************************************************************************************/

FUNC_DEF(longjmp)
    CALL_SETUP  __longjmpSetup

    RESTORE_NON_VOLATILE_REG

    OR      R3 , R4 , R4
    CMPWI   CR0, R4 , 0
    BNE     CR0, longjmpnotzero
    LI      R3 , 1
LINE_LABEL(longjmpnotzero)
    BLR
    FUNC_END()
    
;/*********************************************************************************************************
;  ϵͳ����
;*********************************************************************************************************/

FUNC_DEF(archReboot)
    B       bspReboot
    FUNC_END()
    
;/*********************************************************************************************************
;  Bogo ѭ��
;*********************************************************************************************************/

FUNC_DEF(archBogoMipsLoop)
1:
    LI      R4 , 4
    LI      R5 , 5
    LI      R6 , 6
    LI      R4 , 4
    LI      R5 , 5
    LI      R6 , 6
    SUBI    R3 , R3 , 1
    CMPWI   CR0, R3 , 0
    BNE     CR0, 1b
    BLR
    FUNC_END()

    FILE_END()
   
;/*********************************************************************************************************
;  END
;*********************************************************************************************************/
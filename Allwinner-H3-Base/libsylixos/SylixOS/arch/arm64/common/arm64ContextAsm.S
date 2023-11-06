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
;** ��   ��   ��: arm64ContextAsm.S
;**
;** ��   ��   ��: Wang.Xuan (���Q)
;**
;** �ļ���������: 2018 �� 06 �� 23 ��
;**
;** ��        ��: ARM64 ��ϵ�����������л�.
;*********************************************************************************************************/

#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>
#include <config/cpu/cpu_cfg.h>
#include <config/kernel/kernel_cfg.h>
#include <config/mp/mp_cfg.h>

#include "arm64ContextAsm.h"

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
;  ����Ϊ��ǰ CPU ���ƿ�, �� X0 Ϊ��ǰ CPU ���ƿ�ָ��
;*********************************************************************************************************/

FUNC_DEF(archTaskCtxStart)
    LDR     X18, [X0]                                                   ;/*  ��ȡ��ǰ TCB �� REG_CTX ��ַ*/

#if LW_CFG_ARM64_HW_STEP_EN > 0
    MOV     X0,        #0
    MSR     OSLAR_EL1, X0                                               ;/*  ���� OS Lock                */
    MOV     X0 ,       MDSCR_EL1_KDE_EN | MDSCR_EL1_MDE_EN
    MSR     MDSCR_EL1, X0                                               ;/*  ʹ�� debug ����             */
#endif

LINE_LABEL(archTaskCtxLoad)
    RESTORE_SMALL_REG_CTX                                               ;/*  �ָ�С�Ĵ���������          */
    FUNC_END()

;/*********************************************************************************************************
;  �߳��л� 
;  ����Ϊ��ǰ CPU ���ƿ�, �� X0 Ϊ��ǰ CPU ���ƿ�ָ��
;*********************************************************************************************************/

FUNC_DEF(archTaskCtxSwitch)
    LDR     X18, [X0]                                                   ;/*  ��ȡ��ǰ TCB �� REG_CTX ��ַ*/
    
    SAVE_SMALL_REG_CTX                                                  ;/*  ����С�Ĵ���������          */

    MOV     X19 , X0                                                    ;/*  X19 �ݴ� X0                 */

#if LW_CFG_SMP_EN > 0    
    BL      _SchedSafeStack                                             ;/*  _SchedSafeStack();          */
    MOV     SP , X0                                                     ;/*  ���� SP                     */
    MOV     X0 , X19                                                    ;/*  �ָ� X0                     */
#endif

    BL      _SchedSwp                                                   ;/*  _SchedSwp();                */

    LDR     X18, [X19]                                                  ;/*  ��ȡ��ǰ TCB �� REG_CTX ��ַ*/
    LDR     X9 , [X18, #CTX_TYPE_OFFSET]                                ;/*  �������������              */
    CMP     X9 , #0
    B.NE    _RestoreSmallCtx

    RESTORE_BIG_REG_CTX                                                 ;/*  �ָ���Ĵ���������          */

LINE_LABEL(_RestoreSmallCtx)
    RESTORE_SMALL_REG_CTX                                               ;/*  �ָ�С�Ĵ���������          */
    FUNC_END()

;/*********************************************************************************************************
;  Э���л� 
;  ����Ϊ��ǰ CPU ���ƿ�, �� X0 Ϊ��ǰ CPU ���ƿ�ָ��
;*********************************************************************************************************/

#if LW_CFG_COROUTINE_EN > 0
FUNC_DEF(archCrtCtxSwitch)
    ADD     X1  , X0 , #16
    LDR     X18 , [X1]                                                  ;/*  ��ȡ��ǰ CCB �� REG_CTX ��ַ*/

    SAVE_SMALL_REG_CTX                                                  ;/*  ����С�Ĵ���������          */

    MOV     X19 , X0                                                    ;/*  X19 �ݴ� X0                 */
    BL      _SchedCrSwp                                                 ;/*  _SchedCrSwp();              */
    MOV     X0  , X19

    ADD     X1  , X0 , #16
    LDR     X18 , [X1]                                                  ;/*  ��ȡ��ǰ CCB �� REG_CTX ��ַ*/

    RESTORE_SMALL_REG_CTX                                               ;/*  �ָ�С�Ĵ���������          */
    FUNC_END()
#endif

;/*********************************************************************************************************
;  �жϷ���ʱ, �߳�װ�� 
;  ����Ϊ��ǰ CPU ���ƿ�, �� X0 Ϊ��ǰ CPU ���ƿ�ָ��
;*********************************************************************************************************/

FUNC_DEF(archIntCtxLoad)
    LDR     X18 , [X0]                                                  ;/*  ��ȡ��ǰ TCB �� REG_CTX ��ַ*/
    LDR     X9  , [X18, #CTX_TYPE_OFFSET]                               ;/*  �������������              */
    CMP     X9  , #0
    B.NE    _RestoreSmallCtx

    RESTORE_BIG_REG_CTX                                                 ;/*  �ָ���Ĵ���������          */
    FUNC_END()

;/*********************************************************************************************************
;  �ź������ķ���
;  ����Ϊ ARCH_REG_CTX ָ��
;*********************************************************************************************************/

FUNC_DEF(archSigCtxLoad)
    MOV     X18 , X0
    LDR     X9  , [X18, #CTX_TYPE_OFFSET]                               ;/*  �������������              */
    CMP     X9  , #0
    B.NE    _RestoreSmallCtx

    RESTORE_BIG_REG_CTX                                                 ;/*  �ָ���Ĵ���������          */
    FUNC_END()

;/*********************************************************************************************************
;  ��������������
;  ���� X0 ΪĿ�� ARCH_REG_CTX ָ��, X1 ΪԴ ARCH_REG_CTX ָ��
;  ������ X18 �Ĵ���
;*********************************************************************************************************/

FUNC_DEF(archTaskCtxCopy)
    LDR     X9 ,       [X1 , #CTX_TYPE_OFFSET]
    STR     X9 ,       [X1 , #CTX_TYPE_OFFSET]

    LDP     X9 , X10 , [X1 , #XGREG_OFFSET(0)]
    STP     X9 , X10 , [X0 , #XGREG_OFFSET(0)]

    LDP     X9 , X10 , [X1 , #XGREG_OFFSET(2)]
    STP     X9 , X10 , [X0 , #XGREG_OFFSET(2)]

    LDP     X9 , X10 , [X1 , #XGREG_OFFSET(4)]
    STP     X9 , X10 , [X0 , #XGREG_OFFSET(4)]

    LDP     X9 , X10 , [X1 , #XGREG_OFFSET(6)]
    STP     X9 , X10 , [X0 , #XGREG_OFFSET(6)]

    LDP     X9 , X10 , [X1 , #XGREG_OFFSET(8)]
    STP     X9 , X10 , [X0 , #XGREG_OFFSET(8)]

    LDP     X9 , X10 , [X1 , #XGREG_OFFSET(10)]
    STP     X9 , X10 , [X0 , #XGREG_OFFSET(10)]

    LDP     X9 , X10 , [X1 , #XGREG_OFFSET(12)]
    STP     X9 , X10 , [X0 , #XGREG_OFFSET(12)]

    LDP     X9 , X10 , [X1 , #XGREG_OFFSET(14)]
    STP     X9 , X10 , [X0 , #XGREG_OFFSET(14)]

    LDP     X9 , X10 , [X1 , #XGREG_OFFSET(16)]
    STP     X9 , X10 , [X0 , #XGREG_OFFSET(16)]

    LDR     X9 , [X1 , #XGREG_OFFSET(19)]
    STR     X9 , [X0 , #XGREG_OFFSET(19)]

    LDP     X9 , X10 , [X1 , #XGREG_OFFSET(20)]
    STP     X9 , X10 , [X0 , #XGREG_OFFSET(20)]

    LDP     X9 , X10 , [X1 , #XGREG_OFFSET(22)]
    STP     X9 , X10 , [X0 , #XGREG_OFFSET(22)]

    LDP     X9 , X10 , [X1 , #XGREG_OFFSET(24)]
    STP     X9 , X10 , [X0 , #XGREG_OFFSET(24)]

    LDP     X9 , X10 , [X1 , #XGREG_OFFSET(26)]
    STP     X9 , X10 , [X0 , #XGREG_OFFSET(26)]

    LDP     X9 , X10 , [X1 , #XGREG_OFFSET(28)]
    STP     X9 , X10 , [X0 , #XGREG_OFFSET(28)]

    LDP     X9 , X10 , [X1 , #XGREG_OFFSET(30)]                         ;/*  LR��PC �Ĵ���               */
    STP     X9 , X10 , [X0 , #XGREG_OFFSET(30)]

    LDP     X9 , X10 , [X1 , #XGREG_OFFSET(32)]                         ;/*  SP��PSTATE �Ĵ���           */
    STP     X9 , X10 , [X0 , #XGREG_OFFSET(32)]
    
    RET
    FUNC_END()

    FILE_END()

;/*********************************************************************************************************
;  END
;*********************************************************************************************************/
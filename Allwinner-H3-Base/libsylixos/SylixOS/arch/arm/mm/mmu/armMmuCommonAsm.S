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
;** ��   ��   ��: armMmuCommon.S
;**
;** ��   ��   ��: Han.Hui (����)
;**
;** �ļ���������: 2013 �� 12 �� 09 ��
;**
;** ��        ��: ARM ��ϵ�ܹ� MMU ͨ�ú���֧��.
;*********************************************************************************************************/

#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>
#include <config/kernel/vmm_cfg.h>

#if !defined(__SYLIXOS_ARM_ARCH_M__)

    FILE_BEGIN()

    EXPORT_LABEL(armMmuAbtFaultStatus)
    EXPORT_LABEL(armMmuPreFaultStatus)
    EXPORT_LABEL(armMmuAbtFaultAddr)

#if LW_CFG_VMM_EN > 0
    EXPORT_LABEL(armMmuInitSysRom)
    EXPORT_LABEL(armMmuEnable)
    EXPORT_LABEL(armMmuDisable)
    EXPORT_LABEL(armMmuEnableWriteBuffer)
    EXPORT_LABEL(armMmuDisableWriteBuffer)
    EXPORT_LABEL(armMmuEnableAlignFault)
    EXPORT_LABEL(armMmuDisableAlignFault)
    EXPORT_LABEL(armMmuSetDomain)
    EXPORT_LABEL(armMmuSetTTBase)
    EXPORT_LABEL(armMmuSetTTBase1)
    EXPORT_LABEL(armMmuInvalidateTLB)
    EXPORT_LABEL(armMmuInvalidateTLBMVA)
    EXPORT_LABEL(armMmuSetProcessId)
#endif
     
;/*********************************************************************************************************
;  ��ȡ������Ϣ
;*********************************************************************************************************/

FUNC_DEF(armMmuAbtFaultStatus)
    MRC     p15, 0, R0, c5, c0, 0
    BX      LR
    FUNC_END()
    
FUNC_DEF(armMmuPreFaultStatus)
    MRC     p15, 0, R0, c5, c0, 1
    BX      LR
    FUNC_END()
    
FUNC_DEF(armMmuAbtFaultAddr)
    MRC     p15, 0, R0, c6, c0, 0
    BX      LR
    FUNC_END()

;/*********************************************************************************************************
;  ��ʼ�� SYS ROM
;*********************************************************************************************************/
#if LW_CFG_VMM_EN > 0

FUNC_DEF(armMmuInitSysRom)
    MRC     p15, 0, R0, c1, c0, 0
    BIC     R0, R0, #P15_R1_SYS
    ORR     R0, R0, #P15_R1_ROM
    MCR     p15, 0, R0, c1, c0, 0
    BX      LR
    FUNC_END()
    
;/*********************************************************************************************************
;  ʹ��/���� MMU
;*********************************************************************************************************/

FUNC_DEF(armMmuEnable)
#if __SYLIXOS_ARM_ARCH__ >= 6
    ARM_DSB()
    ARM_ISB()
#else
    ARM_NOP(R0)                                                         ;/* 2 * NOP                      */
    ARM_NOP(R0)
#endif
    MRC     p15, 0, R0, c1, c0, 0
    ORR     R0, R0, #P15_R1_M
    MCR     p15, 0, R0, c1, c0, 0
#if __SYLIXOS_ARM_ARCH__ >= 6
    ARM_DSB()
    ARM_ISB()
#else
    ARM_NOP(R0)                                                         ;/* 2 * NOP                      */
    ARM_NOP(R0)
#endif
    BX      LR
    FUNC_END()
    
FUNC_DEF(armMmuDisable)
#if __SYLIXOS_ARM_ARCH__ >= 6
    ARM_DSB()
    ARM_ISB()
#else
    ARM_NOP(R0)                                                         ;/* 2 * NOP                      */
    ARM_NOP(R0)
#endif
    MRC     p15, 0, R0, c1, c0, 0
    BIC     R0, R0, #P15_R1_M
    MCR     p15, 0, R0, c1, c0, 0
#if __SYLIXOS_ARM_ARCH__ >= 6
    ARM_DSB()
    ARM_ISB()
#else
    ARM_NOP(R0)                                                         ;/* 2 * NOP                      */
    ARM_NOP(R0)
#endif
    BX      LR
    FUNC_END()
    
;/*********************************************************************************************************
;  ʹ��/���� д����
;*********************************************************************************************************/

FUNC_DEF(armMmuEnableWriteBuffer)
    MRC     p15, 0, R0, c1, c0, 0
    ORR     R0, R0, #P15_R1_W
    MCR     p15, 0, R0, c1, c0, 0
    BX      LR
    FUNC_END()
    
FUNC_DEF(armMmuDisableWriteBuffer)
    MRC     p15, 0, R0, c1, c0, 0
    BIC     R0, R0, #P15_R1_W
    MCR     p15, 0, R0, c1, c0, 0
    BX      LR
    FUNC_END()
    
;/*********************************************************************************************************
;  ʹ��/���� ��ַ������
;*********************************************************************************************************/

FUNC_DEF(armMmuEnableAlignFault)
    MRC     p15, 0, R0, c1, c0, 0
    ORR     R0, R0, #P15_R1_A
    MCR     p15, 0, R0, c1, c0, 0
    BX      LR
    FUNC_END()
    
FUNC_DEF(armMmuDisableAlignFault)
    MRC     p15, 0, R0, c1, c0, 0
    BIC     R0, R0, #P15_R1_A
    MCR     p15, 0, R0, c1, c0, 0
    BX      LR
    FUNC_END()
    
;/*********************************************************************************************************
;  ����������
;*********************************************************************************************************/

FUNC_DEF(armMmuSetDomain)
    MCR     p15, 0, R0, c3, c0, 0
    BX      LR
    FUNC_END()
    
;/*********************************************************************************************************
;  ����ҳ������ַ TTBR0 Ϊ�������ҳ��  TTBR1 Ϊϵͳȫ��ҳ��
;*********************************************************************************************************/

FUNC_DEF(armMmuSetTTBase)
    ARM_DSB()
    ARM_ISB()
    MCR     p15, 0, R0, c2, c0, 0                                       ;/* Set TT Base                  */

#if __SYLIXOS_ARM_ARCH__ >= 6
    MOV     R0, #0
#if LW_CFG_SMP_EN > 0
    SMP_UP_CHECK(R1)
    MCRHI   p15, 0, R0, c7, c1, 6                                       ;/* BPIALLIS SMP                 */
    MCRLS   p15, 0, R0, c7, c5, 6                                       ;/* BPIALL UP                    */
#else
    MCR     p15, 0, R0, c7, c5, 6                                       ;/* BPIALL UP                    */
#endif
#endif
    ARM_NOP(R0)                                                         ;/* 3 * NOP                      */
    ARM_NOP(R0)
    ARM_NOP(R0)
    ARM_DSB()
    ARM_ISB()
    BX      LR
    FUNC_END()
    
FUNC_DEF(armMmuSetTTBase1)
    ARM_DSB()
    ARM_ISB()
    MCR     p15, 0, R0, c2, c0, 1

#if __SYLIXOS_ARM_ARCH__ >= 6
    MOV     R0, #0
#if LW_CFG_SMP_EN > 0
    SMP_UP_CHECK(R1)
    MCRHI   p15, 0, R0, c7, c1, 6                                       ;/* BPIALLIS SMP                 */
    MCRLS   p15, 0, R0, c7, c5, 6                                       ;/* BPIALL UP                    */
#else
    MCR     p15, 0, R0, c7, c5, 6                                       ;/* BPIALL UP                    */
#endif
#endif
    ARM_NOP(R0)                                                         ;/* 3 * NOP                      */
    ARM_NOP(R0)
    ARM_NOP(R0)
    ARM_DSB()
    ARM_ISB()
    BX      LR
    FUNC_END()
    
;/*********************************************************************************************************
;  TLB ��ز���
;*********************************************************************************************************/

FUNC_DEF(armMmuInvalidateTLB)                                           ;/* ����е���������Ч           */
    ARM_DSB()
    ARM_ISB()
    MOV     R0, #0
#if LW_CFG_SMP_EN > 0
    SMP_UP_CHECK(R1)
    MCRHI   p15, 0, R0, c8, c3, 0                                       ;/* TLBIALLIS SMP                */
    MCRLS   p15, 0, R0, c8, c7, 0                                       ;/* TLBIALL UP                   */
#if __SYLIXOS_ARM_ARCH__ >= 6
    ARM_ISB()
    MCRHI   p15, 0, R0, c7, c1, 6                                       ;/* BPIALLIS SMP                 */
    MCRLS   p15, 0, R0, c7, c5, 6                                       ;/* BPIALL UP                    */
#endif
#else
    MCR     p15, 0, R0, c8, c7, 0                                       ;/* TLBIALL UP                   */
#if __SYLIXOS_ARM_ARCH__ >= 6
    ARM_ISB()
    MCR     p15, 0, R0, c7, c5, 6                                       ;/* BPIALL UP                    */
#endif
#endif
    ARM_NOP(R0)                                                         ;/* 3 * NOP                      */
    ARM_NOP(R0)
    ARM_NOP(R0)
    ARM_DSB()
    ARM_ISB()
    BX      LR
    FUNC_END()
    
FUNC_DEF(armMmuInvalidateTLBMVA)                                        ;/* ��Ч���ָ����ַ�任��Ŀ     */
    ARM_DSB()
    ARM_ISB()
#if LW_CFG_SMP_EN > 0
    SMP_UP_CHECK(R1)
    MCRHI   p15, 0, R0, c8, c3, 1                                       ;/* TLBIMVAIS SMP                */
    MCRLS   p15, 0, R0, c8, c7, 1                                       ;/* TLBIMVA UP                   */
#if __SYLIXOS_ARM_ARCH__ >= 6
    ARM_ISB()
    MOV     R0, #0
    MCRHI   p15, 0, R0, c7, c1, 6                                       ;/* BPIALLIS SMP                 */
    MCRLS   p15, 0, R0, c7, c5, 6                                       ;/* BPIALL UP                    */
#endif
#else
    MCR     p15, 0, R0, c8, c7, 1                                       ;/* TLBIMVA UP                   */
#if __SYLIXOS_ARM_ARCH__ >= 6
    ARM_ISB()
    MOV     R0, #0
    MCR     p15, 0, R0, c7, c5, 6                                       ;/* BPIALL UP                    */
#endif
#endif
    ARM_NOP(R0)                                                         ;/* 3 * NOP                      */
    ARM_NOP(R0)
    ARM_NOP(R0)
    ARM_DSB()
    ARM_ISB()
    BX      LR
    FUNC_END()

;/*********************************************************************************************************
;  ���ý��� ID
;*********************************************************************************************************/

FUNC_DEF(armMmuSetProcessId)
    MCR     p15, 0, R0, c13, c0, 0
    BX      LR
    FUNC_END()

#endif

    FILE_END()

#endif
;/*********************************************************************************************************
;  END
;*********************************************************************************************************/
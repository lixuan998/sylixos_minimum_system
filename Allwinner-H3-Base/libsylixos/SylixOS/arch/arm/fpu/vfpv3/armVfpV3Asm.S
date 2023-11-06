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
;** ��   ��   ��: armVfpV3Asm.S
;**
;** ��   ��   ��: Han.Hui (����)
;**
;** �ļ���������: 2013 �� 12 �� 07 ��
;**
;** ��        ��: ARM ��ϵ�ܹ� VFPv3 ֧��.
;*********************************************************************************************************/

#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>

#if !defined(__SYLIXOS_ARM_ARCH_M__)

#if LW_CFG_CPU_FPU_EN > 0

    FILE_BEGIN()
    
    EXPORT_LABEL(armVfpV3Save16)
    EXPORT_LABEL(armVfpV3Restore16)
    EXPORT_LABEL(armVfpV3Save32)
    EXPORT_LABEL(armVfpV3Restore32)
    
;/*********************************************************************************************************
;  ����������������������
;*********************************************************************************************************/
;/*********************************************************************************************************
;  VFP-D16
;   +-----------+
;   | freg[31]  |    + 0x98  <-- (r0 + 152)
;   |  ...      |
;   | freg[2]   |    + 0x24
;   | freg[1]   |    + 0x20
;   | freg[0]   |    + 0x1C  <-- (r0 + 28)
;   | mfvfr1    |    + 0x18
;   | mfvfr0    |    + 0x14
;   | fpinst2   |    + 0x10
;   | fpinst    |    + 0x0C
;   | fpexc     |    + 0x08
;   | fpscr     |    + 0x04
;   | fpsid     | <-- cpu_fpu_context ( = r0 )
;   +-----------+
;*********************************************************************************************************/

#define VFP_FPSID       0x00
#define VFP_FPSCR       0x04
#define VFP_FPEXC       0x08
#define VFP_FPINST      0x0C
#define VFP_FPINST2     0x10
#define VFP_MVVFR0      0x14
#define VFP_MVVFR1      0x18
#define VFP_GPR         0x1C

;/*********************************************************************************************************
;  VFPv3 �ָ����ƼĴ�����
;*********************************************************************************************************/

MACRO_DEF(VFPv3_SAVE_CTRL)
    FMRX    R1, FPEXC
    ORR     R1, R1, #(1 << 30)
    FMXR    FPEXC, R1

    STR     R1, [R0, #VFP_FPEXC]        ;/*  ���� FPEXC                                                  */

    FMRX    R2, FPSCR
    STR     R2, [R0, #VFP_FPSCR]        ;/*  ���� FPSCR                                                  */

    ORR     R1, R1, #((1 << 28) | (1 << 31))
    FMXR    FPEXC, R1                   ;/*  Set FPEXC.EX=1 and FPEXC.FP2V=1                             */

    FMRX    R1, FPEXC                   ;/*  Read back the FPEXC register                                */
    TST     R1, #(1 << 31)              ;/*  if FPEXC.EX == 0 then                                       */
    BEQ     1f                          ;/*  Neither FPINST nor FPINST2 are implemented                  */

    FMRX    R2, FPINST                  ;/*  FPINST is implemented                                       */
    STR     R2, [R0, #VFP_FPINST]       ;/*  ���� FPINST                                                 */

    TST     R1, #(1 << 28)              ;/*  if FPEXC.FP2V == 0 then                                     */
    BEQ     1f                          ;/*  FPINST2 is not implemented                                  */

    FMRX    R3, FPINST2                 ;/*  FPINST2 is implemented                                      */
    STR     R3, [R0, #VFP_FPINST2]      ;/*  ���� FPINST2                                                */
1:
    ADD     R1, R0, #VFP_GPR
    MACRO_END()

;/*********************************************************************************************************
;  VFPv3 �ָ����ƼĴ�����
;*********************************************************************************************************/

MACRO_DEF(VFPv3_RESTORE_CTRL)
    FMRX    R1, FPEXC
    ORR     R1, R1, #((1 << 28) | (1 << 30) | (1 << 31))
    FMXR    FPEXC, R1                   ;/*  Set FPEXC.EX=1 and FPEXC.FP2V=1                             */

    FMRX    R1, FPEXC                   ;/*  Read back the FPEXC register                                */
    TST     R1, #(1 << 31)              ;/*  if FPEXC.EX == 0 then                                       */
    BEQ     1f                          ;/*  Neither FPINST nor FPINST2 are implemented                  */

    LDR     R2, [R0, #VFP_FPINST]       ;/*  FPINST is implemented                                       */
    FMXR    FPINST, R2                  ;/*  �ָ� FPINST                                                 */

    TST     R1, #(1 << 28)              ;/*  if FPEXC.FP2V == 0 then                                     */
    BEQ     1f                          ;/*  FPINST2 is not implemented                                  */

    LDR     R3, [R0, #VFP_FPINST2]      ;/*  FPINST2 is implemented                                      */
    FMXR    FPINST2, R3                 ;/*  �ָ� FPINST2                                                */
1:
    LDR     R1, [R0, #VFP_FPEXC]
    FMXR    FPEXC, R1                   ;/*  �ָ� FPEXC                                                  */

    LDR     R1, [R0, #VFP_FPSCR]
    FMXR    FPSCR, R1                   ;/*  �ָ� FPSCR                                                  */

    ADD     R1, R0, #VFP_GPR
    MACRO_END()

;/*********************************************************************************************************
;  VFPv3 ����/�ָ��Ĵ���
;*********************************************************************************************************/

FUNC_DEF(armVfpV3Save16)
    VFPv3_SAVE_CTRL
    FSTMIAD R1, {d0-d15}                ;/*  ���� D0-D15                                                 */
    BX      LR
    FUNC_END()

FUNC_DEF(armVfpV3Restore16)
    VFPv3_RESTORE_CTRL
    FLDMIAD R1, {d0-d15}                ;/*  �ָ� D0-D15                                                */
    BX      LR
    FUNC_END()
    
;/*********************************************************************************************************
;  VFP-D32
;   +-----------+
;   | freg[63]  |    + 0x118 <-- (r0 + 280)
;   |  ...      |
;   | freg[31]  |    + 0x98  <-- (r0 + 152)
;   |  ...      |
;   | freg[2]   |    + 0x24
;   | freg[1]   |    + 0x20
;   | freg[0]   |    + 0x1C  <-- (r0 + 28)
;   | mfvfr1    |    + 0x18
;   | mfvfr0    |    + 0x14
;   | fpinst2   |    + 0x10
;   | fpinst    |    + 0x0C
;   | fpexc     |    + 0x08
;   | fpscr     |    + 0x04
;   | fpsid     | <-- cpu_fpu_context ( = r0 )
;   +-----------+
;*********************************************************************************************************/

FUNC_DEF(armVfpV3Save32)
    VFPv3_SAVE_CTRL
    FSTMIAD R1!, {d0-d15}               ;/*  ���� D0-D15                                                 */
    FSTMIAD R1,  {d16-d31}              ;/*  ���� D16-D31                                                */
    BX      LR
    FUNC_END()
    
FUNC_DEF(armVfpV3Restore32)
    VFPv3_RESTORE_CTRL
    FLDMIAD R1!, {d0-d15}               ;/*  �ָ� D0-D15                                                 */
    FLDMIAD R1,  {d16-d31}              ;/*  �ָ� D16-D31                                                */
    BX      LR
    FUNC_END()
    
    FILE_END()

#endif
#endif
;/*********************************************************************************************************
;  END
;*********************************************************************************************************/
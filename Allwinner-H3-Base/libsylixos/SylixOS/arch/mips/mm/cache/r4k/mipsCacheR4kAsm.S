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
;** ��   ��   ��: mipsCacheR4kAsm.S
;**
;** ��   ��   ��: Jiao.JinXing (������)
;**
;** �ļ���������: 2015 �� 12 �� 01 ��
;**
;** ��        ��: MIPS R4K ��ϵ���� CACHE ����.
;*********************************************************************************************************/

#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>
#include <config/kernel/cache_cfg.h>
#include <config/cpu/cpu_cfg_mips.h>

#if LW_CFG_CACHE_EN > 0

    EXPORT_LABEL(mipsCacheR4kDisableHw)
    EXPORT_LABEL(mipsCacheR4kEnableHw)

    EXPORT_LABEL(mipsDCacheR4kLineFlush)
    EXPORT_LABEL(mipsDCacheR4kLineClear)
    EXPORT_LABEL(mipsDCacheR4kLineInvalidate)
    EXPORT_LABEL(mipsDCacheR4kIndexClear)
    EXPORT_LABEL(mipsDCacheR4kIndexStoreTag)

    EXPORT_LABEL(mipsICacheR4kLineInvalidate)
    EXPORT_LABEL(mipsICacheR4kIndexInvalidate)
    EXPORT_LABEL(mipsICacheR4kFill)
    EXPORT_LABEL(mipsICacheR4kIndexStoreTag)

    IMPORT_LABEL(mipsICacheR4kInvalidateAll)
    IMPORT_LABEL(mipsDCacheR4kClearAll)
#if LW_CFG_MIPS_CACHE_L2 > 0
    IMPORT_LABEL(mipsL2R4kClearAll)
#endif

;/*********************************************************************************************************
;  Kseg0 Cache ������ CPU ����ʵ���й�, �����������������������, BSP ������ʵ������
;*********************************************************************************************************/

    WEAK(mipsCacheR4kDisableHw)
    WEAK(mipsCacheR4kEnableHw)

    FILE_BEGIN()

;/*********************************************************************************************************
;  ���� CACHE
;*********************************************************************************************************/

FUNC_DEF(mipsCacheR4kDisableHw)
    MFC0_EHB(T0, CP0_CONFIG)
    ANDI        T0 , T0 , CONF_CM_CMASK
    LI          T1 , CONF_CM_UNCACHED
    BEQ         T0 , T1 , 1f
    NOP

    REG_SUBU    SP , ARCH_STK_VAR_SIZE
    REG_S       RA , ARCH_STK_OFF_VAR(0)(SP)
    REG_S       FP , ARCH_STK_OFF_VAR(1)(SP)
    MOV         FP , SP

    JAL         mipsDCacheR4kClearAll
    NOP

    JAL         mipsICacheR4kInvalidateAll
    NOP

#if LW_CFG_MIPS_CACHE_L2 > 0
    JAL         mipsL2R4kClearAll
    NOP
#endif

    MOV         SP , FP
    REG_L       RA , ARCH_STK_OFF_VAR(0)(SP)
    REG_L       FP , ARCH_STK_OFF_VAR(1)(SP)
    REG_ADDU    SP , ARCH_STK_VAR_SIZE

    MFC0_EHB(T0, CP0_CONFIG)
    AND         T0 , T0 , ~CONF_CM_CMASK
    ORI         T0 , T0 , CONF_CM_UNCACHED
    MTC0_EHB(T0, CP0_CONFIG)

1:
    JR          RA
    NOP
    FUNC_END(mipsCacheR4kDisableHw)

;/*********************************************************************************************************
;  ʹ�� CACHE
;*********************************************************************************************************/

FUNC_DEF(mipsCacheR4kEnableHw)
    MFC0_EHB(T0, CP0_CONFIG)
    ANDI        T0 , T0 , CONF_CM_CMASK
    LI          T1 , CONF_CM_CACHABLE_NONCOHERENT
    BEQ         T0 , T1 , 1f
    NOP

    MFC0_EHB(T0, CP0_CONFIG)
    AND         T0 , T0 , ~CONF_CM_CMASK
    ORI         T0 , CONF_CM_CACHABLE_NONCOHERENT
    MTC0_EHB(T0, CP0_CONFIG)

1:
    JR          RA
    NOP
    FUNC_END(mipsCacheR4kEnableHw)

;/*********************************************************************************************************
;  ��дһ�� DCACHE ��
;*********************************************************************************************************/

FUNC_DEF(mipsDCacheR4kLineFlush)
    CACHE       Hit_Writeback_D , 0(A0)
    JR          RA
    NOP
    FUNC_END(mipsDCacheR4kLineFlush)

;/*********************************************************************************************************
;  ��д����Чһ�� DCACHE ��
;*********************************************************************************************************/

FUNC_DEF(mipsDCacheR4kLineClear)
    CACHE       Hit_Writeback_Inv_D , 0(A0)
    JR          RA
    NOP
    FUNC_END(mipsDCacheR4kLineClear)

;/*********************************************************************************************************
;  ��Чһ�� DCACHE ��
;*********************************************************************************************************/

FUNC_DEF(mipsDCacheR4kLineInvalidate)
    CACHE       Hit_Invalidate_D , 0(A0)
    JR          RA
    NOP
    FUNC_END(mipsDCacheR4kLineInvalidate)

;/*********************************************************************************************************
;  ͨ�� CACHE �洢���ĵ�ַ������д����Чһ�� DCACHE ��
;*********************************************************************************************************/

FUNC_DEF(mipsDCacheR4kIndexClear)
    CACHE       Index_Writeback_Inv_D , 0(A0)
    JR          RA
    NOP
    FUNC_END(mipsDCacheR4kIndexClear)

;/*********************************************************************************************************
;  ��Чһ�� ICACHE ��
;*********************************************************************************************************/

FUNC_DEF(mipsICacheR4kLineInvalidate)
    CACHE       Hit_Invalidate_I , 0(A0)
    JR          RA
    NOP
    FUNC_END(mipsICacheR4kLineInvalidate)

;/*********************************************************************************************************
;  ͨ�� CACHE �洢���ĵ�ַ������д����Чһ�� ICACHE ��
;*********************************************************************************************************/

FUNC_DEF(mipsICacheR4kIndexInvalidate)
    CACHE       Index_Invalidate_I , 0(A0)
    JR          RA
    NOP
    FUNC_END(mipsICacheR4kIndexInvalidate)

;/*********************************************************************************************************
;  FILL ICACHE
;*********************************************************************************************************/

FUNC_DEF(mipsICacheR4kFill)
    CACHE       Fill , 0(A0)
    JR          RA
    NOP
    FUNC_END(mipsICacheR4kFill)

;/*********************************************************************************************************
;  ���� DCACHE �� TAG
;*********************************************************************************************************/

FUNC_DEF(mipsDCacheR4kIndexStoreTag)
    CACHE       Index_Store_Tag_D , 0(A0)
    JR          RA
    NOP
    FUNC_END(mipsDCacheR4kIndexStoreTag)

;/*********************************************************************************************************
;  ���� ICACHE �� TAG
;*********************************************************************************************************/

FUNC_DEF(mipsICacheR4kIndexStoreTag)
    CACHE       Index_Store_Tag_I , 0(A0)
    JR          RA
    NOP
    FUNC_END(mipsICacheR4kIndexStoreTag)

    FILE_END()

#endif
;/*********************************************************************************************************
;  END
;*********************************************************************************************************/
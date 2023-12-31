;/*********************************************************************************************************
;**
;**                                    中国软件开源组织
;**
;**                                   嵌入式实时操作系统
;**
;**                                       SylixOS(TM)
;**
;**                               Copyright  All Rights Reserved
;**
;**--------------文件信息--------------------------------------------------------------------------------
;**
;** 文   件   名: mips64MmuAsm.S
;**
;** 创   建   人: Jiao.JinXing (焦进星)
;**
;** 文件创建日期: 2017 年 11 月 30 日
;**
;** 描        述: MIPS64 体系构架 MMU TLB 重填处理.
;*********************************************************************************************************/

#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>
#include <config/kernel/kernel_cfg.h>
#include <config/kernel/memory_cfg.h>
#include <config/kernel/vmm_cfg.h>
#include <config/mp/mp_cfg.h>

#if LW_CFG_VMM_EN > 0
#if LW_CFG_CPU_WORD_LENGHT == 64

#include "arch/mips/arch_mmu.h"
#include "arch/mips/arch_regs.h"
#include "./mips64MmuAlgorithm.h"

    FILE_BEGIN()

    EXPORT_LABEL(mipsMmuTlbRefillEntry)
#if (LW_CFG_SMP_EN > 0) && (LW_CFG_MIPS_HAS_RDHWR_INSTR > 0)
    EXPORT_LABEL(_G_mips64MmuTlbRefillCtxMp)
#else
    EXPORT_LABEL(_G_mips64MmuTlbRefillCtx)
#endif

;/*********************************************************************************************************
;  用于临时保存异常上下文
;  如果有 RDHWR 指令, 可以通过 RDHWR 指令获得 CPU NUM,
;  所以每个处理器逻辑核的临时异常上下文是相互独立的, 故不用加锁访问, 以提高实时性
;*********************************************************************************************************/

#if (LW_CFG_SMP_EN > 0) && (LW_CFG_MIPS_HAS_RDHWR_INSTR > 0)
MACRO_DEF(PTR_VALUE from, to)
    PTR         ((\from) * MIPS64_TLB_CTX_SIZE + _G_mips64MmuTlbRefillCtxMp)
    .if         \to-\from
    PTR_VALUE   "(\from+1)",\to
    .endif
    MACRO_END()

    SECTION(.data)
    .balign     ARCH_STK_ALIGN_SIZE
LINE_LABEL(_G_mips64MmuTlbRefillCtx)
    PTR_VALUE   0 , (LW_CFG_MAX_PROCESSORS - 1)

    SECTION(.bss)
    .balign     ARCH_STK_ALIGN_SIZE
LINE_LABEL(_G_mips64MmuTlbRefillCtxMp)
    .space      (MIPS64_TLB_CTX_SIZE * LW_CFG_MAX_PROCESSORS)
#else
    SECTION(.bss)
    .balign     ARCH_STK_ALIGN_SIZE
LINE_LABEL(_G_mips64MmuTlbRefillCtx)
    .space      MIPS64_TLB_CTX_SIZE
#endif

;/*********************************************************************************************************
;  TLB 重填
;*********************************************************************************************************/

FUNC_DEF(mipsMmuTlbRefillEntry)
    .set        push
    .set        noat

    ;/*
    ; * 因为这些都是内核代码，不经 TLB 转换，所以执行时不会发生 TLB 重填异常
    ; */
;/*********************************************************************************************************
;  1. 对 _G_mips64MmuTlbRefillCtx 进行加锁
;*********************************************************************************************************/
    ;/*
    ; * _G_mips64MmuTlbRefillCtx 在内核内存，不经 TLB 转换，所以访问时不会发生 TLB 重填异常
    ; * 用自旋锁的方式访问 _G_mips64MmuTlbRefillCtx
    ; */
1:
    PTR_LA      K0 , _G_mips64MmuTlbRefillCtx
#if LW_CFG_SMP_EN > 0
#if LW_CFG_MIPS_HAS_RDHWR_INSTR > 0
    .set        push
#if LW_CFG_CPU_WORD_LENGHT == 32
    .set        mips32r2
#else
    .set        mips64r2
#endif
    RDHWR       K1 , HWR_CPUNUM
    .set        pop
    PTR_SLL     K1 , K1 , PTRLOG
    PTR_ADDU    K1 , K1 , K0
    PTR_L       K0 , 0(K1)
#else
    KN_WEAK_LLSC_MB_INST
    LL          K1 , MIPS64_TLB_CTX_LOCK(K0)
    BNEZ        K1 , 1b
    NOP

    LI          K1 , 1
    SC          K1 , MIPS64_TLB_CTX_LOCK(K0)
    BEQZ        K1 , 1b
    NOP
    KN_SMP_LLSC_MB_INST
#endif
#endif

;/*********************************************************************************************************
;  2. 保存异常用到的寄存器到 _G_mips64MmuTlbRefillCtx
;*********************************************************************************************************/

    REG_S       T0 , MIPS64_TLB_CTX_T0(K0)
    REG_S       T1 , MIPS64_TLB_CTX_T1(K0)
    REG_S       T2 , MIPS64_TLB_CTX_T2(K0)

;/*********************************************************************************************************
;  3. 获得 PGD TABLE BASE 与 BAD VA
;*********************************************************************************************************/

    REG_L       T2 , MIPS64_TLB_CTX_PGD(K0)                             ;/*  T2 等于 PGD TABLE BASE      */

    DMFC0       T0 , CP0_XCONTEXT                                       ;/*  T0 等于 XContext 寄存器     */
    MIPS_EHB

    ;/*
    ; * T0 <<= MIPS64_PTE_BASE_SIZE
    ; * T0 >>= MIPS64_PTE_BASE_SIZE
    ; * T0 >>= MIPS64_BADVPN2_SHIFT
    ; * T0 <<= LW_CFG_VMM_PAGE_SHIFT
    ; */
    LONG_SLL    T0 , MIPS64_PTE_BASE_SIZE
    LONG_SRL    T0 , (MIPS64_PTE_BASE_SIZE + MIPS64_BADVPN2_SHIFT + 4)
    LONG_SLL    T0 , (LW_CFG_VMM_PAGE_SHIFT + 1)                        ;/*  T0 为 BAD VA                */

;/*********************************************************************************************************
;  4. 获得 PGD 条目
;*********************************************************************************************************/

    PTR_LI      T1 , LW_CFG_VMM_PGD_MASK
    AND         T1 , T0 , T1
    LONG_SRL    T1 , LW_CFG_VMM_PGD_SHIFT
    LONG_SLL    T1 , LONGLOG
    REG_ADDU    T1 , T2 , T1
    REG_L       T2 , 0(T1)
    BEQ         T2 , ZERO , __error_handle
    NOP

;/*********************************************************************************************************
;  5. 获得 PMD 条目
;*********************************************************************************************************/

    PTR_LI      T1 , LW_CFG_VMM_PMD_MASK
    AND         T1 , T0 , T1
    LONG_SRL    T1 , LW_CFG_VMM_PMD_SHIFT
    LONG_SLL    T1 , LONGLOG
    REG_ADDU    T1 , T2 , T1
    REG_L       T2 , 0(T1)
    BEQ         T2 , ZERO , __error_handle
    NOP

;/*********************************************************************************************************
;  6. 获得 PTS 条目
;*********************************************************************************************************/

    PTR_LI      T1 , LW_CFG_VMM_PTS_MASK
    AND         T1 , T0 , T1
    LONG_SRL    T1 , LW_CFG_VMM_PTS_SHIFT
    LONG_SLL    T1 , LONGLOG
    REG_ADDU    T1 , T2 , T1
    REG_L       T2 , 0(T1)
    BEQ         T2 , ZERO , __error_handle
    NOP

;/*********************************************************************************************************
;  7. 获得一对 PTE 条目
;*********************************************************************************************************/

    PTR_LI      T1 , LW_CFG_VMM_PTE_MASK
    AND         T1 , T0 , T1
    LONG_SRL    T1 , LW_CFG_VMM_PTE_SHIFT
    LONG_SLL    T1 , LONGLOG
    REG_ADDU    T1 , T2 , T1
    REG_L       T0 , 0(T1)
    REG_L       T1 , LONGSIZE(T1)

;/*********************************************************************************************************
;  8. TLB 重填
;*********************************************************************************************************/

    DMTC0       T0 , CP0_ENTRYLO0                                       ;/*  EntryLo0                    */
    DMTC0       T1 , CP0_ENTRYLO1                                       ;/*  EntryLo1                    */
    MIPS_EHB
    TLBWR                                                               ;/*  TLB 随机替换                */

;/*********************************************************************************************************
;  9. 恢复寄存器
;*********************************************************************************************************/

    REG_L       T0 , MIPS64_TLB_CTX_T0(K0)
    REG_L       T1 , MIPS64_TLB_CTX_T1(K0)
    REG_L       T2 , MIPS64_TLB_CTX_T2(K0)

;/*********************************************************************************************************
;  10. 解锁上下文
;*********************************************************************************************************/

#if (LW_CFG_SMP_EN > 0) && (LW_CFG_MIPS_HAS_RDHWR_INSTR == 0)
    KN_SMP_MB_INST
    REG_S       ZERO , MIPS64_TLB_CTX_LOCK(K0)                          ;/*  解锁 MmuTlbRefillCtx        */
    KN_SMP_MB_INST
#endif

;/*********************************************************************************************************
;  11. 异常返回
;*********************************************************************************************************/

    ERET                                                                ;/*  异常返回                    */
    NOP

;/*********************************************************************************************************
;  错误处理
;*********************************************************************************************************/

LINE_LABEL(__error_handle)

;/*********************************************************************************************************
;  1. TLB 重填
;*********************************************************************************************************/

    DMTC0       ZERO , CP0_ENTRYLO0                                     ;/*  EntryLo0                    */
    DMTC0       ZERO , CP0_ENTRYLO1                                     ;/*  EntryLo1                    */
    MIPS_EHB
    TLBWR                                                               ;/*  TLB 随机替换                */

;/*********************************************************************************************************
;  2. 恢复寄存器
;*********************************************************************************************************/

    REG_L       T0 , MIPS64_TLB_CTX_T0(K0)
    REG_L       T1 , MIPS64_TLB_CTX_T1(K0)
    REG_L       T2 , MIPS64_TLB_CTX_T2(K0)

;/*********************************************************************************************************
;  3. 解锁上下文
;*********************************************************************************************************/

#if (LW_CFG_SMP_EN > 0) && (LW_CFG_MIPS_HAS_RDHWR_INSTR == 0)
    KN_SMP_MB_INST
    REG_S       ZERO , MIPS64_TLB_CTX_LOCK(K0)                          ;/*  解锁 MmuTlbRefillCtx        */
    KN_SMP_MB_INST
#endif

;/*********************************************************************************************************
;  4. 异常返回
;*********************************************************************************************************/

    ERET                                                                ;/*  异常返回                    */
    NOP

    .set        pop
    FUNC_END(mipsMmuTlbRefillEntry)

    FILE_END()

#endif
#endif
;/*********************************************************************************************************
;  END
;*********************************************************************************************************/

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
;** 文   件   名: armLibAsm.S
;**
;** 创   建   人: Han.Hui (韩辉)
;**
;** 文件创建日期: 2013 年 12 月 07 日
;**
;** 描        述: ARM 体系构架内部库.
;**
;** BUG:
;2014.12.29  由于一些函数需要操作安全模式寄存器, 有些处理器需要做特殊处理, 所以这里改为弱符号.
;2015.09.07  当支持 CLZ 指令时, archFindLsb 与 archFindMsb 使用汇编实现. 
;*********************************************************************************************************/

#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>

#if !defined(__SYLIXOS_ARM_ARCH_M__)

    FILE_BEGIN()

;/*********************************************************************************************************
;  ARM 体系构架相关异常处理函数
;*********************************************************************************************************/

    IMPORT_LABEL(__setjmpSetup)
    IMPORT_LABEL(__sigsetjmpSetup)
    IMPORT_LABEL(__longjmpSetup)
    IMPORT_LABEL(__siglongjmpSetup)
    IMPORT_LABEL(bspReboot)
    
    EXPORT_LABEL(setjmp)
    EXPORT_LABEL(sigsetjmp)
    EXPORT_LABEL(longjmp)
    EXPORT_LABEL(siglongjmp)
    
#if __SYLIXOS_ARM_ARCH__ >= 5
    EXPORT_LABEL(archFindLsb)
    EXPORT_LABEL(archFindMsb)
#endif

    EXPORT_LABEL(archIntDisable)
    EXPORT_LABEL(archIntEnable)
    EXPORT_LABEL(archIntEnableForce)
    EXPORT_LABEL(archPageCopy)
    EXPORT_LABEL(archReboot)
    EXPORT_LABEL(archBogoMipsLoop)

    EXPORT_LABEL(archGetCpsr)

    WEAK(archIntDisable)
    WEAK(archIntEnable)
    WEAK(archIntEnableForce)

;/*********************************************************************************************************
;  ARM 关闭总中断
;*********************************************************************************************************/

FUNC_DEF(archIntDisable)
    MRS     R0 , CPSR
    ORR     R1 , R0, #0x80
    MSR     CPSR_c, R1
    MRS     R2 , CPSR
    AND     R2 , R2, #0x80
    CMP     R2 , #0x80
    BNE     archIntDisable
    BX      LR
    FUNC_END()
    
FUNC_DEF(archIntEnable)
    MSR     CPSR_c, R0
    BX      LR
    FUNC_END()
    
FUNC_DEF(archIntEnableForce)
    MRS     R0 , CPSR
    BIC     R0 , R0, #0x80
    MSR     CPSR_c, R0
    BX      LR
    FUNC_END()

;/*********************************************************************************************************
;  ARM 获得 CPSR
;*********************************************************************************************************/

FUNC_DEF(archGetCpsr)
    MRS     R0 , CPSR
    BX      LR
    FUNC_END()

;/*********************************************************************************************************
;  ARM 前导零运算
;*********************************************************************************************************/

#if __SYLIXOS_ARM_ARCH__ >= 5
FUNC_DEF(archFindLsb)
    RSB     R1 , R0, #0
    ANDS    R0 , R1, R0
    CLZNE   R0 , R0
    RSBNE   R0 , R0, #32
    BX      LR
    FUNC_END()

FUNC_DEF(archFindMsb)
    CMP     R0 , #0
    BXEQ    LR
    CLZNE   R0 , R0
    RSBNE   R0 , R0, #32
    BX      LR
    FUNC_END()
#endif

;/*********************************************************************************************************
;  页面拷贝 (代码来自 linux/arch/arm/lib)
;*********************************************************************************************************/

#define PAGE_SZ         4096
#define L1_CACHE_BYTES  32
#define COPY_COUNT      (PAGE_SZ / (2 * L1_CACHE_BYTES) PLD(-1))

FUNC_DEF(archPageCopy)
    STMFD       SP! , {R4, LR}

    PLD(    PLD [R1, #0]        )
    PLD(    PLD [R1, #L1_CACHE_BYTES]       )

    MOV         R2  , #COPY_COUNT
    LDMIA       R1! , {R3, R4, IP, LR}

1:  PLD(    PLD [R1, #2 * L1_CACHE_BYTES]       )
    PLD(    PLD [R1, #3 * L1_CACHE_BYTES]       )

2:
    .rept   (2 * L1_CACHE_BYTES / 16 - 1)
    STMIA       R0! , {R3, R4, IP, LR}
    LDMIA       R1! , {R3, R4, IP, LR}
    .endr

    SUBS        R2  , R2, #1
    STMIA       R0! , {R3, R4, IP, LR}
    LDMGTIA     R1! , {R3, R4, IP, LR}

    BGT         1b

    PLD(    LDMEQIA R1!, {R3, R4, IP, LR}   )
    PLD(    BEQ 2b          )

    LDMFD       SP! , {R4, PC}
    FUNC_END()

;/*********************************************************************************************************
;  注意: setjmp 与 longjmp 上下文结构与线程上下文结构不同
;
;  +---------------+
;  |    R4 - R7    | (4 regs)
;  +---------------+
;  |    R8 - R10   | (3 regs)
;  +---------------+
;  |    FP(R11)    | (1 regs)
;  +---------------+
;  |    SP(R13)    | (1 regs)
;  +---------------+
;  |    LR(R14)    | (1 regs)
;  +---------------+
;*********************************************************************************************************/

;/*********************************************************************************************************
;  sigsetjmp (参数为 jmp_buf, mask_saved)
;*********************************************************************************************************/

FUNC_DEF(sigsetjmp)
    STMFD   SP!, {R0, R1, R2, LR}
    BL      __sigsetjmpSetup
    LDMFD   SP!, {R0, R1, R2, LR}
    
    STMIA   R0!, {R4, R5, R6, R7}                                       ;/* Save registers in jump buffer*/
    MOV     R1 , R8
    MOV     R2 , R9
    MOV     R3 , R10
    MOV     R4 , FP
    MOV     R5 , SP
    MOV     R6 , LR
    STMIA   R0!, {R1, R2, R3, R4, R5, R6}
    
    SUB     R0 , R0 , #40
    LDMIA   R0!, {R4, R5, R6, R7}                                       ;/* Restore callee-saved low regs*/
    
    MOV     R0 , #0                                                     ;/* Return zero.                 */
    BX      LR
    FUNC_END()

;/*********************************************************************************************************
;  setjmp (参数为 jmp_buf)
;*********************************************************************************************************/

FUNC_DEF(setjmp)
    STMFD   SP!, {R0, LR}
    BL      __setjmpSetup
    LDMFD   SP!, {R0, LR}
    
    STMIA   R0!, {R4, R5, R6, R7}                                       ;/* Save registers in jump buffer*/
    MOV     R1 , R8
    MOV     R2 , R9
    MOV     R3 , R10
    MOV     R4 , FP
    MOV     R5 , SP
    MOV     R6 , LR
    STMIA   R0!, {R1, R2, R3, R4, R5, R6}
    
    SUB     R0 , R0 , #40
    LDMIA   R0!, {R4, R5, R6, R7}                                       ;/* Restore callee-saved low regs*/
    
    MOV     R0 , #0                                                     ;/* Return zero.                 */
    BX      LR
    FUNC_END()
    
;/*********************************************************************************************************
;  siglongjmp (参数为 jmp_buf, retval)
;*********************************************************************************************************/

FUNC_DEF(siglongjmp)
    STMFD   SP!, {R0, R1, R2, LR}
    BL      __siglongjmpSetup
    LDMFD   SP!, {R0, R1, R2, LR}

    ADD     R0 , R0 , #16
    LDMIA   R0!, {R2, R3, R4, R5, R6}                                   ;/* Restore High regs.           */
    MOV     R8 , R2
    MOV     R9 , R3
    MOV     R10, R4
    MOV     FP , R5
    MOV     SP , R6
    
    LDMIA   R0!, {R3}                                                   ;/* lr                           */

    SUB     R0 , R0 , #40                                               ;/* Restore low regs.            */
    LDMIA   R0!, {R4, R5, R6, R7}
                                                                        ;/* Return the result argument,  */
                                                                        ;/* or 1 if it is zero.          */
    MOV     R0 , R1
    CMP     R0 , #0
    BNE     siglongjmpnotzero
    MOV     R0 , #1

LINE_LABEL(siglongjmpnotzero)
    BX      R3
    FUNC_END()

;/*********************************************************************************************************
;  longjmp (参数为 jmp_buf, retval)
;*********************************************************************************************************/

FUNC_DEF(longjmp)
    STMFD   SP!, {R0, R1, R2, LR}
    BL      __longjmpSetup
    LDMFD   SP!, {R0, R1, R2, LR}

    ADD     R0 , R0 , #16
    LDMIA   R0!, {R2, R3, R4, R5, R6}                                   ;/* Restore High regs.           */
    MOV     R8 , R2
    MOV     R9 , R3
    MOV     R10, R4
    MOV     FP , R5
    MOV     SP , R6
    
    LDMIA   R0!, {R3}                                                   ;/* lr                           */

    SUB     R0 , R0 , #40                                               ;/* Restore low regs.            */
    LDMIA   R0!, {R4, R5, R6, R7}
                                                                        ;/* Return the result argument,  */
                                                                        ;/* or 1 if it is zero.          */
    MOV     R0 , R1
    CMP     R0 , #0
    BNE     longjmpnotzero
    MOV     R0 , #1

LINE_LABEL(longjmpnotzero)
    BX      R3
    FUNC_END()
    
;/*********************************************************************************************************
;  系统重启
;*********************************************************************************************************/

FUNC_DEF(archReboot)
    B       bspReboot                                                   ;/* 不再返回, 由 BSP 进行复位操作*/
    FUNC_END()
    
;/*********************************************************************************************************
;  Bogo 循环
;*********************************************************************************************************/

FUNC_DEF(archBogoMipsLoop)
1:
    MOV     R1 , #1
    MOV     R2 , #2
    MOV     R3 , #3
    MOV     R1 , #1
    MOV     R2 , #2
    MOV     R3 , #3
    SUBS    R0 , R0 , #1
    BNE     1b
    BX      LR
    FUNC_END()

    FILE_END()

#endif
;/*********************************************************************************************************
;  END
;*********************************************************************************************************/

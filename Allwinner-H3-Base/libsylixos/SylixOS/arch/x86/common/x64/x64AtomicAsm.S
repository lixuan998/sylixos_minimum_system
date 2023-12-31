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
;** 文   件   名: x64AtomicAsm.S
;**
;** 创   建   人: Jiao.JinXing (焦进星)
;**
;** 文件创建日期: 2018 年 07 月 30 日
;**
;** 描        述: x86-64 体系构架处理器原子量操作.
;*********************************************************************************************************/

#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>

    EXPORT_LABEL(archAtomicAdd)
    EXPORT_LABEL(archAtomicSub)
    EXPORT_LABEL(archAtomicAnd)
    EXPORT_LABEL(archAtomicNand)
    EXPORT_LABEL(archAtomicOr)
    EXPORT_LABEL(archAtomicXor)

    EXPORT_LABEL(archAtomic64Add)
    EXPORT_LABEL(archAtomic64Sub)
    EXPORT_LABEL(archAtomic64And)
    EXPORT_LABEL(archAtomic64Nand)
    EXPORT_LABEL(archAtomic64Or)
    EXPORT_LABEL(archAtomic64Xor)

;/*********************************************************************************************************
;  原子量加
;*********************************************************************************************************/

FUNC_DEF(archAtomicAdd)
    MOVL        %EDI , %EAX
    LOCK
    XADDL       %EDI , (%RSI)
    ADDL        %EDI , %EAX
    RET
    FUNC_END(archAtomicAdd)

;/*********************************************************************************************************
;  原子量减
;*********************************************************************************************************/

FUNC_DEF(archAtomicSub)
    NEGL        %EDI
    MOVL        %EDI , %EAX
    LOCK
    XADDL       %EDI , (%RSI)
    ADDL        %EDI , %EAX
    RET
    FUNC_END(archAtomicSub)

;/*********************************************************************************************************
;  原子量与
;*********************************************************************************************************/

FUNC_DEF(archAtomicAnd)
    MOVL        (%RSI), %EAX                                            ;/*  旧值                        */
LINE_LABEL(1)
    MOVL        %EDI , %ECX
    ANDL        %EAX , %ECX
    LOCK
    CMPXCHGL    %ECX , (%RSI)
    JNZ         1b
    MOVL        %ECX , %EAX
    RET
    FUNC_END(archAtomicAnd)

;/*********************************************************************************************************
;  原子量与非
;*********************************************************************************************************/

FUNC_DEF(archAtomicNand)
    MOVL        (%RSI), %EAX                                            ;/*  旧值                        */
LINE_LABEL(1)
    MOVL        %EDI , %ECX
    ANDL        %EAX , %ECX
    NOT         %ECX
    LOCK
    CMPXCHGL    %ECX, (%RSI)
    JNZ         1b
    MOVL        %ECX , %EAX
    RET
    FUNC_END(archAtomicNand)

;/*********************************************************************************************************
;  原子量或
;*********************************************************************************************************/

FUNC_DEF(archAtomicOr)
    MOVL        (%RSI), %EAX                                            ;/*  旧值                        */
LINE_LABEL(1)
    MOVL        %EDI , %ECX
    ORL         %EAX , %ECX
    LOCK
    CMPXCHGL    %ECX , (%RSI)
    JNZ         1b
    MOVL        %ECX , %EAX
    RET
    FUNC_END(archAtomicOr)

;/*********************************************************************************************************
;  原子量异或
;*********************************************************************************************************/

FUNC_DEF(archAtomicXor)
    MOVL        (%RSI), %EAX                                            ;/*  旧值                        */
LINE_LABEL(1)
    MOVL        %EDI , %ECX
    XORL        %EAX , %ECX
    LOCK
    CMPXCHGL    %ECX , (%RSI)
    JNZ         1b
    MOVL        %ECX , %EAX
    RET
    FUNC_END(archAtomicXor)

;/*********************************************************************************************************
;  64 位原子量加
;*********************************************************************************************************/

FUNC_DEF(archAtomic64Add)
    MOVQ        %RDI , %RAX
    LOCK
    XADDQ       %RDI , (%RSI)
    ADDQ        %RDI , %RAX
    RET
    FUNC_END(archAtomic64Add)

;/*********************************************************************************************************
;  64 位原子量减
;*********************************************************************************************************/

FUNC_DEF(archAtomic64Sub)
    NEGQ        %RDI
    MOVQ        %RDI , %RAX
    LOCK
    XADDQ       %RDI , (%RSI)
    ADDQ        %RDI , %RAX
    RET
    FUNC_END(archAtomic64Sub)

;/*********************************************************************************************************
;  64 位原子量与
;*********************************************************************************************************/

FUNC_DEF(archAtomic64And)
    MOVQ        (%RSI), %RAX                                            ;/*  旧值                        */
LINE_LABEL(1)
    MOVQ        %RDI , %RCX
    ANDQ        %RAX , %RCX
    LOCK
    CMPXCHGQ    %RCX , (%RSI)
    JNZ         1b
    MOVQ        %RCX , %RAX
    RET
    FUNC_END(archAtomic64And)

;/*********************************************************************************************************
;  64 位原子量与非
;*********************************************************************************************************/

FUNC_DEF(archAtomic64Nand)
    MOVQ        (%RSI), %RAX                                            ;/*  旧值                        */
LINE_LABEL(1)
    MOVQ        %RDI , %RCX
    ANDQ        %RAX , %RCX
    NOTQ        %RCX
    LOCK
    CMPXCHGQ    %RCX, (%RSI)
    JNZ         1b
    MOVQ        %RCX , %RAX
    RET
    FUNC_END(archAtomic64Nand)

;/*********************************************************************************************************
;  64 位原子量或
;*********************************************************************************************************/

FUNC_DEF(archAtomic64Or)
    MOVQ        (%RSI), %RAX                                            ;/*  旧值                        */
LINE_LABEL(1)
    MOVQ        %RDI , %RCX
    ORQ         %RAX , %RCX
    LOCK
    CMPXCHGQ    %RCX , (%RSI)
    JNZ         1b
    MOVQ        %RCX , %RAX
    RET
    FUNC_END(archAtomic64Or)

;/*********************************************************************************************************
;  64 位原子量异或
;*********************************************************************************************************/

FUNC_DEF(archAtomic64Xor)
    MOVQ        (%RSI), %RAX                                            ;/*  旧值                        */
LINE_LABEL(1)
    MOVQ        %RDI , %RCX
    XORQ        %RAX , %RCX
    LOCK
    CMPXCHGQ    %RCX , (%RSI)
    JNZ         1b
    MOVQ        %RCX , %RAX
    RET
    FUNC_END(archAtomic64Xor)

;/*********************************************************************************************************
;  END
;*********************************************************************************************************/

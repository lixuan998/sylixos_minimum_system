/*********************************************************************************************************
**
**                                    中国软件开源组织
**
**                                   嵌入式实时操作系统
**
**                                SylixOS(TM)  LW : long wing
**
**                               Copyright All Rights Reserved
**
**--------------文件信息--------------------------------------------------------------------------------
**
** 文   件   名: arch_types.h
**
** 创   建   人: Wang.Ziyang (王子阳)
**
** 文件创建日期: 2022 年 04 月 07 日
**
** 描        述: LoongArch 类型相关.
*********************************************************************************************************/

#ifndef __LOONGARCH_ARCH_TYPES_H
#define __LOONGARCH_ARCH_TYPES_H

/*********************************************************************************************************
  C++ 下 void 不可以 typedef
*********************************************************************************************************/

#ifdef __cplusplus
#define VOID     void
#else
typedef          void               VOID;                               /*  void   类型                 */
#endif                                                                  /*  __cplusplus                 */

/*********************************************************************************************************
  编译器相关数据类型定义
*********************************************************************************************************/

typedef          int                BOOL;                               /*  布尔变量定义                */

typedef          void              *PVOID;                              /*  void * 类型                 */
typedef const    void              *CPVOID;                             /*  const void  *               */

typedef          char               CHAR;                               /*  8 位字符变量                */
typedef unsigned char               UCHAR;                              /*  8 位无符号字符变量          */
typedef unsigned char              *PUCHAR;                             /*  8 位无符号字符变量指针      */
typedef          char              *PCHAR;                              /*  8 位字符指针变量            */
typedef const    char              *CPCHAR;                             /*  const char  *               */

typedef unsigned char               BYTE;                               /*  8 位字节变量                */
typedef unsigned char              *PBYTE;                              /*  8 位字节变量指针            */

typedef          long               LONG;                               /*  32/64 位数定义              */
typedef unsigned long               ULONG;                              /*  32/64 位无符号数定义        */

typedef          int                INT;                                /*  编译器相关 int              */
typedef unsigned int                UINT;                               /*  编译器相关 unsigned int     */
typedef signed   int                SINT;                               /*  编译器相关 signed   int     */

/*********************************************************************************************************
  注意: INT8 必须是 signed 型. 这样 int8_t 才能保证是 signed 型.
        很多编译器默认 char 是 unsigned. 所以这里必须强制将 INT8 定义为 signed 型.
*********************************************************************************************************/

typedef signed   char               INT8;                               /*  char                        */
typedef unsigned char               UINT8;                              /*  8  位无符号数定义           */
typedef signed   char               SINT8;                              /*  8  位有符号数定义           */

typedef          short              INT16;                              /*  short                       */
typedef unsigned short              UINT16;                             /*  16 位无符号数定义           */
typedef signed   short              SINT16;                             /*  16 位有符号数定义           */

typedef          int                INT32;                              /*  long or int                 */
typedef unsigned int                UINT32;                             /*  32 位无符号数定义           */
typedef signed   int                SINT32;                             /*  32 位有符号数定义           */

typedef          long long          INT64;                              /*  long long                   */
typedef unsigned long long          UINT64;                             /*  64 位无符号数定义           */
typedef signed   long long          SINT64;                             /*  64 位有符号数定义           */

/*********************************************************************************************************
  此类型变量用来保存系统开关中断的上下文.
  注意: INTREG 用于记录 CSR_CRMD, 所以 LoongArch64 也使用 32 位 unsigned int 类型
*********************************************************************************************************/

typedef volatile unsigned int       INTREG;                             /*  定义处理器中断寄存器        */

#endif                                                                  /*  __LOONGARCH_ARCH_TYPES_H    */
/*********************************************************************************************************
  END
*********************************************************************************************************/

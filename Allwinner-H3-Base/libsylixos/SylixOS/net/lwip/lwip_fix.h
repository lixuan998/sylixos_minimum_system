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
** 文   件   名: lwip_fix.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2009 年 05 月 06 日
**
** 描        述: lwip SylixOS 移植.
*********************************************************************************************************/

#ifndef __LWIP_FIX_H
#define __LWIP_FIX_H

#include "SylixOS.h"
#include "string.h"
#include "stdio.h"
#include "sys/endian.h"
#include "lwip_errno.h"

#ifdef __cplusplus
extern "C" {
#endif                                                                  /*  __cplusplus                 */

/*********************************************************************************************************
  数据类型
*********************************************************************************************************/

#define LWIP_NO_STDINT_H            1
#define LWIP_NO_INTTYPES_H          1
#define LWIP_HAVE_INT64             1

typedef UINT8                       u8_t;
typedef SINT8                       s8_t;
typedef UINT16                      u16_t;
typedef SINT16                      s16_t;
typedef UINT32                      u32_t;
typedef SINT32                      s32_t;
typedef UINT64                      u64_t;
typedef SINT64                      s64_t;

typedef ULONG                       mem_ptr_t;

#define U16_F                       "u"
#define S16_F                       "d"
#define U32_F                       "u"
#define S32_F                       "d"
#define U64_F                       "qu"
#define S64_F                       "qd"
#define X16_F                       "x"
#define X32_F                       "x"
#define SZT_F                       "zu"
#define X8_F                        "02x"

/*********************************************************************************************************
  内存对齐声明
*********************************************************************************************************/

#ifdef __GNUC__
#if LW_CFG_CPU_WORD_LENGHT == 32
#define LWIP_DECLARE_MEMORY_ALIGNED(variable_name, size) \
        u8_t variable_name[size] __attribute__((aligned(4)))
#else
#define LWIP_DECLARE_MEMORY_ALIGNED(variable_name, size) \
        u8_t variable_name[size] __attribute__((aligned(8)))
#endif

#else                                                                   /*  __GNUC__                    */
#if LW_CFG_CPU_WORD_LENGHT == 32
#define LWIP_DECLARE_MEMORY_ALIGNED(variable_name, size) \
        u32_t variable_name[(size + sizeof(u32_t) - 1) / sizeof(u32_t)]
#else
#define LWIP_DECLARE_MEMORY_ALIGNED(variable_name, size) \
        u64_t variable_name[(size + sizeof(u64_t) - 1) / sizeof(u64_t)]
#endif
#endif                                                                  /*  !__GNUC__                   */

/*********************************************************************************************************
  编译器结构缩排相关
*********************************************************************************************************/

#define PACK_STRUCT_FIELD(x)        LW_STRUCT_PACK_FIELD(x)
#define PACK_STRUCT_STRUCT          LW_STRUCT_PACK_STRUCT
#define PACK_STRUCT_BEGIN           LW_STRUCT_PACK_BEGIN
#define PACK_STRUCT_END             LW_STRUCT_PACK_END

/*********************************************************************************************************
  主机与网络字节转换 (为了速度这里使用宏, 但是不可以直接使用, 例如: htonl(x++) 就会错误.)
*********************************************************************************************************/

#define LWIP_PLATFORM_BYTESWAP      0                                   /*  use lwip htonl() ...        */

#ifndef __GNUC__
#ifdef  __CC_ARM
#define inline  __inline
#else
#define inline
#endif                                                                  /*  __CC_ARM (armcc compiler)   */
#endif                                                                  /*  __GNUC__                    */

/*********************************************************************************************************
  调试输出
*********************************************************************************************************/

void  sys_assert_print(const char *msg, const char *func, const char *file, int line);
void  sys_error_print(const char *msg, const char *func, const char *file, int line);

#define LWIP_PLATFORM_DIAG(x)	    {   printf x;   }
#define LWIP_PLATFORM_ASSERT(x)     {   sys_assert_print(x, __func__, __FILE__, __LINE__);  }
#define LWIP_PLATFORM_ERROR(x)      {   sys_error_print(x, __func__, __FILE__, __LINE__);  }

#ifdef LW_UNLIKELY
#define LWIP_ERROR(message, expression, handler) do { if (LW_UNLIKELY(!(expression))) { \
        LWIP_PLATFORM_ERROR(message); handler;}} while(0)
#else
#define LWIP_ERROR(message, expression, handler) do { if (!(expression)) { \
        LWIP_PLATFORM_ERROR(message); handler;}} while(0)
#endif

/*********************************************************************************************************
  关键区域保护
*********************************************************************************************************/

void  sys_arch_protect(INTREG  *pireg);
void  sys_arch_unprotect(INTREG  ireg);

#define SYS_ARCH_DECL_PROTECT(x)    INTREG  x
#define SYS_ARCH_PROTECT(x)         sys_arch_protect(&x)
#define SYS_ARCH_UNPROTECT(x)       sys_arch_unprotect(x)

/*********************************************************************************************************
  ATOMIC
*********************************************************************************************************/

#if defined(SYLIXOS) && \
    (LW_CFG_CPU_ATOMIC_EN > 0) && \
    (LW_CFG_LWIP_PBUF_ATOMIC > 0) && \
    (LWIP_PBUF_REF_T == int)
#define SYS_ARCH_INC(var, val)      __LW_ATOMIC_ADD((val), (atomic_t *)&(var))
#define SYS_ARCH_DEC(var, val)      __LW_ATOMIC_SUB((val), (atomic_t *)&(var))
#define SYS_ARCH_GET(var, ret)      (ret) = __LW_ATOMIC_GET((atomic_t *)&(var))
#define SYS_ARCH_SET(var, val)      __LW_ATOMIC_SET((val), (atomic_t *)&(var))
#endif

/*********************************************************************************************************
  Measurement calls made throughout lwip, these can be defined to nothing.
*********************************************************************************************************/

#define PERF_START                                                      /* null definition              */
#define PERF_STOP(x)                                                    /* null definition              */

/*********************************************************************************************************
  OS 数据类型
*********************************************************************************************************/

typedef LW_OBJECT_HANDLE            sys_mutex_t;
typedef LW_OBJECT_HANDLE            sys_sem_t;
typedef LW_OBJECT_HANDLE            sys_mbox_t;
typedef LW_OBJECT_HANDLE            sys_thread_t;

#define SYS_MUTEX_NULL              0ul
#define SYS_SEM_NULL                0ul
#define SYS_MBOX_NULL               0ul

/*********************************************************************************************************
  DNS thread safe function
*********************************************************************************************************/

struct hostent  *sys_thread_hostent(struct hostent  *phostent);

/*********************************************************************************************************
  net safe (操作系统自动 alloc & free 不需要协议栈操作)
*********************************************************************************************************/
#if LW_CFG_NET_SAFE > 0

sys_sem_t *sys_thread_sem_get(void);

#define LWIP_NETCONN_THREAD_SEM_GET()   sys_thread_sem_get()
#define LWIP_NETCONN_THREAD_SEM_ALLOC()
#define LWIP_NETCONN_THREAD_SEM_FREE()

#endif                                                                  /*  LW_CFG_NET_SAFE             */
/*********************************************************************************************************
  fast OS function
*********************************************************************************************************/

#define __u_char_defined                                                /*  do not use PPP lib type     */

void  sys_arch_msleep(u32_t ms);                                        /*  XXX can instead sem timeout */
#define sys_msleep                  sys_arch_msleep

/*********************************************************************************************************
  TCP ISN function
*********************************************************************************************************/

void  sys_tcp_isn_key_update(void);

/*********************************************************************************************************
  rand() 
*********************************************************************************************************/

extern INT  lib_rand(VOID);
#define LWIP_RAND()                 (u32_t)lib_rand()

/*********************************************************************************************************
  Security region
*********************************************************************************************************/
#if LW_CFG_LWIP_SEC_REGION > 0

extern uint8_t  sys_sec_region_init(void);
#define SYS_SEC_REGION_INIT()       sys_sec_region_init()

#endif                                                                  /*  LW_CFG_LWIP_SEC_REGION      */
/*********************************************************************************************************
  mutex
*********************************************************************************************************/

#define LWIP_COMPAT_MUTEX           0                                   /*  use mutex method            */

/*********************************************************************************************************
  host to net byte order convert
*********************************************************************************************************/

LW_API uint32_t     htonl(uint32_t x);
LW_API uint32_t     ntohl(uint32_t x);
LW_API uint16_t     htons(uint16_t x);
LW_API uint16_t     ntohs(uint16_t x);

#ifdef __cplusplus
}
#endif                                                                  /*  __cplusplus                 */

#endif                                                                  /*  __LWIP_FIX_H                */
/*********************************************************************************************************
  END
*********************************************************************************************************/

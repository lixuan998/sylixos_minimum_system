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
** 文   件   名: ThreadJoin.c
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2007 年 07 月 19 日
**
** 描        述: 线程合并

** BUG
2007.07.19  加入 _DebugHandle() 功能
2008.05.18  使用 __KERNEL_ENTER() 代替 ThreadLock();
2010.10.06  加入对 cancel type 的操作, 符合 POSIX 标准.
2012.08.25  API_ThreadJoin() 句柄错误时不打印错误. 这样可以避免一些误打印.
2013.04.01  修正 GCC 4.7.3 引发的新 warning.
2013.07.18  使用新的获取 TCB 的方法, 确保 SMP 系统安全.
2013.09.17  使用 POSIX 规定的取消点动作.
2022.05.27  修正 popen 等待 shell 结束 join 操作错误.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "../SylixOS/kernel/include/k_kernel.h"
/*********************************************************************************************************
  shell
*********************************************************************************************************/
#if LW_CFG_SHELL_EN > 0
#include "../SylixOS/shell/ttinyShell/ttinyShell.h"
#include "../SylixOS/shell/ttinyShell/ttinyShellLib.h"
#endif                                                                  /*  LW_CFG_SHELL_EN > 0         */
/*********************************************************************************************************
  s_internal.h 中也有相关定义
*********************************************************************************************************/
#if LW_CFG_THREAD_CANCEL_EN > 0
#define __THREAD_CANCEL_POINT()         API_ThreadTestCancel()
#else
#define __THREAD_CANCEL_POINT()
#endif                                                                  /*  LW_CFG_THREAD_CANCEL_EN     */
/*********************************************************************************************************
  loader
*********************************************************************************************************/
#if LW_CFG_MODULELOADER_EN > 0
extern pid_t  vprocGetPidByTcbNoLock(PLW_CLASS_TCB  ptcb);
#endif                                                                  /*  LW_CFG_MODULELOADER_EN > 0  */
/*********************************************************************************************************
** 函数名称: __threadCanJoin
** 功能描述: 线程是否可以合并
** 输　入  : ptcbCur   当前线程控制块
**           ptcbJoin  要合并的线程控制块
** 输　出  : 是否可以合并
** 全局变量:
** 调用模块:
                                           API 函数
*********************************************************************************************************/
#if LW_CFG_MODULELOADER_EN > 0

BOOL  __threadCanJoin (PLW_CLASS_TCB  ptcbCur, PLW_CLASS_TCB  ptcbJoin)
{
    REGISTER pid_t   pidCur, pidJoin;

    pidCur  = vprocGetPidByTcbNoLock(ptcbCur);
    pidJoin = vprocGetPidByTcbNoLock(ptcbJoin);
    if (pidCur == pidJoin) {
        return  (LW_TRUE);
    }

#if LW_CFG_SHELL_EN > 0
    if (pidCur && (pidJoin == 0)) {
        if (__TTINY_SHELL_GET_FATHER(ptcbJoin) == pidCur) {             /*  此 shell 目标为本进程创建   */
            return  (LW_TRUE);
        }
    }
#endif                                                                  /*  LW_CFG_SHELL_EN > 0         */

    return  (LW_FALSE);
}

#endif                                                                  /*  LW_CFG_MODULELOADER_EN > 0  */
/*********************************************************************************************************
** 函数名称: API_ThreadJoin
** 功能描述: 线程合并
** 输　入  : 
**           ulId             要合并的目的线程句柄
**           ppvRetValAddr    存放线程返回值的地址
** 输　出  : ID
** 全局变量: 
** 调用模块: 
                                           API 函数
                                           
                                       (不得在中断中调用)
*********************************************************************************************************/
LW_API
ULONG  API_ThreadJoin (LW_OBJECT_HANDLE  ulId, PVOID  *ppvRetValAddr)
{
    REGISTER UINT16                usIndex;
    REGISTER PLW_CLASS_TCB         ptcbCur;
    REGISTER PLW_CLASS_TCB         ptcb;
    REGISTER PLW_CLASS_WAITJOIN    ptwj;
	
    usIndex = _ObjectGetIndex(ulId);
    
    if (LW_CPU_GET_CUR_NESTING()) {                                     /*  不能在中断中调用            */
        _DebugHandle(__ERRORMESSAGE_LEVEL, "called from ISR.\r\n");
        _ErrorHandle(ERROR_KERNEL_IN_ISR);
        return  (ERROR_KERNEL_IN_ISR);
    }
    
    LW_TCB_GET_CUR_SAFE(ptcbCur);
    
#if LW_CFG_ARG_CHK_EN > 0
    if (!_ObjectClassOK(ulId, _OBJECT_THREAD)) {                        /*  检查 ID 类型有效性          */
        _ErrorHandle(ERROR_KERNEL_HANDLE_NULL);
        return  (ERROR_KERNEL_HANDLE_NULL);
    }
    
    if (_Thread_Index_Invalid(usIndex)) {                               /*  检查线程有效性              */
        _ErrorHandle(ERROR_THREAD_NULL);
        return  (ERROR_THREAD_NULL);
    }
#endif

    __THREAD_CANCEL_POINT();                                            /*  测试取消点                  */
    
    __KERNEL_ENTER();                                                   /*  进入内核                    */

    ptcb = _K_ptcbTCBIdTable[usIndex];
    if (ptcb) {
        if (ptcb == ptcbCur) {                                          /*  不能阻塞自己                */
            __KERNEL_EXIT();                                            /*  退出内核                    */
            _DebugHandle(__ERRORMESSAGE_LEVEL, "thread join self.\r\n");
            _ErrorHandle(ERROR_THREAD_JOIN_SELF);
            return  (ERROR_THREAD_JOIN_SELF);
        }

#if LW_CFG_MODULELOADER_EN > 0
        if (!__threadCanJoin(ptcbCur, ptcb)) {                          /*  是否可以 JOIN               */
            __KERNEL_EXIT();                                            /*  退出内核                    */
            _ErrorHandle(ERROR_THREAD_NULL);
            return  (ERROR_THREAD_NULL);
        }
#endif                                                                  /*  LW_CFG_MODULELOADER_EN > 0  */

        if (ptcb->TCB_bDetachFlag) {
            __KERNEL_EXIT();                                            /*  退出内核                    */
            _ErrorHandle(ERROR_THREAD_DETACHED);
            return  (ERROR_THREAD_DETACHED);
        }

        _ThreadJoin(ptcb, LW_NULL, ppvRetValAddr);                      /*  合并                        */

    } else if (!LW_KERN_AUTO_REC_TCB_GET()) {                           /*  需要手动回收                */
        ptwj = &_K_twjTable[usIndex];
        if (ptwj->TWJ_ptcb) {
#if LW_CFG_MODULELOADER_EN > 0
            if (!__threadCanJoin(ptcbCur, ptwj->TWJ_ptcb)) {            /*  是否可以 JOIN               */
                __KERNEL_EXIT();                                        /*  退出内核                    */
                _ErrorHandle(ERROR_THREAD_NULL);
                return  (ERROR_THREAD_NULL);
            }
#endif                                                                  /*  LW_CFG_MODULELOADER_EN > 0  */
            _ThreadJoin(LW_NULL, ptwj, ppvRetValAddr);                  /*  在等待回收队列中            */

        } else {
            __KERNEL_EXIT();                                            /*  退出内核                    */
            _ErrorHandle(ERROR_THREAD_NULL);
            return  (ERROR_THREAD_NULL);
        }

    } else {
        __KERNEL_EXIT();                                                /*  退出内核                    */
        _ErrorHandle(ERROR_THREAD_NULL);
        return  (ERROR_THREAD_NULL);
    }
    
    __KERNEL_EXIT();                                                    /*  退出内核                    */
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
  END
*********************************************************************************************************/

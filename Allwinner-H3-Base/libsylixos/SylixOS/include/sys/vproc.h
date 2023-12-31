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
** 文   件   名: vproc.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2014 年 05 月 31 日
**
** 描        述: 进程扩展接口.
*********************************************************************************************************/

#ifndef __SYS_VPROC_H
#define __SYS_VPROC_H

#include <SylixOS.h>

/*********************************************************************************************************
  进程退出模式 (LW_VPROC_EXIT_FORCE 较为危险, 如果有进程正在占用操作系统资源, 则此资源无法得到释放)
*********************************************************************************************************/

#define LW_VPROC_EXIT_NORMAL    0                                       /*  正常 (等待所有子线程退出)   */
#define LW_VPROC_EXIT_FORCE     1                                       /*  强制退出 (杀死所有子线程)   */

/*********************************************************************************************************
  进程调试选项
*********************************************************************************************************/

#define LW_VPROC_DEBUG_NORMAL   0
#define LW_VPROC_DEBUG_TRAP     1                                       /*  崩溃退出时等待调试器连接    */

/*********************************************************************************************************
  进程退出模式设置
*********************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif                                                                  /*  __cplusplus                 */

LW_API INT  vprocExitModeGet(pid_t  pid, INT  *piMode);
LW_API INT  vprocExitModeSet(pid_t  pid, INT  iMode);

#if LW_CFG_GDB_EN > 0
LW_API INT  vprocDebugFlagsGet(pid_t  pid, INT  *piFlags);
LW_API INT  vprocDebugFlagsSet(pid_t  pid, INT  piFlags);
#endif                                                                  /*  LW_CFG_GDB_EN > 0           */

LW_API INT  vprocListGet(pid_t  pidTable[], UINT  uiMaxCnt);
LW_API INT  vprocMemInfo(pid_t  pid, size_t  *pstStatic, size_t  *pstHeap, size_t  *pstMmap);

/*********************************************************************************************************
  进程信息安全域
*********************************************************************************************************/

LW_API INT    vprocSecRegionGet(pid_t pid, UINT8 *pucSecReg);
LW_API INT    vprocSecRegionSet(pid_t pid, UINT8 ucSecReg);
LW_API UINT8  vprocSecRegionGetCur(VOID);
LW_API INT    vprocSecRegionSetCur(UINT8  ucSecReg);

#ifdef __cplusplus
}
#endif                                                                  /*  __cplusplus                 */

#endif                                                                  /*  __SYS_VPROC_H               */
/*********************************************************************************************************
  END
*********************************************************************************************************/

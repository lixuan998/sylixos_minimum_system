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
** 文   件   名: spipeLib.c
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2007 年 02 月 27 日
**
** 描        述: 字符流管道通信内部功能函数 (多线程同时写并且多线程同时读时, 超时时间可能不准确)

** BUG
2007.06.06  LINE71 应该是判断有效，再处理打开次数
2007.09.21  加入了互斥锁机制。
2007.09.21  加入错误管理.
2007.11.20  加入 select 功能.
2007.12.11  在清空管道信息时,需要激活写等待的线程.
2008.04.01  将 FIORTIMEOUT 和 FIOWTIMEOUT 设置超时时间的参数改为 struct timeval 类型.
            为 NULL 表示永久等待.
2009.02.09  ioctl 不可识别命令, 不打印错误信息.
2009.05.27  加入 abort 功能.
2010.01.14  升级 abort.
2010.09.09  支持 SELEXCEPT 操作. 当管道设备删除时将会被唤醒.
2011.03.27  加入 _SpipeRemove 函数.
2011.08.09  st_size 管道为内部数据量.
2011.12.13  使 write 支持 PIPE_BUF 大小内的原子操作. read 则无原子操作性.
2012.08.25  加入管道单端关闭检测:
            1: 如果读端关闭, 写操作将会收到 SIGPIPE 信号, write 将会返回 -1.
            2: 如果写端关闭, 读操作将会读完所有数据, 然后再次读返回 0.
2012.12.29  决定加入当写一个管道时, 如果没有读端, 则收到 SIGPIPE 信号.
2013.06.12  select read 没有写端, write 没有读端都需要唤醒.
            缓冲区至少有 PIPE_BUF 字节时才激活写端.
2014.03.03  优化代码.
2016.10.25  加入信号量等待保护.
2017.07.27  提高管道运行效率 (减少无谓的信号量发送).
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "../SylixOS/kernel/include/k_kernel.h"
#define  __SPIPE_MAIN_FILE
#include "../SylixOS/system/include/s_system.h"
#include "limits.h"
/*********************************************************************************************************
  EXT MODE
*********************************************************************************************************/
#define LW_SPIPE_EXT_MODE_NOSIG     0x1
#define LW_SPIPE_EXT_MODE_AUTONOMY  0x2
/*********************************************************************************************************
  共享锁操作
*********************************************************************************************************/
#define LW_SPIPE_LOCK(pspipedev, code) \
        if (API_SemaphoreMPend(pspipedev->SPIPEDEV_hOpLock, LW_OPTION_WAIT_INFINITE)) { \
            code; \
        }
#define LW_SPIPE_UNLOCK(pspipedev) \
        API_SemaphoreMPost(pspipedev->SPIPEDEV_hOpLock)
/*********************************************************************************************************
  打开同步锁操作
*********************************************************************************************************/
#define LW_SPIPE_WAIT_ROPEN(pspipedev, code) \
        if (API_SemaphorePostBPend(pspipedev->SPIPEDEV_hOpLock, \
                                   _G_ulSpipeReadOpenLock, LW_OPTION_WAIT_INFINITE)) { \
            code; \
        }
#define LW_SPIPE_WAKEUP_ROPEN() \
        API_SemaphoreBFlush(_G_ulSpipeReadOpenLock, LW_NULL)
        
#define LW_SPIPE_WAIT_WOPEN(pspipedev, code) \
        if (API_SemaphorePostBPend(pspipedev->SPIPEDEV_hOpLock, \
                                   _G_ulSpipeWriteOpenLock, LW_OPTION_WAIT_INFINITE)) { \
            code; \
        }
#define LW_SPIPE_WAKEUP_WOPEN() \
        API_SemaphoreBFlush(_G_ulSpipeWriteOpenLock, LW_NULL)
        
#define LW_SPIPE_WAKEUP_OPEN(pspipedev, flag) \
        {   \
            if ((flag & O_ACCMODE) == O_RDONLY) { \
                LW_SPIPE_WAKEUP_WOPEN(); \
            } else if ((flag & O_ACCMODE) == O_WRONLY) { \
                LW_SPIPE_WAKEUP_ROPEN(); \
            } else { \
                LW_SPIPE_WAKEUP_WOPEN(); \
                LW_SPIPE_WAKEUP_ROPEN(); \
            } \
        }
/*********************************************************************************************************
  计数操作
*********************************************************************************************************/
#define LW_SPIPE_INC_CNT(pspipedev, flag) \
        {   \
            if ((flag & O_ACCMODE) == O_RDONLY) { \
                pspipedev->SPIPEDEV_uiReadCnt++; \
            } else if ((flag & O_ACCMODE) == O_WRONLY) { \
                pspipedev->SPIPEDEV_uiWriteCnt++; \
            } else { \
                pspipedev->SPIPEDEV_uiReadCnt++; \
                pspipedev->SPIPEDEV_uiWriteCnt++; \
            } \
        }
        
#define LW_SPIPE_DEC_CNT(pspipedev, flag) \
        {   \
            if ((flag & O_ACCMODE) == O_RDONLY) { \
                pspipedev->SPIPEDEV_uiReadCnt--; \
            } else if ((flag & O_ACCMODE) == O_WRONLY) { \
                pspipedev->SPIPEDEV_uiWriteCnt--; \
            } else { \
                pspipedev->SPIPEDEV_uiReadCnt--; \
                pspipedev->SPIPEDEV_uiWriteCnt--; \
            } \
        }
/*********************************************************************************************************
  阻塞操作
*********************************************************************************************************/
#define LW_SPIPE_BLOCK(pspipedev, flag) \
        {   \
            if ((flag & O_ACCMODE) == O_RDONLY) { \
                while (!pspipedev->SPIPEDEV_uiWriteCnt) { \
                    LW_SPIPE_WAIT_ROPEN(pspipedev, return (PX_ERROR)); \
                    LW_SPIPE_LOCK(pspipedev, return (PX_ERROR)); \
                } \
            } else if ((flag & O_ACCMODE) == O_WRONLY) { \
                while (!pspipedev->SPIPEDEV_uiReadCnt) { \
                    LW_SPIPE_WAIT_WOPEN(pspipedev, return (PX_ERROR)); \
                    LW_SPIPE_LOCK(pspipedev, return (PX_ERROR)); \
                } \
            } \
        }
/*********************************************************************************************************
  check can read/write
*********************************************************************************************************/
static LW_INLINE BOOL  __spipe_can_read (size_t stMsgLen, size_t stTotal)
{
    return  (stMsgLen ? LW_TRUE : LW_FALSE);
}
static LW_INLINE BOOL  __spipe_can_write (size_t stMsgLen, size_t stTotal)
{
    if ((stTotal - stMsgLen) >= PIPE_BUF) {
        return  (LW_TRUE);
    } else {
        return  (LW_FALSE);
    }
}
/*********************************************************************************************************
  全局信号量
*********************************************************************************************************/
extern LW_OBJECT_HANDLE     _G_ulSpipeReadOpenLock;
extern LW_OBJECT_HANDLE     _G_ulSpipeWriteOpenLock;
/*********************************************************************************************************
** 函数名称: _SpipeOpen
** 功能描述: 打开字符流管道设备
** 输　入  : 
**           pspipedev        字符流管道设备控制块
**           pcName           字符流管道名称
**           iFlags           方式
**           iMode            方法
** 输　出  : ERROR
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
#if (LW_CFG_DEVICE_EN > 0) && (LW_CFG_SPIPE_EN > 0)

LONG  _SpipeOpen (PLW_SPIPE_DEV  pspipedev, 
                  PCHAR          pcName,
                  INT            iFlags, 
                  INT            iMode)
{
    PLW_SPIPE_FILE  pspipefil;

    if (pcName == LW_NULL) {
        _DebugHandle(__ERRORMESSAGE_LEVEL, "device name invalidate.\r\n");
        _ErrorHandle(ERROR_IO_NO_DEVICE_NAME_IN_PATH);
        return  (PX_ERROR);
    
    } else {
        if ((iFlags & O_CREAT) && (iFlags & O_EXCL)) {
            _ErrorHandle(ERROR_IO_FILE_EXIST);                          /*  不能重复创建                */
            return  (PX_ERROR);
        }
        if (iFlags & O_DIRECTORY) {
            _ErrorHandle(ENOTDIR);
            return  (PX_ERROR);
        }
        
        LW_SPIPE_LOCK(pspipedev, return (PX_ERROR));                    /*  锁定管道设备                */
        
        if (iFlags & O_NONBLOCK) {                                      /*  非阻塞                      */
            if ((iFlags & O_ACCMODE) == O_WRONLY) {                     /*  只写方式                    */
                if (!pspipedev->SPIPEDEV_uiReadCnt) {                   /*  没有读端                    */
                    LW_SPIPE_UNLOCK(pspipedev);                         /*  释放设备使用权              */
                    _ErrorHandle(ENXIO);
                    return  (PX_ERROR);
                }
            }
        }
        
        pspipefil = (PLW_SPIPE_FILE)__SHEAP_ALLOC(sizeof(LW_SPIPE_FILE));
        if (!pspipefil) {
            LW_SPIPE_UNLOCK(pspipedev);                                 /*  释放设备使用权              */
            _DebugHandle(__ERRORMESSAGE_LEVEL, "system low memory.\r\n");
            _ErrorHandle(ERROR_SYSTEM_LOW_MEMORY);
            return  (PX_ERROR);
        }
        
        pspipefil->SPIPEFIL_iFlags    = iFlags;
        pspipefil->SPIPEFIL_iMode     = iMode;
        pspipefil->SPIPEFIL_iExtMode  = 0;
        pspipefil->SPIPEFIL_pspipedev = pspipedev;
        
        if (!(iFlags & O_PEEKONLY)) {
            LW_SPIPE_INC_CNT(pspipedev, iFlags);                        /*  增加计数                    */
        }
        
        LW_SPIPE_UNLOCK(pspipedev);                                     /*  释放设备使用权              */
        
        LW_DEV_INC_USE_COUNT(&pspipedev->SPIPEDEV_devhdrHdr);
        
        return  ((LONG)pspipefil);
    }
}
/*********************************************************************************************************
** 函数名称: _SpipeRemove
** 功能描述: 删除字符流管道设备
** 输　入  : 
**           pspipedev        字符流管道设备控制块
**           pcName           字符流管道名称
** 输　出  : ERROR
** 全局变量: 
** 调用模块: 
** 注  意  : 此函数已经确保没有文件打开, 所以不再需要 iosDevFileAbnormal() 操作.
*********************************************************************************************************/
INT  _SpipeRemove (PLW_SPIPE_DEV  pspipedev, PCHAR  pcName)
{
    if (LW_CPU_GET_CUR_NESTING()) {
        _DebugHandle(__ERRORMESSAGE_LEVEL, "called from ISR.\r\n");
        _ErrorHandle(ERROR_KERNEL_IN_ISR);
        return  (PX_ERROR);
    }
    
    if (LW_DEV_GET_USE_COUNT(&pspipedev->SPIPEDEV_devhdrHdr)) {
        _ErrorHandle(EBUSY);                                            /*  这里不打印错误              */
        return  (PX_ERROR);
    }

    LW_SPIPE_LOCK(pspipedev, return (PX_ERROR));                        /*  获得设备操作权利            */
    
    LW_SPIPE_WAKEUP_WOPEN();                                            /*  唤醒阻塞 open 阻塞进程      */
    LW_SPIPE_WAKEUP_ROPEN();
    
    iosDevDelete(&pspipedev->SPIPEDEV_devhdrHdr);                       /*  device no longer in system  */
    
    SEL_WAKE_UP_LIST_TERM(&pspipedev->SPIPEDEV_selwulList);
    
    API_SemaphoreBDelete(&pspipedev->SPIPEDEV_hReadLock);               /*  terminate binary semaphore  */
    API_SemaphoreBDelete(&pspipedev->SPIPEDEV_hWriteLock);
    API_SemaphoreMDelete(&pspipedev->SPIPEDEV_hOpLock);
    
    __SHEAP_FREE(pspipedev);                                            /*  free pipe memory            */
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: _SpipeBlock
** 功能描述: 等待事件发生
** 输　入  : 
**           pspipefil        字符流管道文件
** 输　出  : ERROR
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
static INT  _SpipeBlock (PLW_SPIPE_FILE  pspipefil)
{
    PLW_SPIPE_DEV  pspipedev;
    
    if (pspipefil && !(pspipefil->SPIPEFIL_iFlags & O_PEEKONLY)) {
        pspipedev = pspipefil->SPIPEFIL_pspipedev;
        
        LW_SPIPE_LOCK(pspipedev, return (PX_ERROR));                    /*  获取设备使用权              */
        
        if (!(pspipefil->SPIPEFIL_iFlags & O_NONBLOCK)) {
            LW_SPIPE_BLOCK(pspipedev, pspipefil->SPIPEFIL_iFlags);
        }
        
        LW_SPIPE_WAKEUP_OPEN(pspipedev, pspipefil->SPIPEFIL_iFlags);    /*  唤醒 Open 阻塞              */
        
        LW_SPIPE_UNLOCK(pspipedev);                                     /*  释放设备使用权              */
    }
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: _SpipeClose
** 功能描述: 关闭字符流管道文件
** 输　入  : 
**           pspipefil        字符流管道文件
** 输　出  : ERROR
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
INT  _SpipeClose (PLW_SPIPE_FILE  pspipefil)
{
    PLW_SPIPE_DEV  pspipedev;

    if (pspipefil) {
        pspipedev = pspipefil->SPIPEFIL_pspipedev;
        
        LW_SPIPE_LOCK(pspipedev, return (PX_ERROR));                    /*  获取设备使用权              */
              
        if (!(pspipefil->SPIPEFIL_iFlags & O_PEEKONLY)) {
            LW_SPIPE_DEC_CNT(pspipedev, pspipefil->SPIPEFIL_iFlags);
            
            if (pspipedev->SPIPEDEV_uiWriteCnt == 0) {                  /*  没有写端                    */
                if (!(pspipefil->SPIPEFIL_iExtMode & LW_SPIPE_EXT_MODE_AUTONOMY)) {
                    SEL_WAKE_UP_ALL(&pspipedev->SPIPEDEV_selwulList, SELREAD);
                }
                API_SemaphoreBPost(pspipedev->SPIPEDEV_hReadLock);
            }
            if (pspipedev->SPIPEDEV_uiReadCnt == 0) {                   /*  没有读端                    */
                if (!(pspipefil->SPIPEFIL_iExtMode & LW_SPIPE_EXT_MODE_AUTONOMY)) {
                    SEL_WAKE_UP_ALL(&pspipedev->SPIPEDEV_selwulList, SELWRITE);
                }
                API_SemaphoreBPost(pspipedev->SPIPEDEV_hWriteLock);
            }
        }
        
        LW_SPIPE_UNLOCK(pspipedev);                                     /*  释放设备使用权              */
    
        __SHEAP_FREE(pspipefil);
    
        if (!LW_DEV_DEC_USE_COUNT(&pspipedev->SPIPEDEV_devhdrHdr)) {
            if (pspipedev->SPIPEDEV_bUnlinkReq) {
                _SpipeRemove(pspipedev, "");                            /*  删除设备                    */
            }
        }
        return  (ERROR_NONE);
    
    } else {
        return  (PX_ERROR);
    }
}
/*********************************************************************************************************
** 函数名称: _SpipeRead
** 功能描述: 读字符流管道设备
** 输　入  : 
**           pspipefil        字符流管道文件
**           pcBuffer         接收缓冲区
**           stMaxBytes       接收缓冲区大小
** 输　出  : ERROR
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
ssize_t  _SpipeRead (PLW_SPIPE_FILE  pspipefil, 
                     PCHAR           pcBuffer, 
                     size_t          stMaxBytes)
{
    REGISTER size_t     stNBytes;
    REGISTER ssize_t    sstRetVal;
    
    REGISTER PCHAR      pcBase;                                         /*  缓冲区基地址                */
    REGISTER PCHAR      pcEnd;                                          /*  缓冲区结束地址              */
    REGISTER PCHAR      pcOut;                                          /*  输出指针                    */
    
    REGISTER ULONG      ulLwErrCode;
             ULONG      ulTimeout;
             BOOL       bNonblock;
             BOOL       bOrgWriteEn;
    
             PLW_SPIPE_DEV  pspipedev = pspipefil->SPIPEFIL_pspipedev;
    
    if (!pcBuffer) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    if (!stMaxBytes) {
        return  (0);
    }
    
    if (LW_CPU_GET_CUR_NESTING()) {                                     /*  是否在中断中调用            */
        _DebugHandle(__ERRORMESSAGE_LEVEL, "called from ISR.\r\n");
        _ErrorHandle(ERROR_KERNEL_IN_ISR);
        return  (PX_ERROR);                                             /*  不能在中断中调用            */
    }
    
    pspipedev->SPIPEDEV_iAbortFlag &= ~OPT_RABORT;                      /*  清除 abort                  */
    
    if ((pspipedev->SPIPEDEV_uiWriteCnt == 0) &&                        /*  没有写端且没有数据          */
        (pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stMsgBytes == 0)) {
        if (!(pspipefil->SPIPEFIL_iExtMode & LW_SPIPE_EXT_MODE_NOSIG)) {
            return  (0);
        }
    }
    
    if (pspipefil->SPIPEFIL_iFlags & O_NONBLOCK) {                      /*  非阻塞 IO                   */
        ulTimeout = LW_OPTION_NOT_WAIT;
        bNonblock = LW_TRUE;
    } else {
        ulTimeout = pspipedev->SPIPEDEV_ulRTimeout;
        bNonblock = LW_FALSE;
    }
    
    for (;;) {
        LW_SPIPE_LOCK(pspipedev, return (PX_ERROR));                    /*  获取设备使用权              */

        if (pspipedev->SPIPEDEV_iAbortFlag & OPT_RABORT) {
            LW_SPIPE_UNLOCK(pspipedev);
            _ErrorHandle(ERROR_IO_ABORT);
            return  (0);
        }

        if (pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stMsgBytes) {
            break;                                                      /*  有数据可读                  */
        
        } else {
#if LW_CFG_SPIPE_MULTI_EN > 0
            API_SemaphoreBClear(pspipedev->SPIPEDEV_hReadLock);         /*  数据不可读                  */
#endif
            if (pspipedev->SPIPEDEV_uiWriteCnt == 0) {                  /*  已经不存在写端              */
                LW_SPIPE_UNLOCK(pspipedev);
                return  (0);
            }
        }
        
        LW_SPIPE_UNLOCK(pspipedev);                                     /*  释放设备使用权              */
        
        ulLwErrCode = API_SemaphoreBPend(pspipedev->SPIPEDEV_hReadLock, /*  等待数据有效                */
                                         ulTimeout);
        if (ulLwErrCode != ERROR_NONE) {                                /*  超时                        */
            if (bNonblock) {
                _ErrorHandle(EAGAIN);
            } else {
                _ErrorHandle(ETIMEDOUT);
            }
            return  (0);
        }
    }
    
    stNBytes  = ((stMaxBytes < pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stMsgBytes) ?
                (stMaxBytes) : (pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stMsgBytes));
    
    sstRetVal = (ssize_t)stNBytes;                                      /*  计算返回值                  */
              
    pcBase = pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_pcBuffer;
    pcEnd  = pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_pcBuffer + 
             pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stTotalBytes;
    pcOut  = pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_pcOutPtr;

    {
        REGISTER size_t stLen = pcEnd - pcOut;                          /*  剩余部分长度                */
        
        if (stNBytes > stLen) {
            lib_memcpy(pcBuffer, pcOut, stLen);                         /*  后半部分                    */
            pcBuffer += stLen;
            lib_memcpy(pcBuffer, pcBase, (stNBytes - stLen));           /*  前半部分                    */
            pcOut = pcBase + (stNBytes - stLen);
        } else if (stNBytes < stLen) {
            lib_memcpy(pcBuffer, pcOut, stNBytes);                      /*  直接拷贝                    */
            pcOut += stNBytes;
        } else {
            lib_memcpy(pcBuffer, pcOut, stNBytes);
            pcOut = pcBase;                                             /*  后半部分全部拷贝            */
        }
    }
    
    if (__spipe_can_write(pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stMsgBytes,
                          pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stTotalBytes)) {
        bOrgWriteEn = LW_TRUE;                                          /*  判断之前是否可以写入数据    */
    
    } else {
        bOrgWriteEn = LW_FALSE;
    }
    
    pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stMsgBytes -= (size_t)sstRetVal;
    pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_pcOutPtr    = pcOut;
    
    if (__spipe_can_write(pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stMsgBytes,
                          pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stTotalBytes) && !bOrgWriteEn) {
        SEL_WAKE_UP_ALL(&pspipedev->SPIPEDEV_selwulList, SELWRITE);
        API_SemaphoreBPost(pspipedev->SPIPEDEV_hWriteLock);             /*  通知可以写入数据            */
    }
    
#if LW_CFG_SPIPE_MULTI_EN > 0
    if (__spipe_can_read(pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stMsgBytes,
                         pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stTotalBytes)) {
        API_SemaphoreBPost(pspipedev->SPIPEDEV_hReadLock);              /*  通知还有数据可读            */
    }
#endif
    
    LW_SPIPE_UNLOCK(pspipedev);                                         /*  释放设备使用权              */
    
    return  (sstRetVal);
}
/*********************************************************************************************************
** 函数名称: _SpipeWrite
** 功能描述: 写字符流管道设备
** 输　入  : 
**           pspipefil        字符流管道文件
**           pcBuffer         将要写入的数据指针
**           stNBytes         写入数据大小
** 输　出  : ERROR
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
ssize_t  _SpipeWrite (PLW_SPIPE_FILE  pspipefil, 
                      PCHAR           pcBuffer, 
                      size_t          stNBytes)
{
    REGISTER size_t     stNBytesToWrite;
    REGISTER ssize_t    sstNBytes = (ssize_t)stNBytes;
    REGISTER size_t     stRetVal;
    REGISTER size_t     stFreeByteSize;
    
    REGISTER PCHAR      pcBase;                                         /*  缓冲区基地址                */
    REGISTER PCHAR      pcEnd;                                          /*  缓冲区结束地址              */
    REGISTER PCHAR      pcIn;                                           /*  输入指针                    */
    
    REGISTER ULONG      ulLwErrCode;
             ULONG      ulTimeout;
             BOOL       bNonblock;
             BOOL       bOrgReadEn;
    
             PLW_SPIPE_DEV  pspipedev = pspipefil->SPIPEFIL_pspipedev;
    
    if (!pcBuffer || !stNBytes) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }
    
    if (LW_CPU_GET_CUR_NESTING()) {                                     /*  是否在中断中调用            */
        _DebugHandle(__ERRORMESSAGE_LEVEL, "called from ISR.\r\n");
        _ErrorHandle(ERROR_KERNEL_IN_ISR);
        return  (PX_ERROR);
    }
    
__continue_write:
    pspipedev->SPIPEDEV_iAbortFlag &= ~OPT_WABORT;                      /*  清除 abort                  */
    
    if (pspipedev->SPIPEDEV_uiReadCnt == 0) {                           /*  没有读端                    */
#if LW_CFG_SIGNAL_EN > 0
        if (!(pspipefil->SPIPEFIL_iExtMode & LW_SPIPE_EXT_MODE_NOSIG)) {
            sigevent_t  sigeventPipe;

            sigeventPipe.sigev_signo           = SIGPIPE;
            sigeventPipe.sigev_value.sival_ptr = LW_NULL;
            sigeventPipe.sigev_notify          = SIGEV_SIGNAL;
            _doSigEvent(API_ThreadIdSelf(), &sigeventPipe, SI_MESGQ);   /*  产生 SIGPIPE 信号           */
        }
#endif                                                                  /*  LW_CFG_SIGNAL_EN > 0        */
        _ErrorHandle(EPIPE);
        return  (PX_ERROR);
    }
    
    if (pspipefil->SPIPEFIL_iFlags & O_NONBLOCK) {                      /*  非阻塞 IO                   */
        ulTimeout = LW_OPTION_NOT_WAIT;
        bNonblock = LW_TRUE;

    } else {
        ulTimeout = pspipedev->SPIPEDEV_ulWTimeout;
        bNonblock = LW_FALSE;
    }
    
    for (;;) {
        LW_SPIPE_LOCK(pspipedev, return (PX_ERROR));                    /*  获取设备使用权              */

        if (pspipedev->SPIPEDEV_iAbortFlag & OPT_WABORT) {
            LW_SPIPE_UNLOCK(pspipedev);
            _ErrorHandle(ERROR_IO_ABORT);
            return  (0);
        }
        
        if (pspipedev->SPIPEDEV_uiReadCnt == 0) {                       /*  没有读端                    */
            LW_SPIPE_UNLOCK(pspipedev);
            _ErrorHandle(EPIPE);
            return  (PX_ERROR);
        }
        
        stFreeByteSize = pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stTotalBytes - 
                         pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stMsgBytes;
        
        if (stFreeByteSize >= __MIN(stNBytes, PIPE_BUF)) {              /*  判断空闲空间是否满足需要    */
            break;
        
        } 
#if LW_CFG_SPIPE_MULTI_EN > 0
          else {
            API_SemaphoreBClear(pspipedev->SPIPEDEV_hWriteLock);        /*  不可写入                    */
        }
#endif
        
        LW_SPIPE_UNLOCK(pspipedev);                                     /*  释放设备使用权              */
    
        ulLwErrCode = API_SemaphoreBPend(pspipedev->SPIPEDEV_hWriteLock,/*  等待空间写入                */
                                         ulTimeout);
        if (ulLwErrCode != ERROR_NONE) {                                /*  超时                        */
            if (bNonblock) {
                _ErrorHandle(EAGAIN);
            } else {
                _ErrorHandle(ETIMEDOUT);
            }
            return  (sstNBytes - stNBytes);
        }
    }
    
    stNBytesToWrite = ((stNBytes < stFreeByteSize) ?
                      (stNBytes) : (stFreeByteSize));
                     
    stRetVal = (size_t)stNBytesToWrite;                                 /*  计算返回值                  */
                     
    pcBase = pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_pcBuffer;
    pcEnd  = pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_pcBuffer + 
             pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stTotalBytes;
    pcIn   = pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_pcInPtr;

    {
        REGISTER size_t stLen = pcEnd - pcIn;                           /*  剩余部分长度                */
        
        if (stNBytesToWrite > stLen) {
            lib_memcpy(pcIn, pcBuffer, stLen);                          /*  后半部分                    */
            pcBuffer += stLen;
            lib_memcpy(pcBase, pcBuffer, (stNBytesToWrite - stLen));    /*  前半部分                    */
            pcIn      = pcBase + (stNBytesToWrite - stLen);
            pcBuffer += stNBytesToWrite - stLen;
        } else if (stNBytesToWrite < stLen) {
            lib_memcpy(pcIn, pcBuffer, stNBytesToWrite);                /*  直接拷贝                    */
            pcIn     += stNBytesToWrite;
            pcBuffer += stNBytesToWrite;
        } else {
            lib_memcpy(pcIn, pcBuffer, stNBytesToWrite);
            pcIn      = pcBase;                                         /*  后半部分全部拷贝            */
            pcBuffer += stNBytesToWrite;
        }
    }
    
    if (__spipe_can_read(pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stMsgBytes,
                         pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stTotalBytes)) {
        bOrgReadEn = LW_TRUE;
        
    } else {
        bOrgReadEn = LW_FALSE;
    }
    
    pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stMsgBytes += (size_t)stRetVal;
    pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_pcInPtr     = pcIn;
    
    if (__spipe_can_read(pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stMsgBytes,
                         pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stTotalBytes) && !bOrgReadEn) {
        SEL_WAKE_UP_ALL(&pspipedev->SPIPEDEV_selwulList, SELREAD);
        API_SemaphoreBPost(pspipedev->SPIPEDEV_hReadLock);              /*  通知还有数据可读            */
    }
    
#if LW_CFG_SPIPE_MULTI_EN > 0
    if (__spipe_can_write(pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stMsgBytes,
                          pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stTotalBytes)) {
        API_SemaphoreBPost(pspipedev->SPIPEDEV_hWriteLock);             /*  通知可以写入数据            */
    }
#endif
    
    LW_SPIPE_UNLOCK(pspipedev);
    
    stNBytes -= stRetVal;
    if (stNBytes) {
        goto    __continue_write;
    }
    
    return  (sstNBytes);
}
/*********************************************************************************************************
** 函数名称: _SpipeIoctl
** 功能描述: 控制字符流管道设备
** 输　入  : 
**           pspipefil        字符流管道文件
**           iRequest         功能
**           piArgPtr         参数
** 输　出  : ERROR
** 全局变量: 
** 调用模块: 
** 注  意  : FIONMSGS 与 VxWorks 不兼容.
*********************************************************************************************************/
INT  _SpipeIoctl (PLW_SPIPE_FILE pspipefil, 
                  INT            iRequest, 
                  INT           *piArgPtr)
{
    REGISTER INT                  iErrCode = ERROR_NONE;
    REGISTER PCHAR                pcBufferBase;
    
    REGISTER PLW_SEL_WAKEUPNODE   pselwunNode;
             struct stat         *pstatGet;
             PLW_SPIPE_DEV        pspipedev = pspipefil->SPIPEFIL_pspipedev;
    
    switch (iRequest) {
    
    case FIOSEEK:
    case FIOWHERE:
        iErrCode = PX_ERROR;
        _ErrorHandle(ESPIPE);
        break;
    
    case FIONREAD:                                                      /*  获得管道中数据的字节个数    */
        *(INT *)piArgPtr = (INT)pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stMsgBytes;
        break;
        
    case FIONMSGS:                                                      /*  获得管道中数据的个数        */
        if (pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stMsgBytes) {
            *piArgPtr = 1;
        } else {
            *piArgPtr = 0;
        }
        break;
        
    case FIONFREE:                                                      /*  空闲空间大小                */
        *(INT *)piArgPtr = (INT)(pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stTotalBytes
                         - pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stMsgBytes);
        break;

    case FIONBIO:
        LW_SPIPE_LOCK(pspipedev, return (PX_ERROR));
        if (*piArgPtr) {
            pspipefil->SPIPEFIL_iFlags |= O_NONBLOCK;
        } else {
            pspipefil->SPIPEFIL_iFlags &= ~O_NONBLOCK;
        }
        LW_SPIPE_UNLOCK(pspipedev);
        break;
        
    case FIOPIPEBLOCK:                                                  /*  尝试阻塞                    */
        iErrCode = _SpipeBlock(pspipefil);
        break;
        
    case FIOPIPERDONLY:                                                 /*  仅 pipe 函数使用            */
        LW_SPIPE_LOCK(pspipedev, return (PX_ERROR));
        if ((pspipefil->SPIPEFIL_iFlags & O_ACCMODE) == O_RDWR) {
            pspipefil->SPIPEFIL_iFlags &= ~O_ACCMODE;                   /*  RDONLY == 0                 */
            pspipedev->SPIPEDEV_uiWriteCnt--;
        } else {
            iErrCode = PX_ERROR;
            _ErrorHandle(ENOTSUP);
        }
        LW_SPIPE_UNLOCK(pspipedev);
        break;
        
    case FIOFLUSH:                                                      /*  清空数据                    */
        pcBufferBase = pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_pcBuffer;
        LW_SPIPE_LOCK(pspipedev, return (PX_ERROR));
        pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_pcInPtr    = pcBufferBase;
        pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_pcOutPtr   = pcBufferBase;
        pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stMsgBytes = 0;
        LW_SPIPE_UNLOCK(pspipedev);
        
        SEL_WAKE_UP_ALL(&pspipedev->SPIPEDEV_selwulList, SELWRITE);     /*  通知数据可写                */
        API_SemaphoreBPost(pspipedev->SPIPEDEV_hWriteLock);
        break;
        
    case FIOFSTATGET:                                                   /*  获取文件属性                */
        pstatGet = (struct stat *)piArgPtr;
        if (pstatGet) {
            pstatGet->st_dev     = LW_DEV_MAKE_STDEV(&pspipedev->SPIPEDEV_devhdrHdr);
            pstatGet->st_ino     = (ino_t)0;                            /*  相当于唯一节点              */
            pstatGet->st_mode    = 0666 | S_IFIFO;
            pstatGet->st_nlink   = 1;
            pstatGet->st_uid     = 0;
            pstatGet->st_gid     = 0;
            pstatGet->st_rdev    = 1;
            pstatGet->st_size    = (off_t)pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stMsgBytes;
            pstatGet->st_blksize = 0;
            pstatGet->st_blocks  = 0;
            pstatGet->st_atime   = pspipedev->SPIPEDEV_timeCreate;
            pstatGet->st_mtime   = pspipedev->SPIPEDEV_timeCreate;
            pstatGet->st_ctime   = pspipedev->SPIPEDEV_timeCreate;
        } else {
            return  (PX_ERROR);
        }
        break;
    
    case FIOSELECT:
        pselwunNode = (PLW_SEL_WAKEUPNODE)piArgPtr;
        SEL_WAKE_NODE_ADD(&pspipedev->SPIPEDEV_selwulList, pselwunNode);
        
        switch (pselwunNode->SELWUN_seltypType) {
        
        case SELREAD:                                                   /*  等待数据可读                */
            if (__spipe_can_read(pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stMsgBytes,
                                 pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stTotalBytes)) {
                SEL_WAKE_UP(pselwunNode);                               /*  唤醒节点                    */
            } else if ((pspipedev->SPIPEDEV_uiWriteCnt == 0) &&
                       !(pspipefil->SPIPEFIL_iExtMode & LW_SPIPE_EXT_MODE_AUTONOMY)) {
                SEL_WAKE_UP(pselwunNode);                               /*  没有写端也需要唤醒节点      */
            }
            break;
            
        case SELWRITE:
            if (__spipe_can_write(pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stMsgBytes,
                                  pspipedev->SPIPEDEV_ringbufferBuffer.RINGBUFFER_stTotalBytes)) {
                SEL_WAKE_UP(pselwunNode);                               /*  唤醒节点                    */
            } else if (pspipedev->SPIPEDEV_uiReadCnt == 0) {
                SEL_WAKE_UP(pselwunNode);                               /*  没有读端也需要唤醒节点      */
            }
            break;
            
        case SELEXCEPT:                                                 /*  设备删除时将会被唤醒        */
            break;
        }
        break;

    case FIOUNSELECT:
        SEL_WAKE_NODE_DELETE(&pspipedev->SPIPEDEV_selwulList, (PLW_SEL_WAKEUPNODE)piArgPtr);
        break;
        
    case FIORTIMEOUT:                                                   /*  设置读超时时间              */
        {
            struct timeval *ptvTimeout = (struct timeval *)piArgPtr;
            REGISTER ULONG  ulTick;
            if (ptvTimeout) {
                ulTick = __timevalToTick(ptvTimeout);                   /*  获得 tick 数量              */
                pspipedev->SPIPEDEV_ulRTimeout = ulTick;
            } else {
                pspipedev->SPIPEDEV_ulRTimeout = LW_OPTION_WAIT_INFINITE;
            }
        }
        break;
        
    case FIOWTIMEOUT:
        {
            struct timeval *ptvTimeout = (struct timeval *)piArgPtr;
            REGISTER ULONG  ulTick;
            if (ptvTimeout) {
                ulTick = __timevalToTick(ptvTimeout);                   /*  获得 tick 数量              */
                pspipedev->SPIPEDEV_ulWTimeout = ulTick;
            } else {
                pspipedev->SPIPEDEV_ulWTimeout = LW_OPTION_WAIT_INFINITE;
            }
        }
        break;
    
    case FIOWAITABORT:                                                  /*  停止当前等待 IO 线程        */
        LW_SPIPE_LOCK(pspipedev, return (PX_ERROR));
        if ((INT)(LONG)piArgPtr & OPT_RABORT) {
            ULONG  ulBlockNum;
            API_SemaphoreBStatus(pspipedev->SPIPEDEV_hReadLock, LW_NULL, LW_NULL, &ulBlockNum);
            if (ulBlockNum) {
                pspipedev->SPIPEDEV_iAbortFlag |= OPT_RABORT;
                API_SemaphoreBPost(pspipedev->SPIPEDEV_hReadLock);      /*  激活读等待线程              */
            }
        }
        if ((INT)(LONG)piArgPtr & OPT_WABORT) {
            ULONG  ulBlockNum;
            API_SemaphoreBStatus(pspipedev->SPIPEDEV_hWriteLock, LW_NULL, LW_NULL, &ulBlockNum);
            if (ulBlockNum) {
                pspipedev->SPIPEDEV_iAbortFlag |= OPT_WABORT;
                API_SemaphoreBPost(pspipedev->SPIPEDEV_hWriteLock);     /*  激活读等待线程              */
            }
        }
        LW_SPIPE_UNLOCK(pspipedev);
        break;
        
    case FIOUNMOUNT:                                                    /*  最后一次关闭时删除设备      */
        LW_SPIPE_LOCK(pspipedev, return (PX_ERROR));
        pspipedev->SPIPEDEV_bUnlinkReq = LW_TRUE;
        LW_SPIPE_UNLOCK(pspipedev);
        break;
        
    case FIOPIPENOSIG:                                                  /*  不需要信号                  */
        LW_SPIPE_LOCK(pspipedev, return (PX_ERROR));
        if ((INT)(LONG)piArgPtr) {
            pspipefil->SPIPEFIL_iExtMode |= LW_SPIPE_EXT_MODE_NOSIG;
        } else {
            pspipefil->SPIPEFIL_iExtMode &= ~LW_SPIPE_EXT_MODE_NOSIG;
        }
        LW_SPIPE_UNLOCK(pspipedev);
        break;

    case FIOPIPEAUTONOMY:                                               /*  select 不受对端影响         */
        LW_SPIPE_LOCK(pspipedev, return (PX_ERROR));
        if ((INT)(LONG)piArgPtr) {
            pspipefil->SPIPEFIL_iExtMode |= LW_SPIPE_EXT_MODE_AUTONOMY;
        } else {
            pspipefil->SPIPEFIL_iExtMode &= ~LW_SPIPE_EXT_MODE_AUTONOMY;
        }
        LW_SPIPE_UNLOCK(pspipedev);
        break;

    case FIOSYNC:                                                       /*  不能进行同步操作            */
        iErrCode = PX_ERROR;
        _ErrorHandle(EINVAL);
        break;

    default:
        iErrCode = PX_ERROR;
        _ErrorHandle(ERROR_IO_UNKNOWN_REQUEST);
        break;
    }
    
    return  (iErrCode);
}

#endif                                                                  /*  (LW_CFG_DEVICE_EN > 0) &&   */
                                                                        /*  (LW_CFG_PIPE_EN > 0)        */
/*********************************************************************************************************
  END
*********************************************************************************************************/

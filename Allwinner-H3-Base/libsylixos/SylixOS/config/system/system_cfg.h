/*********************************************************************************************************
**
**                                    中国软件开源组织
**
**                                   嵌入式实时操作系统
**
**                                       SylixOS(TM)
**
**                               Copyright  All Rights Reserved
**
**--------------文件信息--------------------------------------------------------------------------------
**
** 文   件   名: system_cfg.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2006 年 12 月 12 日
**
** 描        述: 这是系统基本配置文件。
*********************************************************************************************************/

#ifndef __SYSTEM_CFG_H
#define __SYSTEM_CFG_H

/*********************************************************************************************************
*                               控件总数量控制
*********************************************************************************************************/

#define LW_CFG_MAX_DRIVERS                      64      /*  系统使用的最多驱动程序              < 65535 */
#define LW_CFG_MAX_FILES                        200     /*  系统文件描述符个数，                < 65536 */

#define LW_CFG_MAX_THREAD_POOLS                 2       /*  系统管理的最大线程池数量，          < 65535 */
#define LW_CFG_MAX_EXCEMSGS                     20      /*  系统异常消息缓冲的最大数量          < 65536 */
#define LW_CFG_MAX_LOGMSGS                      64      /*  系统日志消息缓冲的最大数量          < 65536 */

#define LW_CFG_MAX_DMA_CHANNELS                 64      /*  系统的 DMA 通道数量                 < 65535 */
#define LW_CFG_MAX_DMA_LISTNODES                64      /*  系统 DMA 请求作业队列的数量, 超过此数量,    */
                                                        /*  系统将使用动态内存分配节点                  */

#define LW_CFG_MAX_SIGQUEUE_NODES               100     /*  系统所有信号等待队列最大节点数              */

#define LW_CFG_MAX_GPIOS                        256     /*  系统总 GPIO 数量                            */

/*********************************************************************************************************
*                                     DMA 功能管理
*
* 依存关系: 1: 信号服务 (需要 t_except 服务线程支持)
*********************************************************************************************************/

#define LW_CFG_DMA_EN                           1       /*  是否允许系统提供对 DMA 的支持               */

/*********************************************************************************************************
*                                     线程池功能管理
*
* 依存关系: 1: 互斥信号量
*********************************************************************************************************/

#define LW_CFG_THREAD_POOL_EN                   1       /*  是否允许线程池功能管理                      */

/*********************************************************************************************************
*                   信号管理 (系统需要系统需要建立一个任务支持信号处理)
*
* 依存关系: 1: 消息队列
*********************************************************************************************************/

#define LW_CFG_SIGNAL_EN                        1       /*  是否允许系统使用信号                        */

/*********************************************************************************************************
*    POSIX 定时器及其附件配置, 如果需要 BSD Net/3 网络支持则需要同时开通信号服务和 posix 定时器服务
*
* 依存关系: 1: 信号服务
            2: 内核定时器服务
*********************************************************************************************************/

#define LW_CFG_PTIMER_EN                        1       /*  是否允许操作系统开通 posix 定时器服务       */
#define LW_CFG_PTIMER_AUTO_DEL_EN               0       /*  当线程删除时, 他所创建的 posix 定时器自动   */
                                                        /*  被删除, 使用本选项较耗费资源, 建议自己调用  */
                                                        /*  删除的 posix 定时器API:timer_delete()删除   */

/*********************************************************************************************************
*                                           I/O 系统
* 依存关系: 1: 二进制信号量
            2: 互斥信号量
            3: 计数信号量
            4: RTC 支持
*********************************************************************************************************/

#define LW_CFG_DEVICE_EN                        1       /*  是否允许系统设备管理                        */
                                                        
                                                        /*  建议设置为 0 使用分级管理设备               */
#define LW_CFG_PATH_VXWORKS                     0       /*  是否使用 VxWorks 兼容的 I/O 设备目录结构    */
                                                        /*  VxWorks 将所有设备都挂入统一的单级链表      */
                                                        /*  为 0 时: SylixOS 根文件系统将分级管理设备   */
                                                        /*  详细情况请见 doc/FAQ 帮助文档               */
                                                        
                                                        /*  建议不要改动以下两个配置                    */
#define LW_CFG_PATH_MAX                         512     /*  最长的目录名长短                            */
#define LW_CFG_DIR_MAX                          256     /*  最深的目录级数                              */

/*********************************************************************************************************
*                                      设备树功能
*********************************************************************************************************/

#define LW_CFG_DEVTREE_EN                       1       /*  是否允许系统提供设备树支持                  */

/*********************************************************************************************************
*                                     兼容 Linux 新加入可等待异步事件功能
* 依存关系: 1: 二进制信号量
            2: 互斥信号量
            3: I/O系统
            4: 信号服务
            5: 定时器
*********************************************************************************************************/

#define LW_CFG_SIGNALFD_EN                      1       /*  是否加入 signalfd 功能模块                  */
#define LW_CFG_TIMERFD_EN                       1       /*  是否加入 timerfd 功能模块                   */
#define LW_CFG_EVENTFD_EN                       1       /*  是否加入 eventfd 功能模块                   */

/*********************************************************************************************************
*                                     多路 I/O 复用支持配置
* 依存关系: 1: 二进制信号量
            2: 互斥信号量
            3: I/O系统
*********************************************************************************************************/

#define LW_CFG_SELECT_EN                        1       /*  是否支持 select pselect 等I/O多路复用功能   */
#define LW_CFG_SELECT_INTER_EN                  1       /*  select 系列函数是否支持被信号打断           */
#define LW_CFG_EPOLL_EN                         1       /*  是否支持部分 linux epoll 兼容子系统         */

/*********************************************************************************************************
*                                     系统性能分析
*
* 依存关系: 1: 消息队列
            2: loader
*********************************************************************************************************/

#define LW_CFG_SYSPERF_EN                       1       /*  是否允许系统性能分析                        */

/*********************************************************************************************************
*                                     系统/应用崩溃信息暂存
*
* 依存关系: 1: 设备管理
*********************************************************************************************************/

#define LW_CFG_CDUMP_EN                         1       /*  是否允许系统/应用崩溃信息暂存               */
#define LW_CFG_CDUMP_BUF_SIZE                   2048    /*  缓存大小                                    */
#define LW_CFG_CDUMP_CALL_STACK_DEPTH           20      /*  调用栈深度                                  */

/*********************************************************************************************************
*                                      I/O 系统监控支持
* 依存关系: 1: 二进制信号量
            2: 互斥信号量
            3: I/O系统
*********************************************************************************************************/

#define LW_CFG_INOTIFY_EN                       1       /*  是否支持 linux 系统兼容的 inotify 功能      */
#define LW_CFG_INOTIFY_MAX_QUEUE_EVENTS         256     /*  最大事件队列长度                            */
#define LW_CFG_INOTIFY_MAX_INSTANCES            128     /*  最多可以创建多少个 inotify 实例             */
#define LW_CFG_INOTIFY_MAX_WATCHES              2048    /*  inotify 最大的 watcher 数量                 */

/*********************************************************************************************************
*                                      GPIO 标准接口
* 依存关系: 1: I/O系统
*********************************************************************************************************/

#define LW_CFG_GPIO_EN                          1       /*  是否使能标准的 GPIO 抽象接口                */

/*********************************************************************************************************
*                                      图形化显示设备
* 依存关系: 1: I/O系统
*********************************************************************************************************/

#define LW_CFG_GRAPH_EN                         1       /*  是否使能图形化设备模型                      */

/*********************************************************************************************************
*                                      字符流管道通信管理
* 依存关系: 1: 二进制信号量
            2: 互斥信号量
            3: I/O系统
*********************************************************************************************************/
	
#define LW_CFG_SPIPE_EN                         1       /*  是否允许系统提供字符流管道通信支持          */

#ifdef __SYLIXOS_LITE                                   /*  pipe 与 mkfifo 创建的管道大小               */
#define LW_CFG_SPIPE_DEFAULT_SIZE               (8 * 1024)
#else                                                   /*  注意: 不得小于 PIPE_BUF 定义 (limits.h)     */
#define LW_CFG_SPIPE_DEFAULT_SIZE               (32 * 1024)
#endif

#define LW_CFG_SPIPE_MULTI_EN                   1       /*  是否有应用多个线程发送, 多个线程接收同一管道*/
                                                        /*  如果没有可设为 0 提高效率, 这种应用很少见   */
                                                        /*  为了兼容性, 这里默认设置为 1                */
/*********************************************************************************************************
*                                      管道通信管理
* 依存关系: 1: 二进制信号量
            2: 互斥信号量
            3: 消息队列
            4: I/O系统
*********************************************************************************************************/
	
#define LW_CFG_PIPE_EN                          1       /*  是否允许系统提供管道通信支持                */

/*********************************************************************************************************
*                                      有边界消息设备
* 依存关系: 1: I/O系统
*********************************************************************************************************/

#define LW_CFG_BMSG_EN                          1       /*  是否允许系统提供边界消息设备支持            */

/*********************************************************************************************************
*                                      信号量设备
* 依存关系: 1: I/O系统
*********************************************************************************************************/

#define LW_CFG_SEMFD_EN                         1       /*  将信号量文件化, 可支持 select 等 I/O 复用   */

/*********************************************************************************************************
*                                      内存设备管理
* 依存关系: 1: 二进制信号量
            2: 互斥信号量
            4: I/O系统
*********************************************************************************************************/

#define LW_CFG_MEMDEV_EN                        1       /*  是否允许系统提供内存设备支持                */

/*********************************************************************************************************
*                                      CAN 设备管理
* 依存关系: 1: 二进制信号量
            2: 互斥信号量
            3: I/O系统
*********************************************************************************************************/

#define LW_CFG_CAN_EN                           1       /*  是否允许系统提供 CAN 设备抽象               */

/*********************************************************************************************************
*                                      BUZZER 设备管理
* 依存关系: 1: 二进制信号量
            2: 消息队列
            3: I/O系统
*********************************************************************************************************/

#define LW_CFG_BUZZER_EN                        1       /*  是否允许系统 BUZZER 设备抽象                */

/*********************************************************************************************************
*                                      SDCARD 设备管理 (系统包含有 SD 总线模型)
* 依存关系: 1: 二进制信号量
            2: 互斥信号量
            3: I/O系统
            4: disk cache
*********************************************************************************************************/

#define LW_CFG_SDCARD_EN                        1       /*  是否允许系统提供 SD 卡设备抽象              */
                                                        /*  系统将提供 SD 卡标准设备模型                */
#define LW_CFG_SDCARD_SDIO_EN                   1       /*  是否允许提供 SDIO 协议支持                  */
                                                        /*  它依赖与SD卡设备抽象模块                    */
#define LW_CFG_SDCARD_CRC_EN                    1       /*  是否允许 SD 卡 CRC 计算                     */
                                                        
/*********************************************************************************************************
*                                      PCI 设备管理
* 依存关系: 1: 二进制信号量
            2: 互斥信号量
            3: I/O系统
*********************************************************************************************************/

#define LW_CFG_PCI_EN                           1       /*  是否允许系统提供 PCI 设备抽象               */
#define LW_CFG_PCI_64                           1       /*  PCI 64 bit 地址支持 (必须为 1)              */

/*********************************************************************************************************
*                                      USB 主控设备管理
* 依存关系: 1: 二进制信号量
            2: 互斥信号量
            3: I/O系统
*********************************************************************************************************/

#define LW_CFG_USB_EN                           1       /*  是否允许系统提供 USB 主控设备管理           */

/*********************************************************************************************************
*                                      ATA 设备管理
* 依存关系: 1: 二进制信号量
            2: 互斥信号量
            3: I/O系统
*********************************************************************************************************/

#define LW_CFG_ATA_EN                           1       /*  是否允许系统提供 ATA 设备管理               */
#define LW_CFG_ATA_MAX_CTLS                     2       /*  最大 IDE 控制器个数                         */

/*********************************************************************************************************
*                                      AHCI 设备管理
* 依存关系: 1: 二进制信号量
            2: 互斥信号量
            3: I/O系统
            4: PCI设备
            5: shell
*********************************************************************************************************/

#define LW_CFG_AHCI_EN                          1       /*  是否允许系统提供 AHCI 主控设备管理          */
#define LW_CFG_AHCI_TRIM_EN                     1       /*  是否允许系统使用 TRIM 命令                  */
#define LW_CFG_AHCI_CACHE_SIZE_MB               1       /*  SATA 盘文件系统 CACHE 大小, 单位: MB        */

/*********************************************************************************************************
*                                      NVMe 设备管理
* 依存关系: 1: 二进制信号量
            2: 互斥信号量
            3: I/O系统
            4: PCI设备
            5: shell
            6: vmm
*********************************************************************************************************/

#define LW_CFG_NVME_EN                          1       /*  是否允许系统提供 NVMe 主控设备管理          */
#define LW_CFG_NVME_TRIM_EN                     1       /*  是否允许系统使用 TRIM 命令                  */
#define LW_CFG_NVME_CACHE_SIZE_MB               1       /*  NVMe 盘文件系统 CACHE 大小, 单位: MB        */

/*********************************************************************************************************
*                               SIO 设备管理 (TTY 与 PTY 设备)
* 依存关系: 1: 二进制信号量
            2: 互斥信号量
            3: I/O系统
*********************************************************************************************************/

#define LW_CFG_SIO_DEVICE_EN                    1       /*  是否允许系统管理同步设备                    */
#define LW_CFG_SIO_TERMINAL_NOT_7_BIT           1       /*  tty 终端不使用经典的 7 bit 模式 (软件过滤)  */
                                                        /*  即 OPT_TERMINAL 不包含 OPT_7_BIT 选项       */
/*********************************************************************************************************
*                                        虚拟终端设备管理
* 依存关系: 1: 二进制信号量
            2: 互斥信号量
            3: I/O系统
            4: TTY 设备支持 (SIO)
*********************************************************************************************************/

#define LW_CFG_PTY_DEVICE_EN                    1       /*  是否允许系统管理虚拟终端设备                */

/*********************************************************************************************************
*                                   共享内存设备管理 (用于进程间通信)
* 依存关系: 1: 二进制信号量
            2: 互斥信号量
            3: I/O系统
            4: VMM 虚拟内存管理
*********************************************************************************************************/

#define LW_CFG_SHM_DEVICE_EN                    1       /*  是否允许进程间通信共享内存类型设备          */

/*********************************************************************************************************
*                                      FIO LIB 库支持
* 依存关系: 1: 二进制信号量
            2: 互斥信号量
            3: I/O系统
*********************************************************************************************************/

#define LW_CFG_FIO_LIB_EN                       1       /*  是否允许系统提供 FIO LIB 库                 */
#define LW_CFG_FIO_STDIN_LINE_EN                1       /*  stdin  初始化时是否使能 line buffer 模式    */
#define LW_CFG_FIO_STDOUT_LINE_EN               1       /*  stdout 初始化时是否使能 line buffer 模式    */
#define LW_CFG_FIO_STDERR_LINE_EN               1       /*  stderr 初始化时是否使能 line buffer 模式    */

/*********************************************************************************************************
*                                        日志管理库
* 依存关系: 1: 消息队列
            2: 定长内存管理
            3: I/O系统
*********************************************************************************************************/

#define LW_CFG_LOG_LIB_EN                       1       /*  是否允许系统提供日志管理库                  */
#define LW_CFG_LOG_MSG_LEN_MAX                  1024    /*  printk 最大消息长度                         */

/*********************************************************************************************************
*                                      功耗管理器支持
* 依存关系: 1: 互斥信号量
*********************************************************************************************************/

#define LW_CFG_POWERM_EN                        1       /*  是否允许系统提供功耗管理器功能              */
#define LW_CFG_POWERM_OPTION                    LW_OPTION_THREAD_STK_CHK
                                                        /*  功耗管理器服务线程选项                      */

/*********************************************************************************************************
*                                      热插拔消息与循环检测支持
* 依存关系: 1: 互斥信号量
            2: 消息队列
*********************************************************************************************************/

#define LW_CFG_HOTPLUG_EN                       1       /*  是否允许系统提供热插拔消息支持              */
#define LW_CFG_HOTPLUG_OPTION                   LW_OPTION_THREAD_STK_CHK
                                                        /*  热插拔消息服务线程选项                      */
#define LW_CFG_HOTPLUG_MAX_MSGS                 8       /*  最大并发消息数量(即系统可缓冲的消息数量)    */

#define LW_CFG_HOTPLUG_DEV_DEFAULT_BUFSIZE      4096    /*  /dev/hotplug 设备默认缓冲区大小             */

#endif                                                  /*  __SYSTEM_CFG_H                              */
/*********************************************************************************************************
  END
*********************************************************************************************************/

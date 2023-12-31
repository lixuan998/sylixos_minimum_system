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
** 文   件   名: net_tools_cfg.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2008 年 03 月 13 日
**
** 描        述: 网络工具配置.

** BUG:
2011.03.10  合并 syslog 工具, 使复合 posix 标准, 所以配置不再放在这里.
*********************************************************************************************************/

#ifndef __NET_TOOLS_CFG_H
#define __NET_TOOLS_CFG_H

/*********************************************************************************************************
                                            ping (ICMP)
*  依存关系: 1: 网络
             2: shell
             3: thread ext
             4: raw
*********************************************************************************************************/

#define LW_CFG_NET_PING_EN                          1                   /*  是否需要 ping 工具          */

/*********************************************************************************************************
                                            telnet (TCP : 23)
*  依存关系: 1: 网络
             2: shell
             3: pty
             4: thread cancel
             5: tcp
*********************************************************************************************************/

#define LW_CFG_NET_TELNET_EN                        1                   /*  是否使能 telnet 工具        */
#define LW_CFG_NET_TELNET_LOGFD_EN                  0                   /*  是否添加到 logmsg 输出文件集*/
#define LW_CFG_NET_TELNET_LOGIN_EN                  1                   /*  是否使能用户登陆            */
#if LW_CFG_CPU_WORD_LENGHT == 32
#define LW_CFG_NET_TELNET_STK_SIZE                  (6 * LW_CFG_KB_SIZE)/*  telnet 相关线程堆栈         */
#else
#define LW_CFG_NET_TELNET_STK_SIZE                 (12 * LW_CFG_KB_SIZE)/*  telnet 相关线程堆栈         */
#endif
#define LW_CFG_NET_TELNET_MAX_LINKS                 10                  /*  telnet 最大连接数, 建议为 5 */
#define LW_CFG_NET_TELNET_RBUFSIZE                  128                 /*  pty read buffer size        */
#define LW_CFG_NET_TELNET_WBUFSIZE                  128                 /*  pty write buffer size       */
                                                                        /*  缓冲区为默认值, ioctl 可修改*/

/*********************************************************************************************************
                                            netbios (UDP : 137)
*  依存关系: 1: 网络
             2: udp
*********************************************************************************************************/

#define LW_CFG_NET_NETBIOS_EN                       1                   /*  是否使能简易netbios名字服务 */

/*********************************************************************************************************
                                            tftp (UDP : 69)
*  依存关系: 1: 网络
             2: 文件系统
             3: udp
*  注  意  : tftp client 主要作为 sylixos 系统的本地 bootloader 无盘引导, 调试或升级系统镜像.
             tftp server 主要作为其他伙伴系统的镜像升级服务器.
             tftp 协议相对简单, 只要 LW_CFG_NET_TFTP_EN 为 1, 表示 tftp 服务器与客户机均使能.
*********************************************************************************************************/

#define LW_CFG_NET_TFTP_EN                          0                   /*  是否使能 tftp 服务          */
#if LW_CFG_CPU_WORD_LENGHT == 32
#define LW_CFG_NET_TFTP_STK_SIZE                    (8 * LW_CFG_KB_SIZE)/*  tftp 线程堆栈               */
#else
#define LW_CFG_NET_TFTP_STK_SIZE                   (16 * LW_CFG_KB_SIZE)/*  tftp 线程堆栈               */
#endif

/*********************************************************************************************************
                                            ftp (TCP : 21)
*  依存关系: 1: 网络
             2: tcp
*  注  意  : 因为 ftp 较为复杂, 代码量相对较大, 所以这里分开裁剪.
*********************************************************************************************************/

#define LW_CFG_NET_FTPD_EN                          1                   /*  是否使能 ftp 服务器         */
#define LW_CFG_NET_FTPD_LOG_EN                      0                   /*  是否使能 ftp 服务器打印 log */
#define LW_CFG_NET_FTPD_LOGIN_EN                    1                   /*  是否使能用户登陆            */
#if LW_CFG_CPU_WORD_LENGHT == 32
#define LW_CFG_NET_FTPD_STK_SIZE                    (12 * LW_CFG_KB_SIZE)
#else
#define LW_CFG_NET_FTPD_STK_SIZE                    (24 * LW_CFG_KB_SIZE)
#endif
                                                                        /*  ftp 线程堆栈                */
#define LW_CFG_NET_FTPD_MAX_LINKS                   10                  /*  ftp 最大客户机连接数        */
#define LW_CFG_NET_FTPD_AUTO_SYNC                   0                   /*  每次文件写保证 sync (效率低)*/
#define LW_CFG_NET_FTPC_EN                          1                   /*  是否使能 ftp 客户机         */
                                                                        /*  推荐使用 ncftp 这样的客户机 */
/*********************************************************************************************************
                                            RPC (Remote procedure call)
*  依存关系: 1: 网络
             2: udp, tcp
*********************************************************************************************************/

#define LW_CFG_NET_RPC_EN                           1                   /*  是否使能 RPC 服务           */

/*********************************************************************************************************
                                            NAT (网络地址转换, 例如:家用拨号路由器)
*  依存关系: 1: 网络
             2: 块内存管理
             3: 定时器
             4: LW_CFG_NET_ROUTER
*********************************************************************************************************/

#define LW_CFG_NET_NAT_EN                           1                   /*  是否使用 NAT 服务           */
#define LW_CFG_NET_NAT_MAX_AP_IF                    5                   /*  NAT 外网接口数量            */
#define LW_CFG_NET_NAT_MAX_LOCAL_IF                 5                   /*  NAT 内网接口数量            */
#define LW_CFG_NET_NAT_MAX_SESSION                  2048                /*  NAT 最大会话个数 256-24576  */
#define LW_CFG_NET_NAT_IDLE_TIMEOUT                 10                  /*  NAT 空闲链接超时, 单位:分钟 */

/*********************************************************************************************************
                                            VLAN tools
*  依存关系: 1: 网络
*********************************************************************************************************/

#define LW_CFG_NET_VLAN_EN                          1                   /*  是否使能 VLAN 工具          */

/*********************************************************************************************************
                                            VPN (KidVPN 虚拟专用网络)
*  依存关系: 1: 网络
             2: udp
*********************************************************************************************************/

#define LW_CFG_NET_VPN_EN                           1
#define LW_CFG_NET_KIDVPN_EN                        LW_CFG_NET_VPN_EN   /*  是否使用 KidVPN 服务        */
#define LW_CFG_NET_VNETDEV_EN                       LW_CFG_NET_VPN_EN   /*  虚拟网卡                    */

/*********************************************************************************************************
                                            NETDEV MIP (网络接口多 IP 工具)
*  依存关系: 1: 网络
*********************************************************************************************************/

#define LW_CFG_NET_NETDEV_MIP_EN                    1                   /*  是否使能网络接口多 IP 工具  */

/*********************************************************************************************************
                                            QoS TOOL (基于规则的 QoS 工具)
*  依存关系: 1: 网络
*********************************************************************************************************/

#define LW_CFG_NET_QOS_EN                           1                   /*  是否使能 QoS 配置工具       */

/*********************************************************************************************************
                                            NPF (基于规则的网络数据报过滤器)
*  依存关系: 1: 网络
             2: udp, tcp
*********************************************************************************************************/

#define LW_CFG_NET_NPF_EN                           1                   /*  是否使能 NPF 服务           */

/*********************************************************************************************************
                                            SNTP (简单网络时间协议)
*  依存关系: 1: 网络
             2: udp
*********************************************************************************************************/

#define LW_CFG_NET_SNTP_EN                          1                   /*  是否使能 SNTP 服务          */

/*********************************************************************************************************
                                            SNMP (简单网络管理协议)
*  依存关系: 1: 网络
             2: V3 依赖 mbedTLS
             3: udp
*********************************************************************************************************/

#define LW_CFG_NET_SNMP_V1_EN                       0                   /*  是否使能 SNMPv1 服务        */
#define LW_CFG_NET_SNMP_V2_EN                       0                   /*  是否使能 SNMPv2 服务        */
#define LW_CFG_NET_SNMP_V3_EN                       1                   /*  是否使能 SNMPv3 服务        */
                                                                        /*  建议有 mbedTLS 情况下仅开启 */
                                                                        /*  带有加密认证功能的 SNMPv3   */
/*********************************************************************************************************
                                            LOGINBL (网络登录黑名单)
*  依存关系: 1: 网络
*********************************************************************************************************/

#define LW_CFG_NET_LOGINBL_EN                       1                   /*  网络登录黑名单服务          */
#define LW_CFG_NET_LOGINBL_MAX_NODE                 1000                /*  黑名单存储最大个数          */

/*********************************************************************************************************
                                            PPP (拨号工具)
*  依存关系: 1: 网络
*********************************************************************************************************/

#define LW_CFG_NET_PPP_TIMEOUT                      6                   /*  单次尝试超时时间 (s)        */
#define LW_CFG_NET_PPP_RETRANSMITS                  8                   /*  最大重试次数                */
#define LW_CFG_NET_PPP_ECHOINTERVAL                 10                  /*  Keepalive Echo 间隔时间     */

#endif                                                                  /*  __NET_TOOLS_CFG_H           */
/*********************************************************************************************************
  END
*********************************************************************************************************/

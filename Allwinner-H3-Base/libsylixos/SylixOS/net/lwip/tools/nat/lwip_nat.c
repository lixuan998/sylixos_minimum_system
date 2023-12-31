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
** 文   件   名: lwip_nat.c
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2010 年 03 月 19 日
**
** 描        述: lwip NAT 支持包.

** BUG:
2011.03.06  修正 gcc 4.5.1 相关 warning.
2011.07.30  nats 命令不能在网络终端(例如: telnet)中被调用, 因为他要使用 NAT 锁, printf 如果最终打印到网络
            上, 将会和 netproto 任务抢占 NAT 锁.
*********************************************************************************************************/
#define  __SYLIXOS_STDIO
#define  __SYLIXOS_KERNEL
#include "../SylixOS/kernel/include/k_kernel.h"
#include "../SylixOS/system/include/s_system.h"
#include "../SylixOS/net/include/net_net.h"
/*********************************************************************************************************
  裁剪控制
*********************************************************************************************************/
#if (LW_CFG_NET_EN > 0) && (LW_CFG_NET_ROUTER > 0) && (LW_CFG_NET_NAT_EN > 0)
#include "socket.h"
#include "net/if_lock.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "lwip/inet.h"
#include "lwip_natlib.h"
/*********************************************************************************************************
  函数声明
*********************************************************************************************************/
#if LW_CFG_SHELL_EN > 0
static INT  __tshellNat(INT  iArgC, PCHAR  ppcArgV[]);
static INT  __tshellNatIpFrag(INT  iArgC, PCHAR  ppcArgV[]);
static INT  __tshellNatLocal(INT  iArgC, PCHAR  ppcArgV[]);
static INT  __tshellNatWan(INT  iArgC, PCHAR  ppcArgV[]);
static INT  __tshellNatMap(INT  iArgC, PCHAR  ppcArgV[]);
static INT  __tshellNatAlias(INT  iArgC, PCHAR  ppcArgV[]);
static INT  __tshellNatShow(INT  iArgC, PCHAR  ppcArgV[]);
static INT  __tshellNatTraffic(INT  iArgC, PCHAR  ppcArgV[]);
#endif                                                                  /*  LW_CFG_SHELL_EN > 0         */
/*********************************************************************************************************
** 函数名称: API_INetNatInit
** 功能描述: internet NAT 初始化
** 输　入  : NONE
** 输　出  : NONE
** 全局变量: 
** 调用模块: 
                                           API 函数
*********************************************************************************************************/
LW_API  
VOID  API_INetNatInit (VOID)
{
    static   BOOL   bIsInit = LW_FALSE;
    
    if (bIsInit) {
        return;
    }
    
    __natInit();
    
#if LW_CFG_PROCFS_EN > 0
    __procFsNatInit();
#endif                                                                  /*  LW_CFG_PROCFS_EN > 0        */
    
#if LW_CFG_SHELL_EN > 0
    API_TShellKeywordAdd("nat", __tshellNat);
    API_TShellFormatAdd("nat",  " [stop] | {[LAN Iface] [WAN Iface]}");
    API_TShellHelpAdd("nat",    "start or stop NAT network.\n"
                                "eg. nat wl2 en1 (start NAT network use wl2 as LAN, en1 as WAN)\n"
                                "    nat stop    (stop NAT network)\n");
    
    API_TShellKeywordAdd("natipfrag", __tshellNatIpFrag);
    API_TShellFormatAdd("natipfrag",  " [proto] [1/0]");
    API_TShellHelpAdd("natipfrag",    "NAT set specified protocol to support IP fragment.\n"
                                      "protocol support: tcp udp icmp\n");
    
    API_TShellKeywordAdd("natlocal", __tshellNatLocal);
    API_TShellFormatAdd("natlocal",  " [LAN Iface]");
    API_TShellHelpAdd("natlocal",    "add LAN net interface to NAT network.\n");
    
    API_TShellKeywordAdd("natwan", __tshellNatWan);
    API_TShellFormatAdd("natwan",  " [WAN Iface]");
    API_TShellHelpAdd("natwan",    "add WAN net interface to NAT network.\n");
    
    API_TShellKeywordAdd("natalias", __tshellNatAlias);
    API_TShellFormatAdd("natalias",  " {[add] [alias] [LAN start] [LAN end]} | {[del] [alias]}");
    API_TShellHelpAdd("natalias",    "add or delete NAT alias.\n"
                                     "eg. natalias add 11.22.33.44 192.168.1.2 192.168.1.3  (add alias to 192..2 ~ 192..3)\n"
                                     "    natalias del 11.22.33.44                          (delete alias)\n");
                                     
    API_TShellKeywordAdd("natmap", __tshellNatMap);
    API_TShellFormatAdd("natmap",  " {[add] | [del]} [WAN port] [LAN port] [LAN IP] [protocol] [ip_cnt]");
    API_TShellHelpAdd("natmap",    "add or delete NAT maps.\n"
                                     "eg. natmap add 80 80 192.168.1.2 tcp (map webserver as 192..2)\n"
                                     "    natmap add 80 80 192.168.1.2 tcp 5 (map webserver as 192..2 ~ 192..6)\n"
                                     "    natmap del 80 80 192.168.1.2 tcp (unmap webserver as 192..2)\n");
                                     
    API_TShellKeywordAdd("nats", __tshellNatShow);
    API_TShellHelpAdd("nats",   "show NAT networking infomation.\n");

    API_TShellKeywordAdd("nattraffic", __tshellNatTraffic);
    API_TShellFormatAdd("nattraffic",  " ['on' / 'off']");
    API_TShellHelpAdd("nattraffic",    "Turn On / Off or show nat traffic state.\n");
#endif                                                                  /*  LW_CFG_SHELL_EN > 0         */
    
    bIsInit = LW_TRUE;
}
/*********************************************************************************************************
** 函数名称: API_INetNatStart
** 功能描述: 启动 NAT (外网网络接口必须为 lwip 默认的路由接口)
** 输　入  : pcLocalNetif          本地内网网络接口名
**           pcApNetif             外网网络接口名
** 输　出  : ERROR or OK
** 全局变量: 
** 调用模块: 
                                           API 函数
*********************************************************************************************************/
LW_API  
INT  API_INetNatStart (CPCHAR  pcLocalNetif, CPCHAR  pcApNetif)
{
    if (!pcLocalNetif || !pcApNetif) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }
    
    LWIP_IF_LIST_LOCK(LW_FALSE);
    if (__natStart(pcLocalNetif, pcApNetif)) {                          /*  启动 NAT                    */
        LWIP_IF_LIST_UNLOCK();
        return  (PX_ERROR);
    }
    LWIP_IF_LIST_UNLOCK();
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: API_INetNatStop
** 功能描述: 停止 NAT (注意: 删除 NAT 网络接口时, 必须要先停止 NAT 网络接口)
** 输　入  : NONE
** 输　出  : NONE
** 全局变量: 
** 调用模块: 
                                           API 函数
*********************************************************************************************************/
LW_API  
INT  API_INetNatStop (VOID)
{
    if (__natStop()) {
        _ErrorHandle(ENOTCONN);
        return  (PX_ERROR);
    }
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: API_INetNatIpFragSet
** 功能描述: NAT 设置 IP 分片支持
** 输　入  : ucProto    网络协议 IPPROTO_UDP / IPPROTO_TCP / IPPROTO_ICMP
**           bOn        是否使能分片
** 输　出  : ERROR or OK
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
LW_API  
INT  API_INetNatIpFragSet (UINT8  ucProto, BOOL  bOn)
{
    if (__natIpFragSet(ucProto, bOn)) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: API_INetNatIpFragGet
** 功能描述: NAT 获取 IP 分片支持
** 输　入  : ucProto    网络协议 IPPROTO_UDP / IPPROTO_TCP / IPPROTO_ICMP
**           pbOn       是否使能分片
** 输　出  : ERROR or OK
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
LW_API  
INT  API_INetNatIpFragGet (UINT8  ucProto, BOOL  *pbOn)
{
    if (!pbOn || __natIpFragGet(ucProto, pbOn)) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: API_INetNatLocalAdd
** 功能描述: 在 NAT 网络中增加本地接口
** 输　入  : pcLocalNetif          本地内网网络接口名
** 输　出  : ERROR or OK
** 全局变量: 
** 调用模块: 
                                           API 函数
*********************************************************************************************************/
LW_API  
INT  API_INetNatLocalAdd (CPCHAR  pcLocalNetif)
{
    LWIP_IF_LIST_LOCK(LW_FALSE);
    if (__natAddLocal(pcLocalNetif)) {
        LWIP_IF_LIST_UNLOCK();
        return  (PX_ERROR);
    }
    LWIP_IF_LIST_UNLOCK();
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: API_INetNatLocalDelete
** 功能描述: 在 NAT 网络中删除本地接口
** 输　入  : pcLocalNetif          本地内网网络接口名
** 输　出  : ERROR or OK
** 全局变量:
** 调用模块:
** 注  意  : 不需要 IF list 锁
                                           API 函数
*********************************************************************************************************/
LW_API
INT  API_INetNatLocalDelete (CPCHAR  pcLocalNetif)
{
    return  (__natDeleteLocal(pcLocalNetif));
}
/*********************************************************************************************************
** 函数名称: API_INetNatWanAdd
** 功能描述: 在 NAT 网络中增加 WAN 接口
** 输　入  : pcApNetif             外网网络接口名
** 输　出  : ERROR or OK
** 全局变量: 
** 调用模块: 
                                           API 函数
*********************************************************************************************************/
LW_API  
INT  API_INetNatWanAdd (CPCHAR  pcApNetif)
{
    LWIP_IF_LIST_LOCK(LW_FALSE);
    if (__natAddAp(pcApNetif)) {
        LWIP_IF_LIST_UNLOCK();
        return  (PX_ERROR);
    }
    LWIP_IF_LIST_UNLOCK();
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: API_INetNatWanDelete
** 功能描述: 在 NAT 网络中删除 WAN 接口
** 输　入  : pcApNetif             外网网络接口名
** 输　出  : ERROR or OK
** 全局变量:
** 调用模块:
** 注  意  : 不需要 IF list 锁
                                           API 函数
*********************************************************************************************************/
LW_API
INT  API_INetNatWanDelete (CPCHAR  pcApNetif)
{
    return  (__natDeleteAp(pcApNetif));
}
/*********************************************************************************************************
** 函数名称: API_INetNatMapAdd
** 功能描述: 创建内网外网映射
** 输　入  : pcLocalIp         内网 IP
**           usIpCnt           内网 IP 个数 (负载均衡)
**           usLocalPort       内网 端口
**           usAssPort         映射 端口
**           ucProto           协议 IP_PROTO_TCP / IP_PROTO_UDP
** 输　出  : ERROR or OK
** 全局变量: 
** 调用模块: 
                                           API 函数
*********************************************************************************************************/
LW_API  
INT  API_INetNatMapAdd (CPCHAR  pcLocalIp, UINT16  usIpCnt, UINT16  usLocalPort, 
                        UINT16  usAssPort, UINT8  ucProto)
{
    ip4_addr_t ipaddr;
    INT        iRet;
    
    if (!pcLocalIp) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }
    
    if (usIpCnt < 1) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }
    
    if (usAssPort >= LW_CFG_NET_NAT_MIN_PORT) {
        _ErrorHandle(EADDRINUSE);
        return  (PX_ERROR);
    }
    
    if ((ucProto != IPPROTO_TCP) && (ucProto != IPPROTO_UDP)) {
        _ErrorHandle(ENOPROTOOPT);
        return  (PX_ERROR);
    }
    
    if (!ip4addr_aton(pcLocalIp, &ipaddr)) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }
    
    iRet = __natMapAdd(&ipaddr, usIpCnt, htons(usLocalPort), htons(usAssPort), ucProto);
    
    return  (iRet);
}
/*********************************************************************************************************
** 函数名称: API_INetNatMapDelete
** 功能描述: 删除内网外网映射
** 输　入  : pcLocalIp         内网 IP
**           usLocalPort       内网 端口
**           usAssPort         映射 端口
**           ucProto           协议 IP_PROTO_TCP / IP_PROTO_UDP
** 输　出  : ERROR or OK
** 全局变量: 
** 调用模块: 
                                           API 函数
*********************************************************************************************************/
LW_API  
INT  API_INetNatMapDelete (CPCHAR  pcLocalIp, UINT16  usLocalPort, UINT16  usAssPort, UINT8  ucProto)
{
    ip4_addr_t ipaddr;
    INT        iRet;
    
    if (!pcLocalIp) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }
    
    if (usAssPort >= LW_CFG_NET_NAT_MIN_PORT) {
        _ErrorHandle(EADDRINUSE);
        return  (PX_ERROR);
    }
    
    if ((ucProto != IPPROTO_TCP) && (ucProto != IPPROTO_UDP)) {
        _ErrorHandle(ENOPROTOOPT);
        return  (PX_ERROR);
    }
    
    if (!ip4addr_aton(pcLocalIp, &ipaddr)) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }
    
    iRet = __natMapDelete(&ipaddr, htons(usLocalPort), htons(usAssPort), ucProto);
    
    return  (iRet);
}
/*********************************************************************************************************
** 函数名称: API_INetNatAliasAdd
** 功能描述: NAT 增加网络别名 (公网 IP)
** 输　入  : pcAliasIp         别名 IP
**           pcSLocalIp        本地 IP 起始位置
**           pcELocalIp        本地 IP 结束位置
** 输　出  : ERROR or OK
** 全局变量: 
** 调用模块: 
                                           API 函数
*********************************************************************************************************/
LW_API  
INT  API_INetNatAliasAdd (CPCHAR  pcAliasIp, CPCHAR  pcSLocalIp, CPCHAR  pcELocalIp)
{
    ip4_addr_t  ipaddrAlias;
    ip4_addr_t  ipaddrSLocalIp;
    ip4_addr_t  ipaddrELocalIp;
    INT         iRet;
    
    if (!pcAliasIp || !pcSLocalIp || !pcELocalIp) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }
    
    if (!ip4addr_aton(pcAliasIp, &ipaddrAlias)) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }
    
    if (!ip4addr_aton(pcSLocalIp, &ipaddrSLocalIp)) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }
    
    if (!ip4addr_aton(pcELocalIp, &ipaddrELocalIp)) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }
    
    iRet = __natAliasAdd(&ipaddrAlias, &ipaddrSLocalIp, &ipaddrELocalIp);
    
    return  (iRet);
}
/*********************************************************************************************************
** 函数名称: API_INetNatAliasDelete
** 功能描述: NAT 删除网络别名 (公网 IP)
** 输　入  : pcAliasIp         别名 IP
** 输　出  : ERROR or OK
** 全局变量: 
** 调用模块: 
                                           API 函数
*********************************************************************************************************/
LW_API  
INT  API_INetNatAliasDelete (CPCHAR  pcAliasIp)
{
    ip4_addr_t  ipaddrAlias;
    INT         iRet;
    
    if (!pcAliasIp) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }
    
    if (!ip4addr_aton(pcAliasIp, &ipaddrAlias)) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }
    
    iRet = __natAliasDelete(&ipaddrAlias);
    
    return  (iRet);
}
/*********************************************************************************************************
** 函数名称: API_INetNatTrafficStatSet
** 功能描述: NAT 打开 / 关闭节点流量统计功能
** 输　入  : bTurnOn       是否打开
** 输　出  : ERROR or OK
** 全局变量:
** 调用模块:
                                           API 函数
*********************************************************************************************************/
LW_API
INT  API_INetNatTrafficStatSet (BOOL  bTurnOn)
{
    if (bTurnOn) {
        __natTrafficOn();
    } else {
        __natTrafficOff();
    }

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: API_INetNatTrafficStatGet
** 功能描述: NAT 获取节点流量统计功能
** 输　入  : NONE
** 输　出  : 是否打开
** 全局变量:
** 调用模块:
                                           API 函数
*********************************************************************************************************/
LW_API
BOOL  API_INetNatTrafficStatGet (VOID)
{
    return  (__natTrafficIsOn());
}
/*********************************************************************************************************
** 函数名称: __tshellNat
** 功能描述: 系统命令 "nat"
** 输　入  : iArgC         参数个数
**           ppcArgV       参数表
** 输　出  : 0
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
#if LW_CFG_SHELL_EN > 0

static INT  __tshellNat (INT  iArgC, PCHAR  ppcArgV[])
{
    if (iArgC == 3) {
        if (API_INetNatStart(ppcArgV[1], ppcArgV[2]) != ERROR_NONE) {
            fprintf(stderr, "can not start NAT network, errno: %s\n", lib_strerror(errno));
            return  (PX_ERROR);
        
        } else {
            printf("NAT network started, [LAN: %s] [WAN: %s]\n", ppcArgV[1], ppcArgV[2]);
        }
    
    } else if (iArgC == 2) {
        if (lib_strcmp(ppcArgV[1], "stop") == 0) {
            API_INetNatStop();
            printf("NAT network stoped.\n");
        }

    } else {
        fprintf(stderr, "option error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __tshellNatIpFrag
** 功能描述: 系统命令 "natipfrag"
** 输　入  : iArgC         参数个数
**           ppcArgV       参数表
** 输　出  : 0
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
static INT  __tshellNatIpFrag (INT  iArgC, PCHAR  ppcArgV[])
{
    INT   iOn;
    BOOL  bOn;
    
    if (iArgC != 3) {
        printf("NAT IP Fragment: ");
        if (API_INetNatIpFragGet(IPPROTO_TCP, &bOn) == ERROR_NONE) {
            printf("TCP-%s ", bOn ? "Enable" : "Disable");
        }
        if (API_INetNatIpFragGet(IPPROTO_UDP, &bOn) == ERROR_NONE) {
            printf("UDP-%s ", bOn ? "Enable" : "Disable");
        }
        if (API_INetNatIpFragGet(IPPROTO_ICMP, &bOn) == ERROR_NONE) {
            printf("ICMP-%s ", bOn ? "Enable" : "Disable");
        }
        printf("\n");
        return  (ERROR_NONE);
    }
    
    if (sscanf(ppcArgV[2], "%d", &iOn) != 1) {
        fprintf(stderr, "option error! (On: 1 Off: 0)\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    bOn = (BOOL)iOn;
    
    if (!lib_strcasecmp(ppcArgV[1], "tcp")) {
        API_INetNatIpFragSet(IPPROTO_TCP, bOn);
        
    } else if (!lib_strcasecmp(ppcArgV[1], "udp")) {
        API_INetNatIpFragSet(IPPROTO_UDP, bOn);
    
    } else if (!lib_strcasecmp(ppcArgV[1], "icmp")) {
        API_INetNatIpFragSet(IPPROTO_ICMP, bOn);
    
    } else {
        fprintf(stderr, "protocol option error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __tshellNatLocal
** 功能描述: 系统命令 "natlocal"
** 输　入  : iArgC         参数个数
**           ppcArgV       参数表
** 输　出  : 0
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
static INT  __tshellNatLocal (INT  iArgC, PCHAR  ppcArgV[])
{
    if (iArgC != 2) {
        fprintf(stderr, "option error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    if (API_INetNatLocalAdd(ppcArgV[1]) != ERROR_NONE) {
        fprintf(stderr, "can not add NAT network local net interface, errno: %s\n", lib_strerror(errno));
        return  (PX_ERROR);
    }
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __tshellNatWan
** 功能描述: 系统命令 "natwan"
** 输　入  : iArgC         参数个数
**           ppcArgV       参数表
** 输　出  : 0
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
static INT  __tshellNatWan (INT  iArgC, PCHAR  ppcArgV[])
{
    if (iArgC != 2) {
        fprintf(stderr, "option error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    if (API_INetNatWanAdd(ppcArgV[1]) != ERROR_NONE) {
        fprintf(stderr, "can not add NAT network WAN net interface, errno: %s\n", lib_strerror(errno));
        return  (PX_ERROR);
    }
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __tshellNatMap
** 功能描述: 系统命令 "natmap"
** 输　入  : iArgC         参数个数
**           ppcArgV       参数表
** 输　出  : 0
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
static INT  __tshellNatMap (INT  iArgC, PCHAR  ppcArgV[])
{
    INT         iError;
    INT         iIpCnt;
    INT         iWanPort;
    INT         iLanPort;
    UINT8       ucProto;
    
    if (iArgC < 6) {
        fprintf(stderr, "option error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    if (sscanf(ppcArgV[2], "%d", &iWanPort) != 1) {
        fprintf(stderr, "WAN port error option error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    if (sscanf(ppcArgV[3], "%d", &iLanPort) != 1) {
        fprintf(stderr, "LAN port error option error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    if (lib_strcasecmp(ppcArgV[5], "tcp") == 0) {
        ucProto = IPPROTO_TCP;
        
    } else if (lib_strcasecmp(ppcArgV[5], "udp") == 0) {
        ucProto = IPPROTO_UDP;
    
    } else {
        fprintf(stderr, "protocol option error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    if (iArgC > 6) {
        if ((sscanf(ppcArgV[6], "%d", &iIpCnt) != 1) ||
            (iIpCnt < 1) || (iIpCnt > __ARCH_USHRT_MAX)) {
            fprintf(stderr, "IP cnt error option error!\n");
            return  (-ERROR_TSHELL_EPARAM);
        }
    
    } else {
        iIpCnt = 1;
    }
    
    if (lib_strcmp(ppcArgV[1], "add") == 0) {
        iError = API_INetNatMapAdd(ppcArgV[4], (UINT16)iIpCnt, (UINT16)iLanPort, (UINT16)iWanPort, ucProto);
        if (iError < 0) {
            fprintf(stderr, "add NAT map error: %s!\n", lib_strerror(errno));
            return  (-ERROR_TSHELL_EPARAM);
        }
        
    } else if (lib_strcmp(ppcArgV[1], "del") == 0) {
        iError = API_INetNatMapDelete(ppcArgV[4], (UINT16)iLanPort, (UINT16)iWanPort, ucProto);
        if (iError < 0) {
            fprintf(stderr, "delete NAT map error: %s!\n", lib_strerror(errno));
            return  (-ERROR_TSHELL_EPARAM);
        }
        
    } else {
        fprintf(stderr, "option error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __tshellNatAlias
** 功能描述: 系统命令 "natalias"
** 输　入  : iArgC         参数个数
**           ppcArgV       参数表
** 输　出  : 0
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
static INT  __tshellNatAlias (INT  iArgC, PCHAR  ppcArgV[])
{
    INT    iError;
    
    if (iArgC == 5) {
        if (lib_strcmp(ppcArgV[1], "add")) {
            goto    __error;
        }
        iError = API_INetNatAliasAdd(ppcArgV[2], ppcArgV[3], ppcArgV[4]);
        if (iError < 0) {
            fprintf(stderr, "add NAT alias error: %s!\n", lib_strerror(errno));
            return  (-ERROR_TSHELL_EPARAM);
        }
    
    } else if (iArgC == 3) {
        if (lib_strcmp(ppcArgV[1], "del")) {
            goto    __error;
        }
        iError = API_INetNatAliasDelete(ppcArgV[2]);
        if (iError < 0) {
            fprintf(stderr, "delete NAT alias error: %s!\n", lib_strerror(errno));
            return  (-ERROR_TSHELL_EPARAM);
        }
    
    } else {
__error:
        fprintf(stderr, "option error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __tshellNatShow
** 功能描述: 系统命令 "nats"
** 输　入  : iArgC         参数个数
**           ppcArgV       参数表
** 输　出  : 0
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
static INT  __tshellNatShow (INT  iArgC, PCHAR  ppcArgV[])
{
    INT     iFd;
    CHAR    cBuffer[512];
    ssize_t sstNum;
    
    iFd = open("/proc/net/nat/info", O_RDONLY);
    if (iFd < 0) {
        fprintf(stderr, "can not open /proc/net/nat/info : %s\n", lib_strerror(errno));
        return  (PX_ERROR);
    }
    
    do {
        sstNum = read(iFd, cBuffer, sizeof(cBuffer));
        if (sstNum > 0) {
            write(STDOUT_FILENO, cBuffer, (size_t)sstNum);
        }
    } while (sstNum > 0);
    
    close(iFd);
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __tshellNatTraffic
** 功能描述: 系统命令 "nattraffic"
** 输　入  : iArgC         参数个数
**           ppcArgV       参数表
** 输　出  : 0
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT  __tshellNatTraffic (INT  iArgC, PCHAR  ppcArgV[])
{
    BOOL  bIsOn;

    if (iArgC == 2) {
        if (lib_strcasecmp(ppcArgV[1], "on") == 0) {
            __natTrafficOn();
            return  (ERROR_NONE);

        } else if (lib_strcasecmp(ppcArgV[1], "off") == 0) {
            __natTrafficOff();
            return  (ERROR_NONE);

        } else {
            fprintf(stderr, "on / off option error!\n");
            return  (-ERROR_TSHELL_EPARAM);
        }

    } else {
        bIsOn = __natTrafficIsOn();
        printf("NAT traffic statistics %s\n", bIsOn ? "on" : "off");
        return  (ERROR_NONE);
    }
}

#endif                                                                  /*  LW_CFG_SHELL_EN > 0         */
#endif                                                                  /*  LW_CFG_NET_EN > 0           */
                                                                        /*  LW_CFG_NET_ROUTER > 0       */
                                                                        /*  LW_CFG_NET_NAT_EN > 0       */
/*********************************************************************************************************
  END
*********************************************************************************************************/

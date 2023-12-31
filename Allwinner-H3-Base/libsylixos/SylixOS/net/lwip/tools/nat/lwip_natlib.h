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
** 文   件   名: lwip_natlib.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2010 年 03 月 19 日
**
** 描        述: lwip NAT 支持包内部库.
*********************************************************************************************************/

#ifndef __LWIP_NATLIB_H
#define __LWIP_NATLIB_H

/*********************************************************************************************************
  裁剪控制
*********************************************************************************************************/
#if (LW_CFG_NET_EN > 0) && (LW_CFG_NET_ROUTER > 0) && (LW_CFG_NET_NAT_EN > 0)
/*********************************************************************************************************
  NAT 控制块
*********************************************************************************************************/
typedef struct {
    union {
        LW_LIST_MONO    NATL_monoFree;                                  /*  空闲链                      */
        LW_LIST_LINE    NATL_lineManage;                                /*  控制块链表 (源端口 hash)    */
    } l;
#define NAT_monoFree    l.NATL_monoFree
#define NAT_lineManage  l.NATL_lineManage

    LW_LIST_LINE        NAT_lineAssHash;                                /*  AssPort hash                */
    u8_t                NAT_ucInAssHash;                                /*  是否在 AssPort hash         */
    u8_t                NAT_ucProto;                                    /*  协议                        */
    u16_t               NAT_usMapHash;                                  /*  负载均衡源地址 hash         */
    ip4_addr_t          NAT_ipaddrLocal;                                /*  本地 IP 地址                */
    u16_t               NAT_usLocalPort;                                /*  本地端口号                  */
    u16_t               NAT_usAssPort;                                  /*  映射端口号 (唯一的)         */
    u16_t               NAT_usAssPortSave;                              /*  MAP 映射的端口需要保存      */
    
    u32_t               NAT_uiLocalSequence;                            /*  TCP 序列号                  */
    u32_t               NAT_uiLocalRcvNext;                             /*  TCP 期望接收                */

    ULONG               NAT_ulIdleTimer;                                /*  空闲定时器                  */
    INT                 NAT_iStatus;                                    /*  通信状态                    */
#define __NAT_STATUS_OPEN           0
#define __NAT_STATUS_SYN            1
#define __NAT_STATUS_FIN            2
#define __NAT_STATUS_CLOSING        3
} __NAT_CB;
typedef __NAT_CB       *__PNAT_CB;
/*********************************************************************************************************
  NAT 外网主动连接映射关系
*********************************************************************************************************/
typedef struct {
    LW_LIST_LINE        NATM_lineManage;
    
    u8_t                NATM_ucProto;                                   /*  协议                        */
    ip4_addr_t          NATM_ipaddrLocalIp;                             /*  本地 IP 地址                */
    u16_t               NATM_usLocalCnt;                                /*  本地 IP 段个数 (负载均衡)   */
    u16_t               NATM_usLocalPort;                               /*  本地端口号                  */
    u16_t               NATM_usAssPort;                                 /*  映射端口号 (唯一的)         */
} __NAT_MAP;
typedef __NAT_MAP      *__PNAT_MAP;
/*********************************************************************************************************
  NAT 别名表
*********************************************************************************************************/
typedef struct {
    LW_LIST_LINE        NATA_lineManage;
    
    ip4_addr_t          NATA_ipaddrAliasIp;                             /*  别名地址                    */
    ip4_addr_t          NATA_ipaddrSLocalIp;                            /*  别名对应本地 IP 范围        */
    ip4_addr_t          NATA_ipaddrELocalIp;
} __NAT_ALIAS;
typedef __NAT_ALIAS    *__PNAT_ALIAS;

/*********************************************************************************************************
  网卡事件回调
*********************************************************************************************************/
VOID  nat_netif_add_hook(struct netif *pnetif);
VOID  nat_netif_remove_hook(struct netif *pnetif);

/*********************************************************************************************************
  函数声明
*********************************************************************************************************/
VOID        __natInit(VOID);
INT         __natStart(CPCHAR  pcLocal, CPCHAR  pcAp);
INT         __natStop(VOID);
INT         __natIpFragSet(UINT8  ucProto, BOOL  bOn);
INT         __natIpFragGet(UINT8  ucProto, BOOL  *pbOn);
INT         __natAddLocal(CPCHAR  pcLocal);
INT         __natDeleteLocal(CPCHAR  pcLocal);
INT         __natAddAp(CPCHAR  pcAp);
INT         __natDeleteAp(CPCHAR  pcAp);
INT         __natMapAdd(ip4_addr_t  *pipaddr, u16_t  usIpCnt, u16_t  usPort, u16_t  AssPort, u8_t  ucProto);
INT         __natMapDelete(ip4_addr_t  *pipaddr, u16_t  usPort, u16_t  AssPort, u8_t  ucProto);
INT         __natAliasAdd(const ip4_addr_t  *pipaddrAlias, 
                          const ip4_addr_t  *ipaddrSLocalIp,
                          const ip4_addr_t  *ipaddrELocalIp);
INT         __natAliasDelete(const ip4_addr_t  *pipaddrAlias);
VOID        __natTrafficOn(VOID);
VOID        __natTrafficOff(VOID);
BOOL        __natTrafficIsOn(VOID);

#if LW_CFG_PROCFS_EN > 0
VOID        __procFsNatInit(VOID);
#endif                                                                  /*  LW_CFG_PROCFS_EN > 0        */

#endif                                                                  /*  LW_CFG_NET_EN > 0           */
                                                                        /*  LW_CFG_NET_ROUTER > 0       */
                                                                        /*  LW_CFG_NET_NAT_EN > 0       */
#endif                                                                  /*  __LWIP_NATLIB_H             */
/*********************************************************************************************************
  END
*********************************************************************************************************/

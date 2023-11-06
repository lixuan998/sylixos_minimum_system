#*********************************************************************************************************
#
#                                    中国软件开源组织
#
#                                   嵌入式实时操作系统
#
#                                SylixOS(TM)  LW : long wing
#
#                               Copyright All Rights Reserved
#
#--------------文件信息--------------------------------------------------------------------------------
#
# 文   件   名: tcpdump.mk
#
# 创   建   人: RealEvo-IDE
#
# 文件创建日期: 2017 年 06 月 09 日
#
# 描        述: 本文件由 RealEvo-IDE 生成，用于配置 Makefile 功能，请勿手动修改
#*********************************************************************************************************

#*********************************************************************************************************
# Clear setting
#*********************************************************************************************************
include $(CLEAR_VARS_MK)

#*********************************************************************************************************
# Target
#*********************************************************************************************************
LOCAL_TARGET_NAME := tcpdump

#*********************************************************************************************************
# Source list
#*********************************************************************************************************
LOCAL_SRCS :=  \
tcpdump/addrtoname.c \
tcpdump/addrtostr.c \
tcpdump/af.c \
tcpdump/ascii_strcasecmp.c \
tcpdump/bpf_dump.c \
tcpdump/checksum.c \
tcpdump/cpack.c \
tcpdump/gmpls.c \
tcpdump/gmt2local.c \
tcpdump/in_cksum.c \
tcpdump/ipproto.c \
tcpdump/l2vpn.c \
tcpdump/machdep.c \
tcpdump/missing/datalinks.c \
tcpdump/missing/dlnames.c \
tcpdump/missing/snprintf.c \
tcpdump/missing/strdup.c \
tcpdump/missing/strlcat.c \
tcpdump/missing/strlcpy.c \
tcpdump/missing/strsep.c \
tcpdump/nlpid.c \
tcpdump/oui.c \
tcpdump/parsenfsfh.c \
tcpdump/pcap_dump_ftell.c \
tcpdump/print-802_11.c \
tcpdump/print-802_15_4.c \
tcpdump/print-ah.c \
tcpdump/print-ahcp.c \
tcpdump/print-aodv.c \
tcpdump/print-aoe.c \
tcpdump/print-ap1394.c \
tcpdump/print-arcnet.c \
tcpdump/print-arp.c \
tcpdump/print-ascii.c \
tcpdump/print-atalk.c \
tcpdump/print-atm.c \
tcpdump/print-babel.c \
tcpdump/print-beep.c \
tcpdump/print-bfd.c \
tcpdump/print-bgp.c \
tcpdump/print-bootp.c \
tcpdump/print-bt.c \
tcpdump/print-calm-fast.c \
tcpdump/print-carp.c \
tcpdump/print-cdp.c \
tcpdump/print-cfm.c \
tcpdump/print-chdlc.c \
tcpdump/print-cip.c \
tcpdump/print-cnfp.c \
tcpdump/print-dccp.c \
tcpdump/print-decnet.c \
tcpdump/print-dhcp6.c \
tcpdump/print-domain.c \
tcpdump/print-dtp.c \
tcpdump/print-dvmrp.c \
tcpdump/print-eap.c \
tcpdump/print-egp.c \
tcpdump/print-eigrp.c \
tcpdump/print-enc.c \
tcpdump/print-esp.c \
tcpdump/print-ether.c \
tcpdump/print-fddi.c \
tcpdump/print-forces.c \
tcpdump/print-fr.c \
tcpdump/print-frag6.c \
tcpdump/print-ftp.c \
tcpdump/print-geneve.c \
tcpdump/print-geonet.c \
tcpdump/print-gre.c \
tcpdump/print-hncp.c \
tcpdump/print-hsrp.c \
tcpdump/print-http.c \
tcpdump/print-icmp.c \
tcpdump/print-icmp6.c \
tcpdump/print-igmp.c \
tcpdump/print-igrp.c \
tcpdump/print-ip.c \
tcpdump/print-ip6.c \
tcpdump/print-ip6opts.c \
tcpdump/print-ipcomp.c \
tcpdump/print-ipfc.c \
tcpdump/print-ipnet.c \
tcpdump/print-ipx.c \
tcpdump/print-isakmp.c \
tcpdump/print-isoclns.c \
tcpdump/print-juniper.c \
tcpdump/print-krb.c \
tcpdump/print-l2tp.c \
tcpdump/print-lane.c \
tcpdump/print-ldp.c \
tcpdump/print-lisp.c \
tcpdump/print-llc.c \
tcpdump/print-lldp.c \
tcpdump/print-lmp.c \
tcpdump/print-loopback.c \
tcpdump/print-lspping.c \
tcpdump/print-lwapp.c \
tcpdump/print-lwres.c \
tcpdump/print-m3ua.c \
tcpdump/print-medsa.c \
tcpdump/print-mobile.c \
tcpdump/print-mobility.c \
tcpdump/print-mpcp.c \
tcpdump/print-mpls.c \
tcpdump/print-mptcp.c \
tcpdump/print-msdp.c \
tcpdump/print-msnlb.c \
tcpdump/print-nflog.c \
tcpdump/print-nfs.c \
tcpdump/print-nsh.c \
tcpdump/print-ntp.c \
tcpdump/print-null.c \
tcpdump/print-olsr.c \
tcpdump/print-openflow-1.0.c \
tcpdump/print-openflow.c \
tcpdump/print-ospf.c \
tcpdump/print-ospf6.c \
tcpdump/print-otv.c \
tcpdump/print-pgm.c \
tcpdump/print-pim.c \
tcpdump/print-pktap.c \
tcpdump/print-ppi.c \
tcpdump/print-ppp.c \
tcpdump/print-pppoe.c \
tcpdump/print-pptp.c \
tcpdump/print-radius.c \
tcpdump/print-raw.c \
tcpdump/print-resp.c \
tcpdump/print-rip.c \
tcpdump/print-ripng.c \
tcpdump/print-rpki-rtr.c \
tcpdump/print-rrcp.c \
tcpdump/print-rsvp.c \
tcpdump/print-rt6.c \
tcpdump/print-rtsp.c \
tcpdump/print-rx.c \
tcpdump/print-sctp.c \
tcpdump/print-sflow.c \
tcpdump/print-sip.c \
tcpdump/print-sl.c \
tcpdump/print-sll.c \
tcpdump/print-slow.c \
tcpdump/print-smb.c \
tcpdump/print-smtp.c \
tcpdump/print-snmp.c \
tcpdump/print-stp.c \
tcpdump/print-sunatm.c \
tcpdump/print-sunrpc.c \
tcpdump/print-symantec.c \
tcpdump/print-syslog.c \
tcpdump/print-tcp.c \
tcpdump/print-telnet.c \
tcpdump/print-tftp.c \
tcpdump/print-timed.c \
tcpdump/print-tipc.c \
tcpdump/print-token.c \
tcpdump/print-udld.c \
tcpdump/print-udp.c \
tcpdump/print-usb.c \
tcpdump/print-vjc.c \
tcpdump/print-vqp.c \
tcpdump/print-vrrp.c \
tcpdump/print-vtp.c \
tcpdump/print-vxlan-gpe.c \
tcpdump/print-vxlan.c \
tcpdump/print-wb.c \
tcpdump/print-zephyr.c \
tcpdump/print-zeromq.c \
tcpdump/print.c \
tcpdump/setsignal.c \
tcpdump/signature.c \
tcpdump/smbutil.c \
tcpdump/strtoaddr.c \
tcpdump/tcpdump.c \
tcpdump/util-print.c \
tcpdump/util.c \
tcpdump/version.c

#*********************************************************************************************************
# Header file search path (eg. LOCAL_INC_PATH := -I"Your header files search path")
#*********************************************************************************************************
LOCAL_INC_PATH :=  \
-I"./tcpdump" \
-I"./src" \
-I"$(SYLIXOS_BASE_PATH)/openssl/openssl/include" \
-I"$(SYLIXOS_BASE_PATH)/libpcap/libpcap"

#*********************************************************************************************************
# Pre-defined macro (eg. -DYOUR_MARCO=1)
#*********************************************************************************************************
LOCAL_DSYMBOL :=  \
-DHAVE_NET_PFVAR_H=1 \
-DHAVE_CONFIG_H=1 \
-D_U_=

#*********************************************************************************************************
# Compiler flags
#*********************************************************************************************************
LOCAL_CFLAGS := -ffloat-store
LOCAL_CXXFLAGS := 

#*********************************************************************************************************
# Depend library (eg. LOCAL_DEPEND_LIB := -la LOCAL_DEPEND_LIB_PATH := -L"Your library search path")
#*********************************************************************************************************
LOCAL_DEPEND_LIB :=  \
-lpcap \
-lssl

LOCAL_DEPEND_LIB_PATH :=  \
-L"$(SYLIXOS_BASE_PATH)/libpcap/$(OUTDIR)" \
-L"$(SYLIXOS_BASE_PATH)/openssl/$(OUTDIR)"

#*********************************************************************************************************
# C++ config
#*********************************************************************************************************
LOCAL_USE_CXX        := no
LOCAL_USE_CXX_EXCEPT := no

#*********************************************************************************************************
# Code coverage config
#*********************************************************************************************************
LOCAL_USE_GCOV := no

#*********************************************************************************************************
# OpenMP config
#*********************************************************************************************************
LOCAL_USE_OMP := no

#*********************************************************************************************************
# User link command
#*********************************************************************************************************
LOCAL_PRE_LINK_CMD := 
LOCAL_POST_LINK_CMD := 
LOCAL_PRE_STRIP_CMD := 
LOCAL_POST_STRIP_CMD := 

include $(APPLICATION_MK)

#*********************************************************************************************************
# End
#*********************************************************************************************************

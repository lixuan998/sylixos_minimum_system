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
# 文   件   名: libVxWorks.mk
#
# 创   建   人: RealEvo-IDE
#
# 文件创建日期: 2016 年 10 月 08 日
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
LOCAL_TARGET_NAME := libVxWorks.so

#*********************************************************************************************************
# Source list
#*********************************************************************************************************
LOCAL_SRCS := \
target/src/avlLib.c \
target/src/avlUint64Lib.c \
target/src/avlUintLib.c \
target/src/bLib.c \
target/src/dllLib.c \
target/src/errnoLib.c \
target/src/eventLib.c \
target/src/hostLib.c \
target/src/ifLib.c \
target/src/inetLib.c \
target/src/intLib.c \
target/src/kernelLib.c \
target/src/loginLib.c \
target/src/logLib.c \
target/src/lstLib.c \
target/src/memLib.c \
target/src/msgQLib.c \
target/src/poolLib.c \
target/src/rtpLib.c \
target/src/semLib.c \
target/src/spinLockLib.c \
target/src/taskLib.c \
target/src/tickLib.c \
target/src/usrLib.c \
target/src/vxAtomicLib.c \
target/src/vxCpuLib.c \
target/src/vxWorks.c \
target/src/wdLib.c

#*********************************************************************************************************
# Header file search path (eg. LOCAL_INC_PATH := -I"Your hearder files search path")
#*********************************************************************************************************
LOCAL_INC_PATH := 
LOCAL_INC_PATH += -I"./target/h"

#*********************************************************************************************************
# Pre-defined macro (eg. -DYOUR_MARCO=1)
#*********************************************************************************************************
LOCAL_DSYMBOL := 

#*********************************************************************************************************
# Depend library (eg. LOCAL_DEPEND_LIB := -la LOCAL_DEPEND_LIB_PATH := -L"Your library search path")
#*********************************************************************************************************
LOCAL_DEPEND_LIB      := 
LOCAL_DEPEND_LIB_PATH := 

#*********************************************************************************************************
# C++ config
#*********************************************************************************************************
LOCAL_USE_CXX        := no
LOCAL_USE_CXX_EXCEPT := no

#*********************************************************************************************************
# Code coverage config
#*********************************************************************************************************
LOCAL_USE_GCOV := no

include $(LIBRARY_MK)

#*********************************************************************************************************
# End
#*********************************************************************************************************

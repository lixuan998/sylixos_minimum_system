#*********************************************************************************************************
#
#                                    �й������Դ��֯
#
#                                   Ƕ��ʽʵʱ����ϵͳ
#
#                                SylixOS(TM)  LW : long wing
#
#                               Copyright All Rights Reserved
#
#--------------�ļ���Ϣ--------------------------------------------------------------------------------
#
# ��   ��   ��: socket.mk
#
# ��   ��   ��: RealEvo-IDE
#
# �ļ���������: 2016 �� 10 �� 08 ��
#
# ��        ��: ���ļ��� RealEvo-IDE ���ɣ��������� Makefile ���ܣ������ֶ��޸�
#*********************************************************************************************************

#*********************************************************************************************************
# Clear setting
#*********************************************************************************************************
include $(CLEAR_VARS_MK)

#*********************************************************************************************************
# Target
#*********************************************************************************************************
LOCAL_TARGET_NAME := socket.so

#*********************************************************************************************************
# Source list
#*********************************************************************************************************
LOCAL_SRCS := \
luasocket/src/luasocket.c \
luasocket/src/timeout.c \
luasocket/src/buffer.c \
luasocket/src/io.c \
luasocket/src/auxiliar.c \
luasocket/src/compat.c \
luasocket/src/options.c \
luasocket/src/inet.c \
luasocket/src/tcp.c \
luasocket/src/udp.c \
luasocket/src/except.c \
luasocket/src/select.c \
luasocket/src/usocket.c 

#*********************************************************************************************************
# Header file search path (eg. LOCAL_INC_PATH := -I"Your hearder files search path")
#*********************************************************************************************************
LOCAL_INC_PATH := 
LOCAL_INC_PATH += $(LUA_INC_PATH)

#*********************************************************************************************************
# Pre-defined macro (eg. -DYOUR_MARCO=1)
#*********************************************************************************************************
LOCAL_DSYMBOL := -DLUA_COMPAT_ALL
LOCAL_DSYMBOL += -DLUA_COMPAT_5_1
LOCAL_DSYMBOL += -DLUA_COMPAT_5_2

LOCAL_DSYMBOL += -DLUASOCKET_DEBUG
LOCAL_DSYMBOL += -DUNIX_HAS_SUN_LEN

LOCAL_DSYMBOL += -DLUASOCKET_API='__attribute__((visibility("default")))'
LOCAL_DSYMBOL += -DUNIX_API='__attribute__((visibility("default")))'
LOCAL_DSYMBOL += -DMIME_API='__attribute__((visibility("default")))'

#*********************************************************************************************************
# Compiler flags
#*********************************************************************************************************
LOCAL_CFLAGS := -fvisibility=hidden

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

#*********************************************************************************************************
# User link command
#*********************************************************************************************************
LOCAL_POST_LINK_CMD  := mkdir $(OUTDIR)/socket; \
						cp luasocket/src/ftp.lua $(OUTDIR)/socket; \
						cp luasocket/src/headers.lua $(OUTDIR)/socket; \
						cp luasocket/src/http.lua $(OUTDIR)/socket; \
						cp luasocket/src/smtp.lua $(OUTDIR)/socket; \
						cp luasocket/src/url.lua $(OUTDIR)/socket; \
						cp luasocket/src/tp.lua $(OUTDIR)/socket; \
						cp luasocket/src/ltn12.lua $(OUTDIR); \
						cp luasocket/src/socket.lua $(OUTDIR); \
						cp luasocket/src/mime.lua $(OUTDIR); 
LOCAL_POST_STRIP_CMD := mkdir $(OUTDIR)/strip/socket; \
						cp luasocket/src/ftp.lua $(OUTDIR)/strip/socket; \
						cp luasocket/src/headers.lua $(OUTDIR)/strip/socket; \
						cp luasocket/src/http.lua $(OUTDIR)/strip/socket; \
						cp luasocket/src/smtp.lua $(OUTDIR)/strip/socket; \
						cp luasocket/src/url.lua $(OUTDIR)/strip/socket; \
						cp luasocket/src/tp.lua $(OUTDIR)/strip/socket; \
						cp luasocket/src/ltn12.lua $(OUTDIR)/strip; \
						cp luasocket/src/socket.lua $(OUTDIR)/strip; \
						cp luasocket/src/mime.lua $(OUTDIR)/strip; 

include $(LIBRARY_MK)

#*********************************************************************************************************
# End
#*********************************************************************************************************

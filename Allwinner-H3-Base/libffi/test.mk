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
# 文   件   名: test.mk
#
# 创   建   人: RealEvo-IDE
#
# 文件创建日期: 2018 年 03 月 01 日
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
LOCAL_TARGET_NAME := test

#*********************************************************************************************************
# Source list
#*********************************************************************************************************
LOCAL_SRCS := \
./libffi/testsuite/libffi.call/align_mixed.c \
./libffi/testsuite/libffi.call/align_stdcall.c \
./libffi/testsuite/libffi.call/closure_fn0.c \
./libffi/testsuite/libffi.call/closure_fn1.c \
./libffi/testsuite/libffi.call/closure_fn2.c \
./libffi/testsuite/libffi.call/closure_fn3.c \
./libffi/testsuite/libffi.call/closure_fn4.c \
./libffi/testsuite/libffi.call/closure_fn5.c \
./libffi/testsuite/libffi.call/closure_fn6.c \
./libffi/testsuite/libffi.call/closure_loc_fn0.c \
./libffi/testsuite/libffi.call/closure_simple.c \
./libffi/testsuite/libffi.call/cls_12byte.c \
./libffi/testsuite/libffi.call/cls_16byte.c \
./libffi/testsuite/libffi.call/cls_18byte.c \
./libffi/testsuite/libffi.call/cls_19byte.c \
./libffi/testsuite/libffi.call/cls_1_1byte.c \
./libffi/testsuite/libffi.call/cls_20byte.c \
./libffi/testsuite/libffi.call/cls_20byte1.c \
./libffi/testsuite/libffi.call/cls_24byte.c \
./libffi/testsuite/libffi.call/cls_2byte.c \
./libffi/testsuite/libffi.call/cls_3byte1.c \
./libffi/testsuite/libffi.call/cls_3byte2.c \
./libffi/testsuite/libffi.call/cls_3float.c \
./libffi/testsuite/libffi.call/cls_3_1byte.c \
./libffi/testsuite/libffi.call/cls_4byte.c \
./libffi/testsuite/libffi.call/cls_4_1byte.c \
./libffi/testsuite/libffi.call/cls_5byte.c \
./libffi/testsuite/libffi.call/cls_5_1_byte.c \
./libffi/testsuite/libffi.call/cls_64byte.c \
./libffi/testsuite/libffi.call/cls_6byte.c \
./libffi/testsuite/libffi.call/cls_6_1_byte.c \
./libffi/testsuite/libffi.call/cls_7byte.c \
./libffi/testsuite/libffi.call/cls_7_1_byte.c \
./libffi/testsuite/libffi.call/cls_8byte.c \
./libffi/testsuite/libffi.call/cls_9byte1.c \
./libffi/testsuite/libffi.call/cls_9byte2.c \
./libffi/testsuite/libffi.call/cls_align_double.c \
./libffi/testsuite/libffi.call/cls_align_float.c \
./libffi/testsuite/libffi.call/cls_align_longdouble.c \
./libffi/testsuite/libffi.call/cls_align_longdouble_split.c \
./libffi/testsuite/libffi.call/cls_align_longdouble_split2.c \
./libffi/testsuite/libffi.call/cls_align_pointer.c \
./libffi/testsuite/libffi.call/cls_align_sint16.c \
./libffi/testsuite/libffi.call/cls_align_sint32.c \
./libffi/testsuite/libffi.call/cls_align_sint64.c \
./libffi/testsuite/libffi.call/cls_align_uint16.c \
./libffi/testsuite/libffi.call/cls_align_uint32.c \
./libffi/testsuite/libffi.call/cls_align_uint64.c \
./libffi/testsuite/libffi.call/cls_dbls_struct.c \
./libffi/testsuite/libffi.call/cls_double.c \
./libffi/testsuite/libffi.call/cls_double_va.c \
./libffi/testsuite/libffi.call/cls_float.c \
./libffi/testsuite/libffi.call/cls_longdouble.c \
./libffi/testsuite/libffi.call/cls_longdouble_va.c \
./libffi/testsuite/libffi.call/cls_many_mixed_args.c \
./libffi/testsuite/libffi.call/cls_many_mixed_float_double.c \
./libffi/testsuite/libffi.call/cls_multi_schar.c \
./libffi/testsuite/libffi.call/cls_multi_sshort.c \
./libffi/testsuite/libffi.call/cls_multi_sshortchar.c \
./libffi/testsuite/libffi.call/cls_multi_uchar.c \
./libffi/testsuite/libffi.call/cls_multi_ushort.c \
./libffi/testsuite/libffi.call/cls_multi_ushortchar.c \
./libffi/testsuite/libffi.call/cls_pointer.c \
./libffi/testsuite/libffi.call/cls_pointer_stack.c \
./libffi/testsuite/libffi.call/cls_schar.c \
./libffi/testsuite/libffi.call/cls_sint.c \
./libffi/testsuite/libffi.call/cls_sshort.c \
./libffi/testsuite/libffi.call/cls_struct_va1.c \
./libffi/testsuite/libffi.call/cls_uchar.c \
./libffi/testsuite/libffi.call/cls_uchar_va.c \
./libffi/testsuite/libffi.call/cls_uint.c \
./libffi/testsuite/libffi.call/cls_uint_va.c \
./libffi/testsuite/libffi.call/cls_ulonglong.c \
./libffi/testsuite/libffi.call/cls_ulong_va.c \
./libffi/testsuite/libffi.call/cls_ushort.c \
./libffi/testsuite/libffi.call/cls_ushort_va.c \
./libffi/testsuite/libffi.call/err_bad_abi.c \
./libffi/testsuite/libffi.call/err_bad_typedef.c \
./libffi/testsuite/libffi.call/float.c \
./libffi/testsuite/libffi.call/float1.c \
./libffi/testsuite/libffi.call/float2.c \
./libffi/testsuite/libffi.call/float3.c \
./libffi/testsuite/libffi.call/float4.c \
./libffi/testsuite/libffi.call/float_va.c \
./libffi/testsuite/libffi.call/huge_struct.c \
./libffi/testsuite/libffi.call/many.c \
./libffi/testsuite/libffi.call/many2.c \
./libffi/testsuite/libffi.call/many_double.c \
./libffi/testsuite/libffi.call/many_mixed.c \
./libffi/testsuite/libffi.call/negint.c \
./libffi/testsuite/libffi.call/nested_struct.c \
./libffi/testsuite/libffi.call/nested_struct1.c \
./libffi/testsuite/libffi.call/nested_struct10.c \
./libffi/testsuite/libffi.call/nested_struct11.c \
./libffi/testsuite/libffi.call/nested_struct2.c \
./libffi/testsuite/libffi.call/nested_struct3.c \
./libffi/testsuite/libffi.call/nested_struct4.c \
./libffi/testsuite/libffi.call/nested_struct5.c \
./libffi/testsuite/libffi.call/nested_struct6.c \
./libffi/testsuite/libffi.call/nested_struct7.c \
./libffi/testsuite/libffi.call/nested_struct8.c \
./libffi/testsuite/libffi.call/nested_struct9.c \
./libffi/testsuite/libffi.call/offsets.c \
./libffi/testsuite/libffi.call/pr1172638.c \
./libffi/testsuite/libffi.call/problem1.c \
./libffi/testsuite/libffi.call/promotion.c \
./libffi/testsuite/libffi.call/pyobjc-tc.c \
./libffi/testsuite/libffi.call/return_dbl.c \
./libffi/testsuite/libffi.call/return_dbl1.c \
./libffi/testsuite/libffi.call/return_dbl2.c \
./libffi/testsuite/libffi.call/return_fl.c \
./libffi/testsuite/libffi.call/return_fl1.c \
./libffi/testsuite/libffi.call/return_fl2.c \
./libffi/testsuite/libffi.call/return_fl3.c \
./libffi/testsuite/libffi.call/return_ldl.c \
./libffi/testsuite/libffi.call/return_ll.c \
./libffi/testsuite/libffi.call/return_ll1.c \
./libffi/testsuite/libffi.call/return_sc.c \
./libffi/testsuite/libffi.call/return_sl.c \
./libffi/testsuite/libffi.call/return_uc.c \
./libffi/testsuite/libffi.call/return_ul.c \
./libffi/testsuite/libffi.call/stret_large.c \
./libffi/testsuite/libffi.call/stret_large2.c \
./libffi/testsuite/libffi.call/stret_medium.c \
./libffi/testsuite/libffi.call/stret_medium2.c \
./libffi/testsuite/libffi.call/strlen.c \
./libffi/testsuite/libffi.call/strlen2.c \
./libffi/testsuite/libffi.call/strlen3.c \
./libffi/testsuite/libffi.call/strlen4.c \
./libffi/testsuite/libffi.call/struct1.c \
./libffi/testsuite/libffi.call/struct10.c \
./libffi/testsuite/libffi.call/struct2.c \
./libffi/testsuite/libffi.call/struct3.c \
./libffi/testsuite/libffi.call/struct4.c \
./libffi/testsuite/libffi.call/struct5.c \
./libffi/testsuite/libffi.call/struct6.c \
./libffi/testsuite/libffi.call/struct7.c \
./libffi/testsuite/libffi.call/struct8.c \
./libffi/testsuite/libffi.call/struct9.c \
./libffi/testsuite/libffi.call/testclosure.c \
./libffi/testsuite/libffi.call/uninitialized.c \
./libffi/testsuite/libffi.call/va_1.c \
./libffi/testsuite/libffi.call/va_struct1.c \
./libffi/testsuite/libffi.call/va_struct2.c \
./libffi/testsuite/libffi.call/va_struct3.c \
./libffi/testsuite/run_test_suite.c

#*********************************************************************************************************
# Header file search path (eg. LOCAL_INC_PATH := -I"Your header files search path")
#*********************************************************************************************************
LOCAL_INC_PATH := \
-I"./libffi/include" \
-I"./libffi/src"

#*********************************************************************************************************
# Pre-defined macro (eg. -DYOUR_MARCO=1)
#*********************************************************************************************************
LOCAL_DSYMBOL := -DHAVE_CONFIG_H

#*********************************************************************************************************
# Compiler flags
#*********************************************************************************************************
LOCAL_CFLAGS   := 
LOCAL_CXXFLAGS := 

#*********************************************************************************************************
# Depend library (eg. LOCAL_DEPEND_LIB := -la LOCAL_DEPEND_LIB_PATH := -L"Your library search path")
#*********************************************************************************************************
LOCAL_DEPEND_LIB      := -lffi
LOCAL_DEPEND_LIB_PATH := -L$(OUTDIR)

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
LOCAL_PRE_LINK_CMD   := 
LOCAL_POST_LINK_CMD  := 
LOCAL_PRE_STRIP_CMD  := 
LOCAL_POST_STRIP_CMD := 

#*********************************************************************************************************
# Depend targets
#*********************************************************************************************************
LOCAL_DEPEND_TARGET := ./$(OUTDIR)/libffi.so

include $(UNIT_TEST_MK)

#*********************************************************************************************************
# End
#*********************************************************************************************************

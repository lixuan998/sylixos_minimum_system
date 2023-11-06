/*********************************************************************************************************
**
**                                    �й�������Դ��֯
**
**                                   Ƕ��ʽʵʱ����ϵͳ
**
**                                SylixOS(TM)  LW : long wing
**
**                               Copyright All Rights Reserved
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**
** ��   ��   ��: cppRtBegin.cpp
**
** ��   ��   ��: Han.Hui (����)
**
** �ļ���������: 2011 �� 03 �� 08 ��
**
** ��        ��: ����ϵͳƽ̨ C++ run time ȫ�ֶ��󹹽�������������. 
*********************************************************************************************************/
#include "SylixOS.h"
/*********************************************************************************************************
  C ��������
*********************************************************************************************************/
extern "C" {
/*********************************************************************************************************
  ˵��:
  
  ������(gcc)���� C++ ����ʱ, �������ȫ�ֶ���, ��ôȫ�ֶ���Ĺ�������ָ�����ڿ�ִ�� elf �ļ��� .ctors 
  ����(section), ������������ڿ�ִ�� elf �ļ��� .dtors ����, һ���׼ gcc ��������ĸ�����:
   
  __CTOR_LIST__
  __CTOR_END__
  __DTOR_LIST__
  __DTOR_END__
   
  ���� __CTOR_LIST__ ��ʾ���е�ȫ�ֶ����캯��ָ��������׵�ַ, ��ʼָ��Ϊ 0xFFFFFFFF, ֮���ÿһ��Ϊ
  һ�����캯�������, ֱ�� __CTOR_END__ Ϊֹ, __CTOR_END__ ָ��ĺ���ָ��Ϊ 0x00000000
  
  ���� __DTOR_LIST__ ��ʾ���е�ȫ�ֶ�����������ָ��������׵�ַ, ��ʼָ��Ϊ 0xFFFFFFFF, ֮���ÿһ��Ϊ
  һ���������������, ֱ�� __DTOR_END__ Ϊֹ, __DTOR_END__ ָ��ĺ���ָ��Ϊ 0x00000000
  
  һ�´����ʵ������� 4 ���������ƵĶ���. ����ϵͳ�Ϳ����������û�����֮ǰ, ��ʼ�� C++ ����, ����ȫ��
  ����Ĺ��캯��, ��ϵͳ reboot ʱ, ����ϵͳ����������.
  
  ���Ҫ����Щ���Ŵ��ڶ�Ӧ .ctors �� .dtors ����ָ����λ��, ����Ҫ�������ļ�����һ�´���:
  
  .ctors :
  {
      KEEP (*cppRtBegin*.o(.ctors))
      KEEP (*(.preinit_array))
      KEEP (*(.init_array))
      KEEP (*(SORT(.ctors.*)))
      KEEP (*(.ctors))
      KEEP (*cppRtEnd*.o(.ctors))
  }
  
  .dtors :
  {
      KEEP (*cppRtBegin*.o(.dtors))
      KEEP (*(.fini_array))
      KEEP (*(SORT(.dtors.*)))
      KEEP (*(.dtors))
      KEEP (*cppRtEnd*.o(.dtors))
  }
  
  �������ӽű�, ����Ҫ�ķ��Ŷ��嵽�� .ctors .dtors ������Ӧ��λ�� (�ֱ��嵽����������������β)
  (���� .init_array �� .fini_array �ֱ��ǹ������������о�̬�洢ʱ�޵Ķ���)
  
  ע��:
  
  ���ڲ���ϵͳ���ڵ����û�֮ǰ, ��������ȫ�ֶ����캯��, ��ʱ��û�н�������񻷾�, ���Զ���Ĺ��캯��һ��
  Ҫ�㹻�ļ�, һ�����������ʼ����������Ժ�һЩ�������ݽṹ, ����Ĳ������������м���ר�ŵĳ�ʼ������
  ��ʵ��.
*********************************************************************************************************/
/*********************************************************************************************************
  C++ ȫ�ֶ��󹹽������������� (Ϊ�˺�һЩ��������������ͻ, ����ʹ�� SylixOS �Դ��ķ���)
*********************************************************************************************************/
#ifndef LW_CFG_CPU_ARCH_C6X
#ifdef __GNUC__
static VOIDFUNCPTR __LW_CTOR_LIST__[1] __attribute__((section(".ctors"))) = { (VOIDFUNCPTR)-1 };
static VOIDFUNCPTR __LW_DTOR_LIST__[1] __attribute__((section(".dtors"))) = { (VOIDFUNCPTR)-1 };
#endif                                                                  /*  __GNUC__                    */
#endif                                                                  /*  !LW_CFG_CPU_ARCH_C6X        */
/*********************************************************************************************************
** ��������: __cppRtDoCtors
** ��������: C++ ����ȫ�ֶ����캯��
** �䡡��  : NONE
** �䡡��  : NONE
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
VOID  __cppRtDoCtors (VOID)
{
#ifndef LW_CFG_CPU_ARCH_C6X
#ifdef __GNUC__
    volatile VOIDFUNCPTR    *ppfunc;
    
    for (ppfunc = __LW_CTOR_LIST__ + 1;  *ppfunc != LW_NULL;  ppfunc++) {
        if ((*ppfunc) != (VOIDFUNCPTR)-1) {                             /*  ���� C �⻹��һ����ʼ����   */
            (*ppfunc)();
        }
    }
#endif                                                                  /*  __GNUC__                    */
#else
#define PINIT_BASE      __TI_INITARRAY_Base
#define PINIT_LIMIT     __TI_INITARRAY_Limit

    extern __attribute__((weak)) __far volatile VOIDFUNCPTR const PINIT_BASE[];
    extern __attribute__((weak)) __far volatile VOIDFUNCPTR const PINIT_LIMIT[];

    if (PINIT_BASE != PINIT_LIMIT) {
        ULONG  i = 0;
        while (&(PINIT_BASE[i]) != PINIT_LIMIT) {
            PINIT_BASE[i++]();
        }
    }
#endif                                                                  /*  !LW_CFG_CPU_ARCH_C6X        */
}
/*********************************************************************************************************
** ��������: __cppRtDoDtors
** ��������: C++ ����ȫ�ֶ�����������
** �䡡��  : NONE
** �䡡��  : NONE
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
VOID  __cppRtDoDtors (VOID)
{
#ifndef LW_CFG_CPU_ARCH_C6X
#ifdef __GNUC__
    volatile VOIDFUNCPTR    *ppfunc;
    
    for (ppfunc = __LW_DTOR_LIST__ + 1;  *ppfunc != LW_NULL;  ppfunc++);/*  ������Ҫ�������һ��        */
    ppfunc--;
    
    while (ppfunc > __LW_DTOR_LIST__) {
        if ((*ppfunc) != (VOIDFUNCPTR)-1) {                             /*  ���� C �⻹��һ����ʼ����   */
            (*ppfunc)();
        }
        ppfunc--;
    }
#endif                                                                  /*  __GNUC__                    */
#endif                                                                  /*  !LW_CFG_CPU_ARCH_C6X        */
}
}
/*********************************************************************************************************
  END
*********************************************************************************************************/
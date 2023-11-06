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
** ��   ��   ��: ThreadUnlock.c
**
** ��   ��   ��: Han.Hui (����)
**
** �ļ���������: 2006 �� 12 �� 18 ��
**
** ��        ��: ����򿪵�����.

** BUG
2008.01.20  ��ȫ������Ϊ�ֲ���.
2008.03.30  ��ǰ������������ CPU ʱ, ��: ��ǰΪ�Ǿ���״̬, Ӧ�����Ե���.
2008.04.06  �����߳������ĵķ���ֵ�Ĵ���.
2013.07.18  ���� SMP ϵͳ.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "../SylixOS/kernel/include/k_kernel.h"
/*********************************************************************************************************
** ��������: API_ThreadUnlock
** ��������: ����򿪵�����.
** �䡡��  : NONE
** �䡡��  : �˷���ֵ����.
** ȫ�ֱ���: 
** ����ģ��: 
** ע  ��  : �� API ���Ե�ǰ CPU ��Ч, ������Ӱ������ CPU ����.

                                           API ����
*********************************************************************************************************/
LW_API
INT  API_ThreadUnlock (VOID)
{
    if (LW_CPU_GET_CUR_NESTING()) {                                     /*  �������ж��е���            */
        _ErrorHandle(ERROR_KERNEL_IN_ISR);
        return  (PX_ERROR);
    }

    LW_THREAD_UNLOCK();
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
  END
*********************************************************************************************************/
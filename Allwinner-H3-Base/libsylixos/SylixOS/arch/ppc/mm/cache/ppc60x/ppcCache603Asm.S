;/*********************************************************************************************************
;**
;**                                    �й�������Դ��֯
;**
;**                                   Ƕ��ʽʵʱ����ϵͳ
;**
;**                                       SylixOS(TM)
;**
;**                               Copyright  All Rights Reserved
;**
;**--------------�ļ���Ϣ--------------------------------------------------------------------------------
;**
;** ��   ��   ��: ppcCache603Asm.S
;**
;** ��   ��   ��: Jiao.JinXing (������)
;**
;** �ļ���������: 2016 �� 03 �� 30 ��
;**
;** ��        ��: PowerPC 603 ��ϵ���� CACHE ����.
;*********************************************************************************************************/

#define __SYLIXOS_PPC603__                  0x603
#define __SYLIXOS_PPCEC603__                0xEC603
#define __SYLIXOS_PPC604__                  0x604

#define __SYLIXOS_PPC_CPU__                 __SYLIXOS_PPC603__

/*********************************************************************************************************
  ���¶���ӿ���
*********************************************************************************************************/

#define ppc60xICacheEnable                  ppc603ICacheEnable
#define ppc60xDCacheEnable                  ppc603DCacheEnable
#define ppc60xICacheDisable                 ppc603ICacheDisable
#define ppc60xDCacheDisable                 ppc603DCacheDisable
#define ppc60xICacheInvalidate              ppc603ICacheInvalidate
#define ppc60xDCacheInvalidate              ppc603DCacheInvalidate
#define ppc60xDCacheClear                   ppc603DCacheClear
#define ppc60xDCacheFlush                   ppc603DCacheFlush
#define ppc60xICacheInvalidateAll           ppc603ICacheInvalidateAll
#define ppc60xDCacheInvalidateAll           ppc603DCacheInvalidateAll
#define ppc60xDCacheFlushAll                ppc603DCacheFlushAll
#define ppc60xDCacheClearAll                ppc603DCacheClearAll
#define ppc60xBranchPredictionDisable       ppc603BranchPredictionDisable
#define ppc60xBranchPredictionEnable        ppc603BranchPredictionEnable
#define ppc60xBranchPredictorInvalidate     ppc603BranchPredictorInvalidate
#define ppc60xTextUpdate                    ppc603TextUpdate

/*********************************************************************************************************
  ���� 60X CACHE ����
*********************************************************************************************************/

#include "ppcCache60xAsm.S"

;/*********************************************************************************************************
;  END
;*********************************************************************************************************/
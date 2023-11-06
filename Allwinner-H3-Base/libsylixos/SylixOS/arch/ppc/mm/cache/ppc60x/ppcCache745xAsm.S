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
;** ��   ��   ��: ppcCache745xAsm.S
;**
;** ��   ��   ��: Jiao.JinXing (������)
;**
;** �ļ���������: 2016 �� 03 �� 30 ��
;**
;** ��        ��: PowerPC MPC745X ��ϵ���� CACHE ����.
;*********************************************************************************************************/

#define __SYLIXOS_PPC603__                  0x603
#define __SYLIXOS_PPCEC603__                0xEC603
#define __SYLIXOS_PPC604__                  0x604

#define __SYLIXOS_PPC_CPU__                 __SYLIXOS_PPC604__           /*  MPC745X �� 604 �ı���       */

#define __SYLIXOS_PPC745X__
#define __SYLIXOS_PPC_HAVE_ALTIVEC          1

/*********************************************************************************************************
  ���¶���ӿ���
*********************************************************************************************************/

#define ppc60xICacheEnable                  ppc745xICacheEnable
#define ppc60xDCacheEnable                  ppc745xDCacheEnable
#define ppc60xICacheDisable                 ppc745xICacheDisable
#define ppc60xDCacheDisable                 ppc745xDCacheDisable
#define ppc60xICacheInvalidate              ppc745xICacheInvalidate
#define ppc60xDCacheInvalidate              ppc745xDCacheInvalidate
#define ppc60xDCacheClear                   ppc745xDCacheClear
#define ppc60xDCacheFlush                   ppc745xDCacheFlush
#define ppc60xICacheInvalidateAll           ppc745xICacheInvalidateAll
#define ppc60xDCacheInvalidateAll           ppc745xDCacheInvalidateAll
#define ppc60xDCacheFlushAll                ppc745xDCacheFlushAll
#define ppc60xDCacheClearAll                ppc745xDCacheClearAll
#define ppc60xBranchPredictionDisable       ppc745xBranchPredictionDisable
#define ppc60xBranchPredictionEnable        ppc745xBranchPredictionEnable
#define ppc60xBranchPredictorInvalidate     ppc745xBranchPredictorInvalidate
#define ppc60xTextUpdate                    ppc745xTextUpdate

/*********************************************************************************************************
  ���� 60X CACHE ����
*********************************************************************************************************/

#include "ppcCache60xAsm.S"

;/*********************************************************************************************************
;  END
;*********************************************************************************************************/
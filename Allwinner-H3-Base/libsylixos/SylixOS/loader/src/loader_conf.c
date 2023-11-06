/*********************************************************************************************************
**
**                                    �й������Դ��֯
**
**                                   Ƕ��ʽʵʱ����ϵͳ
**
**                                SylixOS(TM)  LW : long wing
**
**                               Copyright All Rights Reserved
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**
** ��   ��   ��: loader_conf.c
**
** ��   ��   ��: Han.Hui (����)
**
** �ļ���������: 2022 �� 09 �� 28 ��
**
** ��        ��: ����װ�������ļ�����
*********************************************************************************************************/
#define  __SYLIXOS_STDIO
#define  __SYLIXOS_KERNEL
#include "SylixOS.h"
/*********************************************************************************************************
  �ü�֧��
*********************************************************************************************************/
#if LW_CFG_MODULELOADER_EN > 0
#include "../include/loader_lib.h"
#include "../include/loader_conf.h"
/*********************************************************************************************************
  ����г���
*********************************************************************************************************/
#define LW_VP_LOAD_CONF_LINE_MAX_SIZE   1024
/*********************************************************************************************************
  װ��������
*********************************************************************************************************/
typedef struct {
    LW_LIST_LINE            VPLC_lineManage;                            /*  ��������                    */
    PCHAR                   VPLC_pcVpPath;                              /*  ���̾�������·��            */
    CHAR                    VPLC_cLoadPath[1];                          /*  ��������·��                */
} LW_VP_LOAD_CONF;
typedef LW_VP_LOAD_CONF    *PLW_VP_LOAD_CONF;
/*********************************************************************************************************
  װ������������
*********************************************************************************************************/
static PLW_LIST_LINE        _G_plineVpLoad = LW_NULL;
/*********************************************************************************************************
** ��������: __ldConfFree
** ��������: װ��������ɾ��
** �䡡��  : NONE
** �䡡��  : NONE
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
static VOID  __ldConfFree (VOID)
{
    PLW_VP_LOAD_CONF pldconf;

    while (_G_plineVpLoad) {
        pldconf = _LIST_ENTRY(_G_plineVpLoad, LW_VP_LOAD_CONF, VPLC_lineManage);
        _List_Line_Del(&pldconf->VPLC_lineManage, &_G_plineVpLoad);
        __SHEAP_FREE(pldconf->VPLC_pcVpPath);
        __SHEAP_FREE(pldconf);
    }
}
/*********************************************************************************************************
** ��������: __ldConfLoad
** ��������: װ�������ü���
** �䡡��  : NONE
** �䡡��  : NONE
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
static BOOL  __ldConfLoad (VOID)
{
    FILE  *pfile;
    PCHAR  pcConfItem;
    CHAR   cLineBuffer[LW_VP_LOAD_CONF_LINE_MAX_SIZE];
    PCHAR  p;
    PCHAR  pcVpPath;
    PCHAR  pcLoadPath;
    BOOL   bSuccess = LW_TRUE;

    PLW_VP_LOAD_CONF pldconf;

    pfile = fopen(LW_VP_LOAD_CONF_PATH, "r");
    if (pfile == LW_NULL) {
        return  (LW_FALSE);
    }

    cLineBuffer[LW_VP_LOAD_CONF_LINE_MAX_SIZE - 1] = PX_EOS;
    do {
        pcConfItem = fgets(cLineBuffer, LW_VP_LOAD_CONF_LINE_MAX_SIZE - 1, pfile);
        if (pcConfItem) {
            p = cLineBuffer;
            while ((*p == ' ') || (*p == '\t')) {
                p++;
            }
            if (*p != '/') {
                continue;
            }

            pcVpPath = p;

            while ((*p) && (*p != ' ') && (*p != '\t')) {
                p++;
            }
            if (*p == '\0') {
                continue;
            }

            *p = '\0';
            p++;

            while ((*p == ' ') || (*p == '\t')) {
                p++;
            }
            if (*p == '\0') {
                continue;
            }

            pcLoadPath = p;

            while ((*p) && (*p != ' ') && (*p != '\t') && (*p != '\r') && (*p != '\n')) {
                p++;
            }
            *p = '\0';

            pldconf = (PLW_VP_LOAD_CONF)__SHEAP_ALLOC(sizeof(LW_VP_LOAD_CONF) + lib_strlen(pcLoadPath));
            if (!pldconf) {
                bSuccess = LW_FALSE;
                break;
            }

            pldconf->VPLC_pcVpPath = lib_strdup(pcVpPath);
            if (!pldconf->VPLC_pcVpPath){
                bSuccess = LW_FALSE;
                break;
            }
            lib_strcpy(pldconf->VPLC_cLoadPath, pcLoadPath);

            _List_Line_Add_Ahead(&pldconf->VPLC_lineManage, &_G_plineVpLoad);
        }
    } while (pcConfItem);

    if (!bSuccess) {
        __ldConfFree();
    }

    fclose(pfile);

    return  (bSuccess);
}
/*********************************************************************************************************
** ��������: vprocLoadConfRefresh
** ��������: ��ʼ��װ��������
** �䡡��  : NONE
** �䡡��  : �Ƿ�ɹ�
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
BOOL  vprocLoadConfRefresh (VOID)
{
    BOOL  bRet;

    LW_LD_LOCK();
    __ldConfFree();                                                     /*  �ͷ�֮ǰ������              */
    bRet = __ldConfLoad();                                              /*  �����µ�����                */
    LW_LD_UNLOCK();

    return  (bRet);
}
/*********************************************************************************************************
** ��������: vprocLoadConfFind
** ��������: ����װ��������
** �䡡��  : pcFileName      ���̿�ִ���ļ�·��
**           pcPathBuffer    ���û�����
**           stMaxLen        ��������С
** �䡡��  : ģ����
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
INT  vprocLoadConfFind (CPCHAR  pcFileName, PCHAR  pcPathBuffer, size_t  stMaxLen)
{
    PLW_LIST_LINE    plineTmp;
    PLW_VP_LOAD_CONF pldconf;
    size_t           stLen;

    LW_LD_LOCK();

    for (plineTmp  = _G_plineVpLoad;
         plineTmp != LW_NULL;
         plineTmp  = _list_line_get_next(plineTmp)) {

        pldconf = _LIST_ENTRY(plineTmp, LW_VP_LOAD_CONF, VPLC_lineManage);
        if (lib_strcmp(pcFileName, pldconf->VPLC_pcVpPath) == 0) {      /*  ƥ���ļ�ȫ·��              */
            lib_strlcpy(pcPathBuffer, pldconf->VPLC_cLoadPath, stMaxLen);
            LW_LD_UNLOCK();
            return  (ERROR_NONE);

        } else {                                                        /*  ƥ��Ŀ¼                    */
            stLen = lib_strlen(pldconf->VPLC_pcVpPath);
            if (pldconf->VPLC_pcVpPath[stLen - 1] == '/') {
                if (lib_strncmp(pcFileName, pldconf->VPLC_pcVpPath, stLen) == 0) {
                    lib_strlcpy(pcPathBuffer, pldconf->VPLC_cLoadPath, stMaxLen);
                    LW_LD_UNLOCK();
                    return  (ERROR_NONE);
                }
            }
        }
    }

    LW_LD_UNLOCK();

    return  (PX_ERROR);
}

#endif                                                                  /*  LW_CFG_MODULELOADER_EN > 0  */
/*********************************************************************************************************
  END
*********************************************************************************************************/

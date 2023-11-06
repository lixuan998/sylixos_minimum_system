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
** 文   件   名: loader_conf.c
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2022 年 09 月 28 日
**
** 描        述: 进程装载配置文件解析
*********************************************************************************************************/
#define  __SYLIXOS_STDIO
#define  __SYLIXOS_KERNEL
#include "SylixOS.h"
/*********************************************************************************************************
  裁剪支持
*********************************************************************************************************/
#if LW_CFG_MODULELOADER_EN > 0
#include "../include/loader_lib.h"
#include "../include/loader_conf.h"
/*********************************************************************************************************
  最大行长度
*********************************************************************************************************/
#define LW_VP_LOAD_CONF_LINE_MAX_SIZE   1024
/*********************************************************************************************************
  装载器配置
*********************************************************************************************************/
typedef struct {
    LW_LIST_LINE            VPLC_lineManage;                            /*  管理链表                    */
    PCHAR                   VPLC_pcVpPath;                              /*  进程镜像完整路径            */
    CHAR                    VPLC_cLoadPath[1];                          /*  优先搜索路径                */
} LW_VP_LOAD_CONF;
typedef LW_VP_LOAD_CONF    *PLW_VP_LOAD_CONF;
/*********************************************************************************************************
  装载器配置链表
*********************************************************************************************************/
static PLW_LIST_LINE        _G_plineVpLoad = LW_NULL;
/*********************************************************************************************************
** 函数名称: __ldConfFree
** 功能描述: 装载器配置删除
** 输　入  : NONE
** 输　出  : NONE
** 全局变量:
** 调用模块:
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
** 函数名称: __ldConfLoad
** 功能描述: 装载器配置加载
** 输　入  : NONE
** 输　出  : NONE
** 全局变量:
** 调用模块:
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
** 函数名称: vprocLoadConfRefresh
** 功能描述: 初始化装载器配置
** 输　入  : NONE
** 输　出  : 是否成功
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL  vprocLoadConfRefresh (VOID)
{
    BOOL  bRet;

    LW_LD_LOCK();
    __ldConfFree();                                                     /*  释放之前的配置              */
    bRet = __ldConfLoad();                                              /*  加载新的配置                */
    LW_LD_UNLOCK();

    return  (bRet);
}
/*********************************************************************************************************
** 函数名称: vprocLoadConfFind
** 功能描述: 查找装载器配置
** 输　入  : pcFileName      进程可执行文件路径
**           pcPathBuffer    配置缓冲区
**           stMaxLen        缓冲区大小
** 输　出  : 模块句柄
** 全局变量:
** 调用模块:
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
        if (lib_strcmp(pcFileName, pldconf->VPLC_pcVpPath) == 0) {      /*  匹配文件全路径              */
            lib_strlcpy(pcPathBuffer, pldconf->VPLC_cLoadPath, stMaxLen);
            LW_LD_UNLOCK();
            return  (ERROR_NONE);

        } else {                                                        /*  匹配目录                    */
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

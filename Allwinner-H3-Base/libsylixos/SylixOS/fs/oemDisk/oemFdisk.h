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
** 文   件   名: oemFdisk.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2015 年 09 月 25 日
**
** 描        述: OEM 磁盘分区工具.
**
** 注        意: 分区操作目标为 /dev/blk/xxx 块设备文件.
*********************************************************************************************************/

#ifndef __OEMFDISK_H
#define __OEMFDISK_H

/*********************************************************************************************************
  分区参数
*********************************************************************************************************/

typedef struct {
    UINT8       FDP_ucSzPct;                                            /*  分区大小百分比              */
    BOOL        FDP_bIsActive;                                          /*  是否为活动分区              */
    UINT8       FDP_ucPartType;                                         /*  分区格式                    */
    ULONG       FDP_ulMBytes;                                           /*  ucSzPct > 100 使用此参数    */
    PCHAR       FDP_pcName;                                             /*  GPT 分区名称                */
    ULONG       FDP_ulReserve[6];                                       /*  保留                        */
} LW_OEMFDISK_PART;
typedef LW_OEMFDISK_PART    *PLW_OEMFDISK_PART;

/*********************************************************************************************************
  分区描述
*********************************************************************************************************/

typedef struct {
    UINT64      FDP_u64Size;                                            /*  分区大小                    */
    UINT64      FDP_u64Oft;                                             /*  分区偏移量                  */
    BOOL        FDP_bIsActive;                                          /*  是否为活动分区              */
    UINT8       FDP_ucPartType;                                         /*  分区格式                    */
    ULONG       FDP_ulReserve[8];                                       /*  保留                        */
} LW_OEMFDISK_PINFO;
typedef LW_OEMFDISK_PINFO   *PLW_OEMFDISK_PINFO;

/*********************************************************************************************************
  API
*********************************************************************************************************/

LW_API INT  API_OemFdisk(CPCHAR  pcBlkDev, const LW_OEMFDISK_PART  fdpInfo[],
                         UINT  uiNPart, size_t  stAlign);
LW_API INT  API_OemFdiskEx(CPCHAR  pcBlkDev, const LW_OEMFDISK_PART  fdpInfo[],
                           UINT  uiNPart, size_t  stAlign, BOOL  bGpt);
LW_API INT  API_OemFdiskExWithLba(CPCHAR  pcBlkDev, const LW_OEMFDISK_PART  fdpInfo[],
                                  UINT  uiNPart, size_t  stAlign, BOOL  bGpt, UINT64  ui64FirstLba);
LW_API INT  API_OemFdiskGet(CPCHAR  pcBlkDev, LW_OEMFDISK_PINFO  fdpInfo[], UINT  uiNPart);
LW_API INT  API_OemFdiskGetEx(CPCHAR  pcBlkDev, LW_OEMFDISK_PINFO  fdpInfo[], UINT  uiNPart, BOOL  *pbGpt);
LW_API INT  API_OemFdiskShow(CPCHAR  pcBlkDev);

#define oemFdisk                API_OemFdisk
#define oemFdiskEx              API_OemFdiskEx
#define oemFdiskExWithLba       API_OemFdiskExWithLba
#define oemFdiskGet             API_OemFdiskGet
#define oemFdiskGetEx           API_OemFdiskGetEx
#define oemFdiskShow            API_OemFdiskShow

#endif                                                                  /*  __OEMFDISK_H                */
/*********************************************************************************************************
  END
*********************************************************************************************************/

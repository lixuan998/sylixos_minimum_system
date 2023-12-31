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
** 文   件   名: sdmemory.c
**
** 创   建   人: Zeng.Bo (曾波)
**
** 文件创建日期: 2010 年 11 月 25 日
**
** 描        述: sd 记忆卡客户应用驱动源文件

** BUG:
2010.12.08  优化了 __sdmemTestBusy() 函数.
2010.12.08  SD 设备结构中加入块寻址标记, 以支持 SDHC 卡.
2011.01.12  增加对 SPI 的支持.
2011.03.25  修改 __sdMemIoctl() 函数, 增加设备状态检测.
2011.03.25  修改 API_SdMemDevCreate(), 用于底层驱动安装上层的回调.
2011.04.03  将 API_SdMemDevShowInfo() 改为 API_SdMemDevShow() 统一 SylxiOS Show 函数.
2011.04.03  更改 block io 层回调函数的参数.
2015.03.11  增加卡写保护功能.
2015.09.18  增加对保留扇区的处理.
2015.09.22  增加对 MMC/eMMC 扩展协议的支持.
2015.12.17  增加对 MMC/eMMC 兼容性处理.
2016.01.26  修正对 SPI 模式下的写块操作不符合协议的地方.
2018.05.22  增加对 SD 卡仅能使用1位模式的处理.
2018.12.10  修改对 MMC/eMMC 初始化和读写操作的兼容性提升.
2018.12.29  增加对 MMC/eMMC 电源类别配置和DDR模式支持.
2022.04.02  增加对 MMC/eMMC HS200 模式支持(葛文彬).
*********************************************************************************************************/
#define  __SYLIXOS_STDIO
#define  __SYLIXOS_KERNEL
#include "../SylixOS/kernel/include/k_kernel.h"
#include "../SylixOS/system/include/s_system.h"
/*********************************************************************************************************
  加入裁剪支持
*********************************************************************************************************/
#if (LW_CFG_DEVICE_EN > 0) && (LW_CFG_SDCARD_EN > 0)
#include "sdmemory.h"
#include "../core/sdcore.h"
#include "../core/sdcoreLib.h"
#include "../core/sdstd.h"
#include "../include/sddebug.h"
/*********************************************************************************************************
  sd 块设备内部结构
*********************************************************************************************************/
typedef struct __sd_blk_dev {
    LW_BLK_DEV            SDBLKDEV_blkDev;
    PLW_SDCORE_DEVICE     SDBLKDEV_pcoreDev;
    BOOL                  SDBLKDEV_bIsBlockAddr;                        /*  是否是块寻址                */
    BOOL                  SDBLKDEV_bNeedReSelect;                       /*  是否需要重新选择设备        */
    BOOL                  SDBLKDEV_bNeedReSetBlkSize;                   /*  每次传输需要设置块大小      */
    BOOL                  SDBLKDEV_bNeedReCalcBlkCnt;                   /*  每次传输需要重计算实际块数  */
    UINT8                 SDBLKDEV_ucRdBlkLenBits;
    UINT8                 SDBLKDEV_ucWrBlkLenBits;
    ULONG                 SDBLKDEV_ulSectorOff;                         /*  扇区访问偏移                */
    ULONG                 SDBLKDEV_ulDiskNSector;                       /*  挂载磁盘的实际扇区数量      */
    LW_BLK_INFO           SDBLKDEV_blkinfo;
    
    /*
     * 增加 SDM 后, 为了保持 API 不变, 增加以下成员
     */
    BOOL                  SDBLKDEV_bCoreDevSelf;                        /*  coredev 是自己创建(非SDM给) */
} __SD_BLK_DEV, *__PSD_BLK_DEV;
/*********************************************************************************************************
  内部宏
*********************************************************************************************************/
#define __SDMEM_BLKADDR(pdev)       (pdev->SDBLKDEV_bIsBlockAddr)
#define __SD_CID_PNAME(iN)          (sddevcid.DEVCID_pucProductName[iN])

#define __SD_DEV_RETRY              4
#define __SD_MILLION                1000000
/*********************************************************************************************************
  忙检测函数的等待类型
*********************************************************************************************************/
#define __SD_BUSY_TYPE_READ         0
#define __SD_BUSY_TYPE_RDYDATA      1
#define __SD_BUSY_TYPE_PROG         2
#define __SD_BUSY_TYPE_ERASE        3
#define __SD_BUSY_RETRY             0x3fffffff
#define __SD_TIMEOUT_SEC            2                                   /*  2秒超时为一个经验值         */

#define __SD_CARD_STATUS_MSK        (0x0f << 9)
#define __SD_CARD_STATUS_PRG        (0x07 << 9)                         /*  数据正在编程                */
#define __SD_CARD_STATUS_RDYDATA    (0x01 << 8)
#define __SD_CARD_STATUS_SWITCH_ERR (0x01 << 7)
/*********************************************************************************************************
  私有函数声明
*********************************************************************************************************/
static INT __sdMemTestBusy(PLW_SDCORE_DEVICE psdcoredevice, INT iType);
static INT __sdMemInit(PLW_SDCORE_DEVICE psdcoredevice);
static INT __sdMemWrtSingleBlk(PLW_SDCORE_DEVICE  psdcoredevice,
                               UINT8             *pucBuf,
                               UINT32             uiStartBlk);
static INT __sdMemWrtMultiBlk(PLW_SDCORE_DEVICE  psdcoredevice,
                              UINT8             *pucBuf,
                              UINT32             uiStartBlk,
                              UINT32             uiNBlks);
static INT __sdMemRdSingleBlk(PLW_SDCORE_DEVICE  psdcoredevice,
                              UINT8             *pucBuf,
                              UINT32             uiStartBlk);
static INT __sdMemRdMultiBlk(PLW_SDCORE_DEVICE  psdcoredevice,
                             UINT8             *pucBuf,
                             UINT32             uiStartBlk,
                             UINT32             uiNBlks);

static INT __sdMemBlkWrt(__PSD_BLK_DEV   psdblkdevice,
                         VOID            *pvWrtBuffer,
                         ULONG            ulStartBlk,
                         ULONG            ulBlkCount);
static INT __sdMemBlkRd(__PSD_BLK_DEV   psdblkdevice,
                        VOID           *pvRdBuffer,
                        ULONG           ulStartBlk,
                        ULONG           ulBlkCount);
static INT __sdMemIoctl(__PSD_BLK_DEV    psdblkdevice,
                        INT              iCmd,
                        LONG             lArg);

static INT __sdMemBlkInfoFmt(__PSD_BLK_DEV psdblkdevice);
static INT __sdMemStatus(__PSD_BLK_DEV     psdblkdevice);
static INT __sdMemReset(__PSD_BLK_DEV      psdblkdevice);

static INT __sdMemSdSwCapGet(PLW_SDCORE_DEVICE  psdcoredevice,
                             LW_SDDEV_SCR      *psdscr,
                             LW_SDDEV_CSD      *psdcsd,
                             LW_SDDEV_SW_CAP   *psdswcap);
static INT __sdMemSdHsSwitch(PLW_SDCORE_DEVICE  psdcoredevice, LW_SDDEV_SW_CAP  *psdswcap);

static INT __sdMemMmcFreqChange(PLW_SDCORE_DEVICE  psdcoredevice,
                                LW_SDDEV_CSD      *psdcsd,
                                INT               *piCardCap);
static INT __sdMemMmcBusWidthChange(PLW_SDCORE_DEVICE psdcoredevice,
                                    INT               iCardCap,
                                    LW_SDDEV_EXT_CSD *psddevextcsd);
static INT __sdMemMmcBusWidthChangeHS200(PLW_SDCORE_DEVICE psdcoredevice,
                                         INT               iCardCap,
                                         LW_SDDEV_EXT_CSD *psddevextcsd);
static INT __sdMemMmcSelectPwrClass(PLW_SDCORE_DEVICE psdcoredevice,
                                    INT               iCardCap,
                                    UINT32            uiBusWidth,
                                    LW_SDDEV_EXT_CSD *psddevextcsd);
static INT __sdMemSwitchWait(PLW_SDCORE_DEVICE psdcoredevice,
                             UINT              uiTimeout,
                             BOOL              bSendStatus);

static CPCHAR __sdMemProtVsnStr(UINT8 ucType, UINT8 ucVsn);
static ULONG  __sdMemBlkLogic2Phy(ULONG ulLogic, UINT8 ucBlkLenBits);
static ULONG  __sdMemBlkPhy2Logic(ULONG ulPhy, UINT8 ucBlkLenBits);
static VOID  *__sdMemExtBufAlloc(VOID);
static VOID   __sdMemExtBufFree(VOID *pvExtBuf);
/*********************************************************************************************************
** 函数名称: API_SdMemDevCreate
** 功能描述: 创建一个SD记忆卡设备
** 输    入: iAdapterType     设备挂接的适配器类型 (SDADAPTER_TYPE_SPI 或 SDADAPTER_TYPE_SD)
**           pcAdapterName    挂接的适配器名称
**           pcDeviceName     设备名称
**           psdmemchan       通道
** 输    出: NONE
** 返    回: 成功,返回设备块设备指针,否则返回LW_NULL
** 全局变量:
** 调用模块:
*********************************************************************************************************/
LW_API PLW_BLK_DEV API_SdMemDevCreate (INT                       iAdapterType,
                                       CPCHAR                    pcAdapterName,
                                       CPCHAR                    pcDeviceName,
                                       PLW_SDMEM_CHAN            psdmemchan)
{
    PLW_SDCORE_DEVICE   psdcoredevice     = LW_NULL;
    __PSD_BLK_DEV       psdblkdevice      = LW_NULL;
    PLW_BLK_DEV         pblkdevice        = LW_NULL;
    PLW_SDCORE_CHAN     psdcorechan       = LW_NULL;
    BOOL                bCoreDevSelf      = LW_TRUE;
    BOOL                bNeedReSelect     = LW_FALSE;
    BOOL                bNeedReSetBlkSize = LW_FALSE;
    BOOL                bNeedReCalcBlkCnt = LW_FALSE;

    LW_SDDEV_CSD        sddevcsd;
    BOOL                bBlkAddr;
    INT                 iBlkDevFlag;
    ULONG               ulSectorOff;
    ULONG               ulReSelect;

    UINT8               ucType;
    INT                 iError;

    /*
     * 增加了 SDM 后, 约定：当适配器名称和设备名称为空时, 表示 coredev 由 SDM 创建
     * 此时, psdmemchan 指向对应的coredev
     */
    if (!pcAdapterName && !pcDeviceName) {
        psdcoredevice = (PLW_SDCORE_DEVICE)psdmemchan;
        bCoreDevSelf  = LW_FALSE;

    } else {
        psdcorechan   = (PLW_SDCORE_CHAN)psdmemchan;
        psdcoredevice = API_SdCoreDevCreate(iAdapterType,
                                            pcAdapterName,
                                            pcDeviceName,
                                            psdcorechan);
    }

    if (!psdcoredevice) {
        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "create coredevice failed.\r\n");
        return  (LW_NULL);
    }

    psdcoredevice->COREDEV_pucExtBuf = __sdMemExtBufAlloc();
    if (!psdcoredevice->COREDEV_pucExtBuf) {
        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "alloc ext buffer failed.\r\n");

        if (bCoreDevSelf) {
            API_SdCoreDevDelete(psdcoredevice);
        }
        return  (LW_NULL);
    }

    /*
     * 初始化这个设备(针对记忆卡)
     */
    iError = __sdMemInit(psdcoredevice);
    if (iError != ERROR_NONE) {
        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "do memory initialize failed.\r\n");

        __sdMemExtBufFree(psdcoredevice->COREDEV_pucExtBuf);
        if (bCoreDevSelf) {
            API_SdCoreDevDelete(psdcoredevice);
        }

        return  (LW_NULL);
    }

    psdblkdevice  = (__PSD_BLK_DEV)__SHEAP_ALLOC(sizeof(__SD_BLK_DEV));
    if (!psdblkdevice) {
        __sdMemExtBufFree(psdcoredevice->COREDEV_pucExtBuf);
        if (bCoreDevSelf) {
            API_SdCoreDevDelete(psdcoredevice);
        }

        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "system low memory.\r\n");
        _ErrorHandle(ERROR_SYSTEM_LOW_MEMORY);
        return  (LW_NULL);

    }

    API_SdCoreDevTypeView(psdcoredevice, &ucType);

    iError = API_SdCoreDevCsdView(psdcoredevice, &sddevcsd);
    if (iError != ERROR_NONE) {
        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "view csd of device failed.\r\n");
        __SHEAP_FREE(psdblkdevice);

        __sdMemExtBufFree(psdcoredevice->COREDEV_pucExtBuf);
        if (bCoreDevSelf) {
            API_SdCoreDevDelete(psdcoredevice);
        }

        return  (LW_NULL);
    }

    iError = API_SdCoreDevIsBlockAddr(psdcoredevice, &bBlkAddr);
    if (iError != ERROR_NONE) {
        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "unkonwn address access way.\r\n");
        __SHEAP_FREE(psdblkdevice);

        __sdMemExtBufFree(psdcoredevice->COREDEV_pucExtBuf);
        if (bCoreDevSelf) {
            API_SdCoreDevDelete(psdcoredevice);
        }

        return  (LW_NULL);
    }

    if (API_SdmHostIsCardWp(psdcoredevice)) {
        iBlkDevFlag = O_RDONLY;

    } else {
        iBlkDevFlag = O_RDWR;
    }

    API_SdmHostExtOptGet(psdcoredevice,
                         SDHOST_EXTOPT_CONFIG_FLAG_GET,
                         (LONG)&ulReSelect);

    if (ucType != SDDEV_TYPE_MMC) {
        if (ulReSelect & SDHOST_EXTOPT_CONFIG_RESELECT_SDMEM) {
            bNeedReSelect = LW_TRUE;

        } else {
            API_SdCoreDevSelect(psdcoredevice);                       /*  只在这里调用一次              */
        }
    }

    if (sddevcsd.DEVCSD_ucWriteBlkLenBits != sddevcsd.DEVCSD_ucReadBlkLenBits) {
        bNeedReSetBlkSize = LW_TRUE;
    }

    if ((sddevcsd.DEVCSD_ucReadBlkLenBits  != SD_MEM_DEFAULT_BLKSIZE_NBITS) ||
        (sddevcsd.DEVCSD_ucWriteBlkLenBits != SD_MEM_DEFAULT_BLKSIZE_NBITS)) {
        bNeedReCalcBlkCnt = LW_TRUE;
    }

    API_SdmHostExtOptGet(psdcoredevice,
                         SDHOST_EXTOPT_RESERVE_SECTOR_GET,
                         (LONG)&ulSectorOff);

    if (sddevcsd.DEVCSD_uiCapacity <= ulSectorOff) {
        ulSectorOff = 0;
    }

    psdblkdevice->SDBLKDEV_bIsBlockAddr      = bBlkAddr;                /*  设置寻址方式                */
    psdblkdevice->SDBLKDEV_pcoreDev          = psdcoredevice;           /*  连接核心设备                */
    psdblkdevice->SDBLKDEV_bCoreDevSelf      = bCoreDevSelf;
    psdblkdevice->SDBLKDEV_bNeedReSelect     = bNeedReSelect;
    psdblkdevice->SDBLKDEV_bNeedReSetBlkSize = bNeedReSetBlkSize;
    psdblkdevice->SDBLKDEV_bNeedReCalcBlkCnt = bNeedReCalcBlkCnt;
    psdblkdevice->SDBLKDEV_ucRdBlkLenBits    = sddevcsd.DEVCSD_ucReadBlkLenBits;
    psdblkdevice->SDBLKDEV_ucWrBlkLenBits    = sddevcsd.DEVCSD_ucWriteBlkLenBits;
    psdblkdevice->SDBLKDEV_ulSectorOff       = ulSectorOff;

    /*
     * 逻辑扇区大小为 512
     * 需要将实际的扇区数转换为逻辑扇区数
     */
    psdblkdevice->SDBLKDEV_ulDiskNSector  = __sdMemBlkPhy2Logic((ULONG)sddevcsd.DEVCSD_uiCapacity,
                                                                sddevcsd.DEVCSD_ucReadBlkLenBits)
                                          - ulSectorOff;

    pblkdevice = &psdblkdevice->SDBLKDEV_blkDev;

    pblkdevice->BLKD_pcName            = "SD-MemCard";
    pblkdevice->BLKD_pfuncBlkRd        = __sdMemBlkRd;
    pblkdevice->BLKD_pfuncBlkWrt       = __sdMemBlkWrt;
    pblkdevice->BLKD_pfuncBlkIoctl     = __sdMemIoctl;
    pblkdevice->BLKD_pfuncBlkReset     = __sdMemReset;
    pblkdevice->BLKD_pfuncBlkStatusChk = __sdMemStatus;

    pblkdevice->BLKD_ulNSector         = psdblkdevice->SDBLKDEV_ulDiskNSector;

    pblkdevice->BLKD_ulBytesPerSector  = SD_MEM_DEFAULT_BLKSIZE;
    pblkdevice->BLKD_ulBytesPerBlock   = SD_MEM_DEFAULT_BLKSIZE;

    pblkdevice->BLKD_bRemovable        = LW_TRUE;
    pblkdevice->BLKD_bDiskChange       = LW_FALSE;                      /*  媒质没有改变                */
    pblkdevice->BLKD_iRetry            = __SD_DEV_RETRY;                /*  重试次数                    */
    pblkdevice->BLKD_iFlag             = iBlkDevFlag;
    pblkdevice->BLKD_iLogic            = 0;
    pblkdevice->BLKD_uiLinkCounter     = 0;
    pblkdevice->BLKD_pvLink            = LW_NULL;
    pblkdevice->BLKD_uiPowerCounter    = 0;
    pblkdevice->BLKD_uiInitCounter     = 0;

    __sdMemBlkInfoFmt(psdblkdevice);                                    /*  格式化块设备信息            */

    return  (pblkdevice);
}
/*********************************************************************************************************
** 函数名称: API_SdMemDevDelete
** 功能描述: 删除一个SD记忆卡设备
** 输    入: pblkdevice 块设备结构指针
** 输    出: NONE
** 返    回: ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
LW_API INT  API_SdMemDevDelete (PLW_BLK_DEV pblkdevice)
{
    PLW_SDCORE_DEVICE   psdcoredevice   = LW_NULL;
    __PSD_BLK_DEV       psdblkdevice    = LW_NULL;
    INT                 iError;

    if (!pblkdevice) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    psdblkdevice  = (__PSD_BLK_DEV)pblkdevice;
    psdcoredevice = psdblkdevice->SDBLKDEV_pcoreDev;

    if (psdcoredevice) {
        __sdMemExtBufFree(psdcoredevice->COREDEV_pucExtBuf);
    }

    if (psdcoredevice && psdblkdevice->SDBLKDEV_bCoreDevSelf) {
        iError = API_SdCoreDevDelete(psdcoredevice);                    /*  先删除core设备              */
        if (iError != ERROR_NONE) {
            SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "delet coredevice failed.\r\n");
            _ErrorHandle(EINVAL);
            return  (PX_ERROR);
        }
    }

    __SHEAP_FREE(psdblkdevice);                                         /*  再释放sd blk                */

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: API_SdMemDevShow
** 功能描述: 打印SD设备信息
** 输    入: pblkdevice  块设备结构指针
** 输    出: NONE
** 返    回: ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
LW_API INT  API_SdMemDevShow (PLW_BLK_DEV pblkdevice)
{
    PLW_SDCORE_DEVICE   psdcoredevice   = LW_NULL;
    __PSD_BLK_DEV       psdblkdevice    = LW_NULL;
    LW_SDDEV_CSD        sddevcsd;
    LW_SDDEV_CID        sddevcid;
    LW_SDDEV_SW_CAP     sddevswcap;

    UINT32              uiCapMod;
    UINT64              ullCap;
    UINT32              uiMaxSpeed;
    UINT8               ucType;
    CPCHAR              pcTypeStr = "unknown";
    CPCHAR              pcVsnStr  = "unknown";

    if (!pblkdevice) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    if (API_IoTaskStdGet(API_ThreadIdSelf(), STD_OUT) < 0) {
        return  (ERROR_NONE);
    }

    psdblkdevice   = (__PSD_BLK_DEV)pblkdevice;
    psdcoredevice = psdblkdevice->SDBLKDEV_pcoreDev;

    if (!psdcoredevice) {
        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "no core device.\r\n");
        return  (PX_ERROR);
    }

    API_SdCoreDevCsdView(psdcoredevice, &sddevcsd);
    API_SdCoreDevCidView(psdcoredevice, &sddevcid);
    API_SdCoreDevSwCapView(psdcoredevice, &sddevswcap);
    API_SdCoreDevTypeView(psdcoredevice, &ucType);

    if (sddevswcap.DEVSWCAP_uiHsMaxDtr) {
        uiMaxSpeed = sddevswcap.DEVSWCAP_uiHsMaxDtr;

    } else {
        uiMaxSpeed = sddevcsd.DEVCSD_uiTranSpeed;
    }

    ullCap   = (UINT64)sddevcsd.DEVCSD_uiCapacity * ((UINT64)1 << sddevcsd.DEVCSD_ucReadBlkLenBits);
    uiCapMod = ullCap % LW_CFG_MB_SIZE;

    switch (ucType) {

    case SDDEV_TYPE_MMC:
        pcTypeStr = "MMC/eMMC";
        break;

    case SDDEV_TYPE_SDSC:
        pcTypeStr = "SDSC";
        break;

    case SDDEV_TYPE_SDHC:
        pcTypeStr = "SDHC";
        break;

    case SDDEV_TYPE_SDXC:
        pcTypeStr = "SDXC";
        break;

    default:
        break;
    }

    pcVsnStr = __sdMemProtVsnStr(ucType, sddevcsd.DEVCSD_ucStructure);

    printf("\nSD Memory Information >>\n");
    printf("Manufacturer : 0x%02X\n", sddevcid.DEVCID_ucMainFid);
    if (ucType == SDDEV_TYPE_MMC) {
        printf("OEM ID       : %08X\n", sddevcid.DEVCID_usOemId);

    } else {
        printf("OEM ID       : %c%c\n", sddevcid.DEVCID_usOemId >> 8,
                                        sddevcid.DEVCID_usOemId & 0xff);
    }

    printf("Media Type   : %s(%s)\n", pcTypeStr, pcVsnStr);
    printf("Product Name : %c%c%c%c%c\n",
                           __SD_CID_PNAME(0),
                           __SD_CID_PNAME(1),
                           __SD_CID_PNAME(2),
                           __SD_CID_PNAME(3),
                           __SD_CID_PNAME(4));
    printf("Product Vsn  : v%d.%d\n", sddevcid.DEVCID_ucProductVsn >> 4,
                                      sddevcid.DEVCID_ucProductVsn & 0xf);
    printf("Serial Num   : %X\n", sddevcid.DEVCID_uiSerialNum);
    printf("Date         : %d/%02d\n", sddevcid.DEVCID_uiYear, sddevcid.DEVCID_ucMonth);
    printf("Max Speed    : %dMB/s\n", uiMaxSpeed / __SD_MILLION);
    printf("Capacity     : %u.%03u MB\n", (UINT32)(ullCap / LW_CFG_MB_SIZE), uiCapMod / 1000);
    printf("Block Size   : %d(R) %d(W)\n", 1 << psdblkdevice->SDBLKDEV_ucRdBlkLenBits,
                                           1 << psdblkdevice->SDBLKDEV_ucWrBlkLenBits);

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __sdMemTestBusy
** 功能描述: 忙检测函数.在写时使用
** 输    入: psdcoredevice  核心设备
**           iType          检测类型
** 输    出: NONE
** 返    回: ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT __sdMemTestBusy (PLW_SDCORE_DEVICE psdcoredevice, INT iType)
{
    INT     iError;
    UINT    uiSta;
    INT     iRetry = 0;

    struct timespec   tvOld;
    struct timespec   tvNow;

    lib_clock_gettime(CLOCK_MONOTONIC, &tvOld);

    while (iRetry++ < __SD_BUSY_RETRY) {
        iError = API_SdCoreDevGetStatus(psdcoredevice, &uiSta);

        if (iError != ERROR_NONE) {
            SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "get device status failed.\r\n");
            break;
        }

        if ((uiSta & __SD_CARD_STATUS_MSK) != __SD_CARD_STATUS_PRG) {
            return   (ERROR_NONE);
        }

        lib_clock_gettime(CLOCK_MONOTONIC, &tvNow);
        if ((tvNow.tv_sec - tvOld.tv_sec) >= __SD_TIMEOUT_SEC) {        /*  超时退出                    */
            SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "timeout.\r\n");
            break;
        }
    }

    return  (PX_ERROR);
}
/*********************************************************************************************************
** 函数名称: __sdMemInit
** 功能描述: SD记忆卡初始化
** 输    入: psdcoredevice  核心设备
** 输    出: NONE
** 返    回: ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT __sdMemInit (PLW_SDCORE_DEVICE psdcoredevice)
{
    INT               iError;
    UINT8             ucType;
    UINT32            uiOCR;
    UINT32            uiRCA;
    LW_SDDEV_OCR      sddevocr;
    LW_SDDEV_CID      sddevcid;
    LW_SDDEV_CSD      sddevcsd;
    LW_SDDEV_SCR      sddevscr;
    LW_SDDEV_SW_CAP   sddevswcap;
    LW_SDDEV_EXT_CSD  sddevextcsd;
    INT               iCardCap = 0;
    INT               iHostCap = 0;

    lib_bzero(&sddevcid, sizeof(LW_SDDEV_CID));
    lib_bzero(&sddevcsd, sizeof(LW_SDDEV_CSD));
    lib_bzero(&sddevscr, sizeof(LW_SDDEV_SCR));
    lib_bzero(&sddevswcap, sizeof(LW_SDDEV_SW_CAP));

    switch (psdcoredevice->COREDEV_iAdapterType) {

    case SDADAPTER_TYPE_SD:
        API_SdCoreDevCtl(psdcoredevice, SDBUS_CTRL_POWEROFF, 0);
        bspDelayUs(10);
        API_SdCoreDevCtl(psdcoredevice, SDBUS_CTRL_POWERON, 0);

        iError = API_SdCoreDevCtl(psdcoredevice,
                                  SDBUS_CTRL_SETCLK,
                                  SDARG_SETCLK_LOW);                    /*  初始化时 低速时钟           */
        if (iError != ERROR_NONE) {
            SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "set clock to low failed.\r\n");
            return  (PX_ERROR);
        }

        API_TimeMSleep(10);
        API_SdCoreDevCtl(psdcoredevice, SDBUS_CTRL_SETBUSWIDTH, SDARG_SETBUSWIDTH_1);

        iError = API_SdCoreDevReset(psdcoredevice);                     /*  cmd0 go idle                */
        if (iError != ERROR_NONE) {
            SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "device reset failed.\r\n");
            return  (PX_ERROR);
        }

        iError = API_SdCoreDevCtl(psdcoredevice,
                                  SDBUS_CTRL_GETOCR,
                                  (LONG)&uiOCR);

        iError = API_SdCoreDevSendIfCond(psdcoredevice);                /*  cmd8 (v2.0以上的卡必须这个) */
                                                                        /*  v2.0以下的卡无应答,忽略错误 */
        if (iError == ERROR_NONE) {                                     /*  SDHC卡初始化支持            */
            uiOCR |= SD_OCR_HCS;
        }

        iError = API_SdCoreDevSendAppOpCond(psdcoredevice,
                                            uiOCR,
                                            &sddevocr,
                                            &ucType);                   /*  acmd41                      */
        if (iError != ERROR_NONE) {
            SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "device don't support the ocr.\r\n");
            return  (PX_ERROR);
        }

        /*
         * 这里保存设备 OCR 信息, 随后会根据此信息判断设备是否真的是
         * 字节寻址还是块寻址
         */
        if (sddevocr & SD_OCR_HCS) {
            psdcoredevice->COREDEV_iDevSta |= COREDEV_STA_HIGHCAP_OCR;
        }

        API_SdCoreDevTypeSet(psdcoredevice, ucType);                    /*  设置type域                  */

        iError = API_SdCoreDevSendAllCID(psdcoredevice, &sddevcid);     /*  cmd2                        */
        if (iError != ERROR_NONE) {
            SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "get device cid failed.\r\n");
            return  (PX_ERROR);
        }
        if (ucType == SDDEV_TYPE_MMC) {
            uiRCA = 0x01;
            iError = API_SdCoreDevMmcSetRelativeAddr(psdcoredevice, uiRCA);

        } else {
            iError = API_SdCoreDevSendRelativeAddr(psdcoredevice, &uiRCA);
                                                                        /*  cmd3                        */
        }
        if (iError != ERROR_NONE) {
            SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "get device rca failed.\r\n");
            return  (PX_ERROR);
        }

        API_SdCoreDevRcaSet(psdcoredevice, uiRCA);                      /*  设置RCA域                   */

        iError = API_SdCoreDevSendAllCSD(psdcoredevice, &sddevcsd);
        if (iError != ERROR_NONE) {
            SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "get device csd failed.\r\n");
            return  (PX_ERROR);
        }

        if (ucType == SDDEV_TYPE_MMC) {
            iError = API_SdCoreDevCtl(psdcoredevice,
                                      SDBUS_CTRL_SETCLK,
                                      SDARG_SETCLK_NORMAL);
            if (iError != ERROR_NONE) {
                SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "set clock to normal failed.\r\n");
                return  (PX_ERROR);
            }

            lib_bzero(&sddevextcsd, sizeof(sddevextcsd));
            API_SdCoreDevSelect(psdcoredevice);                         /*  MMC 只需在这里选择卡一次    */
            API_SdCoreDecodeExtCSD(psdcoredevice, &sddevcsd, &sddevextcsd);

            if ((sddevextcsd.DEVEXTCSD_uiRev >= 5) && (sddevcid.DEVCID_uiYear < 2010)) {
                sddevcid.DEVCID_uiYear += 16;
            }

        } else {
            sddevcsd.DEVCSD_uiCapacity       <<= sddevcsd.DEVCSD_ucReadBlkLenBits
                                               - SD_MEM_DEFAULT_BLKSIZE_NBITS;
            sddevcsd.DEVCSD_ucReadBlkLenBits   = SD_MEM_DEFAULT_BLKSIZE_NBITS;
            sddevcsd.DEVCSD_ucWriteBlkLenBits  = SD_MEM_DEFAULT_BLKSIZE_NBITS;
        }

        API_SdCoreDevCsdSet(psdcoredevice, &sddevcsd);                  /*  设置CSD域                   */
        API_SdCoreDevCidSet(psdcoredevice, &sddevcid);                  /*  设置CID域                   */

        if (ucType != SDDEV_TYPE_MMC) {
            iError = API_SdCoreDevCtl(psdcoredevice,
                                      SDBUS_CTRL_SETCLK,
                                      SDARG_SETCLK_NORMAL);
            if (iError != ERROR_NONE) {
                SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "set clock to normal failed.\r\n");
                return  (PX_ERROR);
            }

            API_SdmHostCapGet(psdcoredevice, &iHostCap);

            API_SdCoreDevSelect(psdcoredevice);
            iError = API_SdCoreDevSendAllSCR(psdcoredevice, &sddevscr);
            if ((iError == ERROR_NONE) && (iHostCap & SDHOST_CAP_HIGHSPEED)) {
                API_SdCoreDevScrSet(psdcoredevice, &sddevscr);          /*  设置SCR域                   */
                iError = __sdMemSdSwCapGet(psdcoredevice,
                                           &sddevscr,
                                           &sddevcsd,
                                           &sddevswcap);
                if (iError == ERROR_NONE) {
                    API_SdCoreDevSwCapSet(psdcoredevice, &sddevswcap);  /*  设置SWCAP域                 */
                    iError = __sdMemSdHsSwitch(psdcoredevice, &sddevswcap);
                    if (iError == ERROR_NONE) {
                        API_SdCoreDevCtl(psdcoredevice,
                                         SDBUS_CTRL_SETCLK,
                                         SDARG_SETCLK_MAX);
                    }
                }
            }
            API_SdCoreDevDeSelect(psdcoredevice);

            iError = API_SdCoreDevSetBlkLen(psdcoredevice, SD_MEM_DEFAULT_BLKSIZE);
            if (iError != ERROR_NONE) {
                SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "set blklen failed.\r\n");
                return  (PX_ERROR);
            }

            if ((iHostCap & SDHOST_CAP_DATA_4BIT) &&
                !(iHostCap & SDHOST_CAP_SD_FORCE_1BIT)) {
                iError = API_SdCoreDevSetBusWidth(psdcoredevice, SDARG_SETBUSWIDTH_4);
                                                                        /*  acmd6 set bus width         */
                if (iError != ERROR_NONE) {
                    SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "set bus width error.\r\n");
                    return  (PX_ERROR);
                }
                API_SdCoreDevCtl(psdcoredevice, SDBUS_CTRL_SETBUSWIDTH, SDARG_SETBUSWIDTH_4);
            }

        } else {                                                        /*  mmc 总线特殊设置            */
            iError = __sdMemMmcFreqChange(psdcoredevice, &sddevcsd, &iCardCap);
            if (iError != ERROR_NONE) {
                SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "mmc change frequency error.\r\n");
                return  (PX_ERROR);
            }

            iError = __sdMemMmcBusWidthChangeHS200(psdcoredevice, iCardCap, &sddevextcsd);
            if (iError != ERROR_NONE) {
                iError = __sdMemMmcBusWidthChange(psdcoredevice, iCardCap, &sddevextcsd);
                if (iError != ERROR_NONE) {
                    SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "mmc change bus width error.\r\n");
                    return  (PX_ERROR);
                }
            }

            /*
             * MMC 扇区大小配置为不超过512
             */
            if (sddevcsd.DEVCSD_ucReadBlkLenBits > SD_MEM_DEFAULT_BLKSIZE_NBITS) {
                sddevcsd.DEVCSD_uiCapacity       <<= sddevcsd.DEVCSD_ucReadBlkLenBits
                                                   - SD_MEM_DEFAULT_BLKSIZE_NBITS;
                sddevcsd.DEVCSD_ucReadBlkLenBits   = SD_MEM_DEFAULT_BLKSIZE_NBITS;
            }

            if (sddevcsd.DEVCSD_ucWriteBlkLenBits > SD_MEM_DEFAULT_BLKSIZE_NBITS) {
                sddevcsd.DEVCSD_ucWriteBlkLenBits = SD_MEM_DEFAULT_BLKSIZE_NBITS;
            }

            if (sddevcsd.DEVCSD_ucReadBlkLenBits == sddevcsd.DEVCSD_ucWriteBlkLenBits) {
                API_SdCoreDevSetBlkLenRaw(psdcoredevice, 1 << sddevcsd.DEVCSD_ucReadBlkLenBits);
            }
        }

        return  (ERROR_NONE);

    case SDADAPTER_TYPE_SPI:
        API_SdCoreDevCtl(psdcoredevice, SDBUS_CTRL_POWERON, 0);
        API_TimeMSleep(3);

        iError = API_SdCoreDevCtl(psdcoredevice,
                                  SDBUS_CTRL_SETCLK,
                                  SDARG_SETCLK_LOW);                    /*  初始化时 低速时钟           */
        if (iError != ERROR_NONE) {
            SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "set clock to low failed.\r\n");
            return  (PX_ERROR);
        }

        API_SdCoreDevSpiClkDely(psdcoredevice, 100);                    /*  延时大于74个时钟            */

        iError = API_SdCoreDevReset(psdcoredevice);                     /*  cmd0 go idle                */
        if (iError != ERROR_NONE) {
            SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "device reset failed.\r\n");
            return  (PX_ERROR);
        }

        iError = API_SdCoreDevCtl(psdcoredevice,
                                  SDBUS_CTRL_GETOCR,
                                  (LONG)&uiOCR);

        iError = API_SdCoreSpiSendIfCond(psdcoredevice);                /*  cmd8 (v2.0以上的卡必须这个) */
                                                                        /*  v2.0以下的卡无应答,忽略错误 */

        uiOCR |= SD_OCR_HCS;                                            /*  SDHC卡初始化支持            */

        iError = API_SdCoreDevSendAppOpCond(psdcoredevice,
                                            uiOCR,
                                            &sddevocr,
                                            &ucType);                   /*  acmd41   +  cmd58(spi)      */
        if (iError != ERROR_NONE) {
            SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "device don't support the ocr.\r\n");
            return  (PX_ERROR);
        }

        /*
         * 这里保存设备 OCR 信息, 随后会根据此信息判断设备是否真的是
         * 字节寻址还是块寻址
         */
        if (sddevocr & SD_OCR_HCS) {
            psdcoredevice->COREDEV_iDevSta |= COREDEV_STA_HIGHCAP_OCR;
        }

        API_SdCoreDevTypeSet(psdcoredevice, ucType);                    /*  设置type域                  */

#if LW_CFG_SDCARD_CRC_EN > 0
        API_SdCoreDevSpiCrcEn(psdcoredevice, LW_TRUE);                  /*  使能crc                     */
#endif

        iError = API_SdCoreDevSetBlkLen(psdcoredevice, SD_MEM_DEFAULT_BLKSIZE);
        if (iError != ERROR_NONE) {
            SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "set blklen failed.\r\n");
            return  (PX_ERROR);
        }

        iError = API_SdCoreDevSendAllCID(psdcoredevice, &sddevcid);     /*  cmd2                        */
        if (iError != ERROR_NONE) {
            SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "get device cid failed.\r\n");
            return  (PX_ERROR);
        }

        iError = API_SdCoreDevSendAllCSD(psdcoredevice, &sddevcsd);
        if (iError != ERROR_NONE) {
            SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "get device csd failed.\r\n");
            return  (PX_ERROR);
        }

        if (ucType == SDDEV_TYPE_MMC) {
            LW_SDDEV_EXT_CSD  sddevextcsd;

            lib_bzero(&sddevextcsd, sizeof(sddevextcsd));
            API_SdCoreDecodeExtCSD(psdcoredevice, &sddevcsd, &sddevextcsd);
        }

        sddevcsd.DEVCSD_uiCapacity       <<= sddevcsd.DEVCSD_ucReadBlkLenBits
                                           - SD_MEM_DEFAULT_BLKSIZE_NBITS;
        sddevcsd.DEVCSD_ucReadBlkLenBits   = SD_MEM_DEFAULT_BLKSIZE_NBITS;
        sddevcsd.DEVCSD_ucWriteBlkLenBits  = SD_MEM_DEFAULT_BLKSIZE_NBITS;

        API_SdCoreDevCsdSet(psdcoredevice, &sddevcsd);                  /*  设置CSD域                   */
        API_SdCoreDevCidSet(psdcoredevice, &sddevcid);                  /*  设置CID域                   */

        iError = API_SdCoreDevCtl(psdcoredevice,
                                  SDBUS_CTRL_SETCLK,
                                  SDARG_SETCLK_NORMAL);                 /*  设置为全速时钟              */
        if (iError != ERROR_NONE) {
            SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "set to normal clock mode failed.\r\n");
        }
        return  (ERROR_NONE);

    default:
        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "unknown adapter type.\r\n");
        break;
    }

    return  (PX_ERROR);
}
/*********************************************************************************************************
** 函数名称: __sdMemWrtSingleBlk
** 功能描述: SD记忆卡设备写单块
** 输    入: psdcoredevice 核心设备结构
**           pucBuf        写缓冲
**           uiStartBlk    起始地址
** 输    出: NONE
** 返    回: ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT __sdMemWrtSingleBlk (PLW_SDCORE_DEVICE  psdcoredevice,
                                UINT8             *pucBuf,
                                UINT32             uiStartBlk)
{
    LW_SD_MESSAGE   sdmsg;
    LW_SD_COMMAND   sdcmd;
    LW_SD_DATA      sddat;
    INT             iError;
    INT             iDevSta;

    iDevSta = API_SdCoreDevStaView(psdcoredevice);
    if (iDevSta != SD_DEVSTA_EXIST) {
        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "device does not exist.\r\n");
        return  (PX_ERROR);
    }

    lib_bzero(&sdcmd, sizeof(sdcmd));
    lib_bzero(&sddat, sizeof(sddat));

    sdcmd.SDCMD_uiOpcode   = SD_WRITE_BLOCK;
    sdcmd.SDCMD_uiArg      = uiStartBlk;
    sdcmd.SDCMD_uiFlag     = SD_RSP_SPI_R1 | SD_RSP_R1 | SD_CMD_ADTC;   /*  命令                        */

    sddat.SDDAT_uiBlkNum   = 1;
    sddat.SDDAT_uiBlkSize  = SD_MEM_DEFAULT_BLKSIZE;
    sddat.SDDAT_uiFlags    = SD_DAT_WRITE;                              /*  数据                        */

    sdmsg.SDMSG_pucWrtBuffer = pucBuf;
    sdmsg.SDMSG_psddata      = &sddat;
    sdmsg.SDMSG_psdcmdCmd    = &sdcmd;
    sdmsg.SDMSG_psdcmdStop   = LW_NULL;
    sdmsg.SDMSG_pucRdBuffer  = LW_NULL;                                 /*  读缓冲为空                  */

    iError = API_SdCoreDevTransfer(psdcoredevice, &sdmsg, 1);
    if (iError != ERROR_NONE) {
        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "request error.\r\n");
        return  (PX_ERROR);
    }

    if (COREDEV_IS_SD(psdcoredevice)) {
        iError = __sdMemTestBusy(psdcoredevice, __SD_BUSY_TYPE_PROG);
        if (iError != ERROR_NONE) {
            SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "check busy error.\r\n");
            return  (PX_ERROR);
        }
    }

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __sdMemWrtMultiBlk
** 功能描述: SD记忆卡设备写多块
** 输    入: psdcoredevice 核心设备结构
**           pucBuf      写缓冲
**           uiStartBlk  起始地址
**           uiNBlks     块数量
** 输    出: NONE
** 返    回: ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT __sdMemWrtMultiBlk (PLW_SDCORE_DEVICE  psdcoredevice,
                               UINT8             *pucBuf,
                               UINT32             uiStartBlk,
                               UINT32             uiNBlks)
{
    LW_SD_MESSAGE   sdmsg;
    LW_SD_COMMAND   sdcmd;
    LW_SD_DATA      sddat;
    LW_SD_COMMAND   sdcmdStop;
    INT             iError;
    INT             iDevSta;

    iDevSta = API_SdCoreDevStaView(psdcoredevice);
    if (iDevSta != SD_DEVSTA_EXIST) {
        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "device does not exist.\r\n");
        return  (PX_ERROR);
    }

    lib_bzero(&sdcmd, sizeof(sdcmd));
    lib_bzero(&sdcmdStop, sizeof(sdcmdStop));
    lib_bzero(&sddat, sizeof(sddat));

    sdcmd.SDCMD_uiOpcode  = SD_WRITE_MULTIPLE_BLOCK;
    sdcmd.SDCMD_uiArg     = uiStartBlk;
    sdcmd.SDCMD_uiFlag    = SD_RSP_SPI_R1 | SD_RSP_R1 | SD_CMD_ADTC;    /*  命令                        */

    sddat.SDDAT_uiBlkNum  = uiNBlks;
    sddat.SDDAT_uiBlkSize = SD_MEM_DEFAULT_BLKSIZE;
    sddat.SDDAT_uiFlags   = SD_DAT_WRITE;                               /*  数据                        */

    /*
     * 在多块写传输中,要发送停止命令.该命令在SD和SPI模式下不同
     */
    if (COREDEV_IS_SD(psdcoredevice)) {
        sdcmdStop.SDCMD_uiOpcode = SD_STOP_TRANSMISSION;
        sdcmdStop.SDCMD_uiFlag   = SD_RSP_SPI_R1B | SD_RSP_R1B | SD_CMD_AC;
        sdmsg.SDMSG_psdcmdStop   = &sdcmdStop;                          /*  停止命令                    */

    } else {
        sdmsg.SDMSG_psdcmdStop   = LW_NULL;
    }

    sdmsg.SDMSG_pucWrtBuffer = pucBuf;
    sdmsg.SDMSG_psddata      = &sddat;
    sdmsg.SDMSG_psdcmdCmd    = &sdcmd;
    sdmsg.SDMSG_pucRdBuffer  = LW_NULL;                                 /*  读缓冲为空                  */

    iError = API_SdCoreDevTransfer(psdcoredevice, &sdmsg, 1);
    if (iError != ERROR_NONE) {
        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "request error.\r\n");
        return  (PX_ERROR);
    }

    /*
     * SPI多块写,最后发送停止数据令牌
     */
    if (COREDEV_IS_SPI(psdcoredevice)) {
        API_SdCoreSpiMulWrtStop(psdcoredevice);

    } else {
        iError = __sdMemTestBusy(psdcoredevice, __SD_BUSY_TYPE_PROG);
        if (iError != ERROR_NONE) {
            SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "check busy error.\r\n");
            return  (PX_ERROR);
        }
    }

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __sdMemRdSingleBlk
** 功能描述: SD记忆卡设备读单块
** 输    入: psdcoredevice 核心设备结构
**           pucBuf      读缓冲
**           uiStartBlk  起始地址
** 输    出: NONE
** 返    回: ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT __sdMemRdSingleBlk (PLW_SDCORE_DEVICE  psdcoredevice,
                               UINT8             *pucBuf,
                               UINT32             uiStartBlk)
{
    LW_SD_MESSAGE   sdmsg;
    LW_SD_COMMAND   sdcmd;
    LW_SD_DATA      sddat;
    INT             iError;
    INT             iDevSta;

    iDevSta = API_SdCoreDevStaView(psdcoredevice);
    if (iDevSta != SD_DEVSTA_EXIST) {
        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "device does not exist.\r\n");
        return  (PX_ERROR);
    }

    lib_bzero(&sdcmd, sizeof(sdcmd));
    lib_bzero(&sddat, sizeof(sddat));

    sdcmd.SDCMD_uiOpcode   = SD_READ_SINGLE_BLOCK;
    sdcmd.SDCMD_uiArg      = uiStartBlk;
    sdcmd.SDCMD_uiFlag     = SD_RSP_SPI_R1 | SD_RSP_R1 | SD_CMD_ADTC;   /*  命令                        */

    sddat.SDDAT_uiBlkNum   = 1;
    sddat.SDDAT_uiBlkSize  = SD_MEM_DEFAULT_BLKSIZE;
    sddat.SDDAT_uiFlags    = SD_DAT_READ;                               /*  数据                        */

    sdmsg.SDMSG_pucRdBuffer  = pucBuf;
    sdmsg.SDMSG_psddata      = &sddat;
    sdmsg.SDMSG_psdcmdCmd    = &sdcmd;
    sdmsg.SDMSG_psdcmdStop   = LW_NULL;
    sdmsg.SDMSG_pucWrtBuffer = LW_NULL;                                 /*  写缓冲为空                  */

    iError = API_SdCoreDevTransfer(psdcoredevice, &sdmsg, 1);
    if (iError != ERROR_NONE) {
        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "request error.\r\n");
        return  (PX_ERROR);
    }

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __sdMemRdMultiBlk
** 功能描述: SD记忆卡设备读多块
** 输    入: psdcoredevice 核心设备结构
**           pucBuf      读缓冲
**           uiStartBlk  起始地址
**           uiNBlks     块数量
** 输    出: NONE
** 返    回: ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT __sdMemRdMultiBlk (PLW_SDCORE_DEVICE   psdcoredevice,
                              UINT8              *pucBuf,
                              UINT32              uiStartBlk,
                              UINT32              uiNBlks)
{
    LW_SD_MESSAGE   sdmsg;
    LW_SD_COMMAND   sdcmd;
    LW_SD_DATA      sddat;
    LW_SD_COMMAND   sdcmdStop;
    INT             iError;
    INT             iDevSta;

    iDevSta = API_SdCoreDevStaView(psdcoredevice);
    if (iDevSta != SD_DEVSTA_EXIST) {
        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "device does not exist.\r\n");
        return  (PX_ERROR);
    }

    lib_bzero(&sdcmd, sizeof(sdcmd));
    lib_bzero(&sdcmdStop, sizeof(sdcmdStop));
    lib_bzero(&sddat, sizeof(sddat));

    sdcmd.SDCMD_uiOpcode  = SD_READ_MULTIPLE_BLOCK;
    sdcmd.SDCMD_uiArg     = uiStartBlk;
    sdcmd.SDCMD_uiFlag    = SD_RSP_SPI_R1 | SD_RSP_R1 | SD_CMD_ADTC;    /*  命令                        */

    sddat.SDDAT_uiBlkNum  = uiNBlks;
    sddat.SDDAT_uiBlkSize = SD_MEM_DEFAULT_BLKSIZE;
    sddat.SDDAT_uiFlags   = SD_DAT_READ;                                /*  数据                        */

    /*
     * 在多块读操作中, SPI和SD模式的停止命令是相同的
     */
    sdcmdStop.SDCMD_uiOpcode = SD_STOP_TRANSMISSION;
    sdcmdStop.SDCMD_uiFlag   = SD_RSP_SPI_R1B | SD_RSP_R1B | SD_CMD_AC; /*  停止命令                    */

    sdmsg.SDMSG_pucRdBuffer  = pucBuf;
    sdmsg.SDMSG_psddata      = &sddat;
    sdmsg.SDMSG_psdcmdCmd    = &sdcmd;
    sdmsg.SDMSG_psdcmdStop   = &sdcmdStop;
    sdmsg.SDMSG_pucWrtBuffer = LW_NULL;                                 /*  写缓冲为空                  */

    iError = API_SdCoreDevTransfer(psdcoredevice, &sdmsg, 1);
    if (iError != ERROR_NONE) {
        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "request error.\r\n");
        return  (PX_ERROR);
    }

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __sdMemBlkWrt
** 功能描述: SD记忆卡块设备写
** 输    入: psdblkdevice     块设备结构
**           pvWrtBuffer      写缓冲
**           ulStartBlk       起始地址
**           ulBlkCount       块数量
** 输    出: NONE
** 返    回: ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT __sdMemBlkWrt (__PSD_BLK_DEV   psdblkdevice,
                          VOID           *pvWrtBuffer,
                          ULONG           ulStartBlk,
                          ULONG           ulBlkCount)
{
    INT                iError;
    INT                iDevSta;
    PLW_SDCORE_DEVICE  psdcoredevice;
    UINT8              ucBlkLenBits;

    if (!psdblkdevice || !pvWrtBuffer) {
        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "parameter error.\r\n");
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    psdcoredevice = ((__PSD_BLK_DEV)psdblkdevice)->SDBLKDEV_pcoreDev;
    if (!psdcoredevice) {
        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "no core device member.\r\n");
        return  (PX_ERROR);
    }

    ucBlkLenBits = psdblkdevice->SDBLKDEV_ucWrBlkLenBits;

    iDevSta = API_SdCoreDevStaView(psdcoredevice);
    if (iDevSta != SD_DEVSTA_EXIST) {
        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "device does not exist.\r\n");
        return  (PX_ERROR);
    }

    if (COREDEV_IS_SD(psdcoredevice) && psdblkdevice->SDBLKDEV_bNeedReSelect) {
        iError = API_SdCoreDevSelect(psdcoredevice);                        /*  选择设备                */
        if (iError != ERROR_NONE) {
            SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "select device failed.\r\n");
            return  (PX_ERROR);
        }
    }

    if (psdblkdevice->SDBLKDEV_bNeedReSetBlkSize) {
        API_SdCoreDevSetBlkLenRaw(psdcoredevice, 1 << ucBlkLenBits);
    }

    if ((ulStartBlk + ulBlkCount) > psdblkdevice->SDBLKDEV_ulDiskNSector) {
        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "block number out of range.\r\n");
        iError = PX_ERROR;
        goto    __error_handle;
    }
    ulStartBlk += psdblkdevice->SDBLKDEV_ulSectorOff;

    if (psdblkdevice->SDBLKDEV_bNeedReCalcBlkCnt) {
        ulStartBlk = __sdMemBlkLogic2Phy(ulStartBlk, ucBlkLenBits);
        ulBlkCount = __sdMemBlkLogic2Phy(ulBlkCount, ucBlkLenBits);
    }

    /*
     * 块地址转换
     */
    if (!__SDMEM_BLKADDR(psdblkdevice)) {
        ulStartBlk <<= ucBlkLenBits;
    }

    if (ulBlkCount <= 1) {
        iError = __sdMemWrtSingleBlk(psdcoredevice, (UINT8 *)pvWrtBuffer, (UINT32)ulStartBlk);

    } else {
        iError = __sdMemWrtMultiBlk(psdcoredevice, (UINT8 *)pvWrtBuffer,
                                    (UINT32)ulStartBlk, (UINT32)ulBlkCount);
    }

__error_handle:
    if (COREDEV_IS_SD(psdcoredevice) && psdblkdevice->SDBLKDEV_bNeedReSelect) {
        API_SdCoreDevDeSelect(psdcoredevice);                           /*  取消设备                    */
    }

    return  (iError);
}
/*********************************************************************************************************
** 函数名称: __sdMemBlkRd
** 功能描述: SD记忆卡块设备读
** 输    入: psdblkdevice   块设备结构
**           pvRdBuffer     读缓冲
**           ulStartBlk     起始地址
**           ulBlkCount     块数量
** 输    出: NONE
** 返    回: ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT __sdMemBlkRd (__PSD_BLK_DEV   psdblkdevice,
                         VOID           *pvRdBuffer,
                         ULONG           ulStartBlk,
                         ULONG           ulBlkCount)
{
    INT                iError;
    INT                iDevSta;
    PLW_SDCORE_DEVICE  psdcoredevice;
    UINT8              ucBlkLenBits;

    if (!psdblkdevice || !pvRdBuffer) {
        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "parameter error.\r\n");
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    psdcoredevice = ((__PSD_BLK_DEV)psdblkdevice)->SDBLKDEV_pcoreDev;
    if (!psdcoredevice) {
        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "no core device.\r\n");
        return  (PX_ERROR);
    }

    ucBlkLenBits = psdblkdevice->SDBLKDEV_ucRdBlkLenBits;

    iDevSta = API_SdCoreDevStaView(psdcoredevice);
    if (iDevSta != SD_DEVSTA_EXIST) {
        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "device does not exist.\r\n");
        return  (PX_ERROR);
    }

    if (COREDEV_IS_SD(psdcoredevice) && psdblkdevice->SDBLKDEV_bNeedReSelect) {
        iError = API_SdCoreDevSelect(psdcoredevice);                    /*  选择设备                    */
        if (iError != ERROR_NONE) {
            SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "select device failed.\r\n");
            return  (PX_ERROR);
        }
    }

    if (psdblkdevice->SDBLKDEV_bNeedReSetBlkSize) {
        API_SdCoreDevSetBlkLenRaw(psdcoredevice, 1 << ucBlkLenBits);
    }

    if ((ulStartBlk + ulBlkCount) > psdblkdevice->SDBLKDEV_ulDiskNSector) {
        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "block number is out of range.\r\n");
        iError = PX_ERROR;
        goto    __error_handle;
    }
    ulStartBlk += psdblkdevice->SDBLKDEV_ulSectorOff;

    if (psdblkdevice->SDBLKDEV_bNeedReCalcBlkCnt) {
        ulStartBlk = __sdMemBlkLogic2Phy(ulStartBlk, ucBlkLenBits);
        ulBlkCount = __sdMemBlkLogic2Phy(ulBlkCount, ucBlkLenBits);
    }

    /*
     * 块地址转换
     */
    if (!__SDMEM_BLKADDR(psdblkdevice)) {
        ulStartBlk <<= ucBlkLenBits;
    }

    if (ulBlkCount <= 1) {
        iError = __sdMemRdSingleBlk(psdcoredevice, (UINT8 *)pvRdBuffer, (UINT32)ulStartBlk);

    } else {
        iError = __sdMemRdMultiBlk(psdcoredevice, (UINT8 *)pvRdBuffer,
                                   (UINT32)ulStartBlk, (UINT32)ulBlkCount);
    }

__error_handle:
    if (COREDEV_IS_SD(psdcoredevice) && psdblkdevice->SDBLKDEV_bNeedReSelect) {
        API_SdCoreDevDeSelect(psdcoredevice);                           /*  取消设备                    */
    }

    return  (iError);
}
/*********************************************************************************************************
** 函数名称: __sdMemIoctl
** 功能描述: SD记忆卡块设备IO控制
** 输    入: psdblkdevice   块设备结构
**           iCmd           控制命令
**           lArg           参数
** 输    出: NONE
** 返    回: ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT __sdMemIoctl (__PSD_BLK_DEV    psdblkdevice,
                         INT              iCmd,
                         LONG             lArg)
{
    INT           iDevSta;
    LW_SDDEV_CSD  sdcsd;
    LW_BLK_INFO  *pblkinfo;

    if (!psdblkdevice) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    iDevSta = API_SdCoreDevStaView(psdblkdevice->SDBLKDEV_pcoreDev);
    if (iDevSta != SD_DEVSTA_EXIST) {
        return  (PX_ERROR);
    }

    switch (iCmd) {

    case FIOSYNC:
    case FIODATASYNC:
    case FIOFLUSH:                                                      /*  将缓存回写到磁盘            */
    case FIOSYNCMETA:
    case FIOUNMOUNT:                                                    /*  卸载卷                      */
    case FIODISKINIT:                                                   /*  初始化设备                  */
    case FIODISKCHANGE:                                                 /*  磁盘媒质发生变化            */
        break;

    case LW_BLKD_GET_SECSIZE:
    case LW_BLKD_GET_BLKSIZE:
        *((ULONG *)lArg) = SD_MEM_DEFAULT_BLKSIZE;
        break;

    case LW_BLKD_GET_SECNUM:
        API_SdCoreDevCsdView(psdblkdevice->SDBLKDEV_pcoreDev, &sdcsd);
        *((ULONG *)lArg) = psdblkdevice->SDBLKDEV_ulDiskNSector;
        break;

    case LW_BLKD_CTRL_INFO:
        pblkinfo = (LW_BLK_INFO *)lArg;
        lib_memcpy(pblkinfo, &psdblkdevice->SDBLKDEV_blkinfo, sizeof(LW_BLK_INFO));
        break;

    case FIOWTIMEOUT:
    case FIORTIMEOUT:
        break;

    default:
        _ErrorHandle(ENOSYS);
        return  (PX_ERROR);
    }

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __sdMemBlkInfoFmt
** 功能描述: 格式化块设备信息
** 输    入: psdblkdevice  块设备结构
** 输    出: NONE
** 返    回: ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT __sdMemBlkInfoFmt (__PSD_BLK_DEV psdblkdevice)
{
    PLW_SDCORE_DEVICE  psdcoredevice;
    LW_SDDEV_CSD       sddevcsd;
    LW_SDDEV_CID       sddevcid;
    LW_BLK_INFO       *pblkinfo;

    UINT8              ucType;
    CPCHAR             pcTypeStr = "unknown";
    CPCHAR             pcVsnStr  = "unknown";

    psdcoredevice = psdblkdevice->SDBLKDEV_pcoreDev;
    pblkinfo      = &psdblkdevice->SDBLKDEV_blkinfo;
    
    pblkinfo->BLKI_uiType = LW_BLKD_CTRL_INFO_TYPE_SDMMC;

    API_SdCoreDevCsdView(psdcoredevice, &sddevcsd);
    API_SdCoreDevCidView(psdcoredevice, &sddevcid);
    API_SdCoreDevTypeView(psdcoredevice, &ucType);

    switch (ucType) {

    case SDDEV_TYPE_MMC:
        pcTypeStr = "MMC/eMMC";
        break;

    case SDDEV_TYPE_SDSC:
        pcTypeStr = "SDSC";
        break;

    case SDDEV_TYPE_SDHC:
        pcTypeStr = "SDHC";
        break;

    case SDDEV_TYPE_SDXC:
        pcTypeStr = "SDXC";
        break;

    default:
        break;
    }

    pcVsnStr = __sdMemProtVsnStr(ucType, sddevcsd.DEVCSD_ucStructure);

    snprintf(pblkinfo->BLKI_cSerial,
             LW_BLKD_CTRL_INFO_STR_SZ,
             "%08X",
             sddevcid.DEVCID_uiSerialNum);

    snprintf(pblkinfo->BLKI_cFirmware,
             LW_BLKD_CTRL_INFO_STR_SZ,
             "%d.%02d, v%d.%d",
             sddevcid.DEVCID_uiYear,
             sddevcid.DEVCID_ucMonth,
             sddevcid.DEVCID_ucProductVsn >> 4,
             sddevcid.DEVCID_ucProductVsn & 0xf);

    snprintf(pblkinfo->BLKI_cProduct,
             LW_BLKD_CTRL_INFO_STR_SZ,
             "%c%c%c%c%c %s memory card",
             __SD_CID_PNAME(0),
             __SD_CID_PNAME(1),
             __SD_CID_PNAME(2),
             __SD_CID_PNAME(3),
             __SD_CID_PNAME(4),
             pcTypeStr);

    snprintf(pblkinfo->BLKI_cMedia,
             LW_BLKD_CTRL_INFO_STR_SZ,
             "%s(%s), %d(R) %d(W)",
             pcTypeStr,
             pcVsnStr,
             1 << psdblkdevice->SDBLKDEV_ucRdBlkLenBits,
             1 << psdblkdevice->SDBLKDEV_ucWrBlkLenBits);

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __sdMemStatus
** 功能描述: SD记忆卡块设备状态函数
** 输    入: psdblkdevice  块设备结构
** 输    出: NONE
** 返    回: ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT __sdMemStatus (__PSD_BLK_DEV   psdblkdevice)
{
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __sdMemStatus
** 功能描述: SD记忆卡块设备复位
** 输    入: psdblkdevice  块设备结构
** 输    出: NONE
** 返    回: ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT __sdMemReset (__PSD_BLK_DEV    psdblkdevice)
{
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __sdMemProtVsnStr
** 功能描述: 获取卡的协议版本字符串
** 输    入: ucType   卡类型(MMC 或 SD)
**           ucVsn    CSD里面的结构标识
** 输    出: NONE
** 返    回: ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static CPCHAR __sdMemProtVsnStr (UINT8 ucType, UINT8 ucVsn)
{
    CPCHAR pcVsnStr = "v1.0";

    if (ucType != SDDEV_TYPE_MMC) {
        switch (ucVsn) {

        case CSD_STRUCT_VER_1_0:
            pcVsnStr = "v1.0";
            break;

        case CSD_STRUCT_VER_2_0:
            pcVsnStr = "v2.0";
            break;

        default:
            pcVsnStr = "v2.0";
            break;
        }

    } else {
        switch (ucVsn) {

        case MMC_VERSION_1_2:
            pcVsnStr = "v1.2";
            break;

        case MMC_VERSION_1_4:
            pcVsnStr = "v1.4";
            break;

        case MMC_VERSION_2_2:
            pcVsnStr = "v2.2";
            break;

        case MMC_VERSION_3:
            pcVsnStr = "v3.0";
            break;

        case MMC_VERSION_4:
            pcVsnStr = "v4.0";
            break;

        case MMC_VERSION_4_1:
            pcVsnStr = "v4.1";
            break;

        case MMC_VERSION_4_2:
            pcVsnStr = "v4.2";
            break;

        case MMC_VERSION_4_3:
            pcVsnStr = "v4.3";
            break;

        case MMC_VERSION_4_4:
            pcVsnStr = "v4.4";
            break;

        case MMC_VERSION_4_5:
            pcVsnStr = "v4.5";
            break;

        case MMC_VERSION_5_0:
            pcVsnStr = "v5.0";
            break;

        case MMC_VERSION_5_1:
            pcVsnStr = "v5.1";
            break;

        case MMC_VERSION_NEW:
            pcVsnStr = "v5.2 or later";
            break;

        default:
            pcVsnStr = "v1.2";
            break;
        }
    }

    return  (pcVsnStr);
}
/*********************************************************************************************************
** 函数名称: __sdMemBlkLogic2Phy
** 功能描述: 逻辑块数据到物理块数据转换
** 输    入: ulLogic      逻辑块数据
**           ucBlkLenBits 物理块大小(位数)
** 输    出: NONE
** 返    回: 物理块数据
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static ULONG __sdMemBlkLogic2Phy (ULONG ulLogic, UINT8 ucBlkLenBits)
{
    if (ucBlkLenBits > SD_MEM_DEFAULT_BLKSIZE_NBITS) {
        ulLogic >>= ucBlkLenBits - SD_MEM_DEFAULT_BLKSIZE_NBITS;

    } else {
        ulLogic <<= SD_MEM_DEFAULT_BLKSIZE_NBITS - ucBlkLenBits;
    }

    return  (ulLogic);
}
/*********************************************************************************************************
** 函数名称: __sdMemBlkPhy2Logic
** 功能描述: 物理块数据到逻辑块数据转换
** 输    入: ulPhy        物理块数据
**           ucBlkLenBits 物理块大小(位数)
** 输    出: NONE
** 返    回: 逻辑块数据
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static ULONG __sdMemBlkPhy2Logic (ULONG ulPhy, UINT8 ucBlkLenBits)
{
    if (ucBlkLenBits > SD_MEM_DEFAULT_BLKSIZE_NBITS) {
        ulPhy <<= ucBlkLenBits - SD_MEM_DEFAULT_BLKSIZE_NBITS;

    } else {
        ulPhy >>= SD_MEM_DEFAULT_BLKSIZE_NBITS - ucBlkLenBits;
    }

    return  (ulPhy);
}
/*********************************************************************************************************
** 函数名称: __sdMemExtBufAlloc
** 功能描述: 分配初始化使用的扩展缓冲区
** 输    入: NONE
** 输    出: NONE
** 返    回: 缓冲区指针
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static VOID  *__sdMemExtBufAlloc (VOID)
{
    VOID *pvExtBuf;

    pvExtBuf = API_CacheDmaMallocAlign(1024, 4);

    return  (pvExtBuf);
}
/*********************************************************************************************************
** 函数名称: __sdMemExtBufFree
** 功能描述: 释放扩展缓冲区
** 输    入: pvExtBuf  缓冲区指针
** 输    出: NONE
** 返    回: NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static VOID   __sdMemExtBufFree (VOID *pvExtBuf)
{
    API_CacheDmaFree(pvExtBuf);
}
/*********************************************************************************************************
** 函数名称: __sdMemSdSwCapGet
** 功能描述: 获得SD卡的SWITCH功能信息
** 输    入: psdcoredevice   核心设备对象
**           psdscr          SCR 信息
**           psdcsd          CSD 信息
**           psdswcap        保存获得的SWITCH功能信息
** 输    出: NONE
** 返    回: ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT __sdMemSdSwCapGet (PLW_SDCORE_DEVICE  psdcoredevice,
                              LW_SDDEV_SCR      *psdscr,
                              LW_SDDEV_CSD      *psdcsd,
                              LW_SDDEV_SW_CAP   *psdswcap)
{
    UINT8  *pucSwCap = psdcoredevice->COREDEV_pucExtBuf;
    INT     iRet;

    lib_bzero(psdswcap, sizeof(LW_SDDEV_SW_CAP));

    if (psdscr->DEVSCR_ucSdaVsn < SD_SCR_SPEC_VER_1) {
        return  (PX_ERROR);
    }

    if (!(psdcsd->DEVCSD_usCmdclass & CCC_SWITCH)) {
        return  (PX_ERROR);
    }

    iRet = API_SdCoreDevSwitchEx(psdcoredevice, 0, 0, 0, pucSwCap);
    if (iRet != ERROR_NONE) {
        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "get sd switch information error.\r\n");
        return  (PX_ERROR);
    }

    if (pucSwCap[13] & SD_SW_MODE_HIGH_SPEED) {
        psdswcap->DEVSWCAP_uiHsMaxDtr = 50000000;
    }

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __sdMemSdHsSwitch
** 功能描述: SD 卡高速模式切换
** 输    入: psdcoredevice   核心设备对象
**           psdswcap        SWITCH功能信息
** 输    出: NONE
** 返    回: ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT __sdMemSdHsSwitch (PLW_SDCORE_DEVICE  psdcoredevice, LW_SDDEV_SW_CAP   *psdswcap)
{
    INT     iRet;
    UINT8  *pucStatus = psdcoredevice->COREDEV_pucExtBuf;

    if (!psdswcap->DEVSWCAP_uiHsMaxDtr) {
        return  (PX_ERROR);
    }

    iRet = API_SdCoreDevSwitchEx(psdcoredevice, 1, 0, 1, pucStatus);
    if (iRet != ERROR_NONE) {
        return  (PX_ERROR);
    }

    if ((pucStatus[16] & 0xF) != 1) {
        return  (PX_ERROR);
    }

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __sdMemMmcFreqChange
** 功能描述: 设置 MMC 卡的总线频率
** 输    入: psdcoredevice   核心设备对象
**           psdcsd          已经解析后的 CSD
**           piCardCap       保存CARD的功能(速度和支持的位宽模式)
** 输    出: NONE
** 返    回: ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT __sdMemMmcFreqChange (PLW_SDCORE_DEVICE  psdcoredevice,
                                 LW_SDDEV_CSD      *psdcsd,
                                 INT               *piCardCap)
{
    UINT8  *pucExtCsd = psdcoredevice->COREDEV_pucExtBuf;
    CHAR    cCardType;
    INT     iError;
    INT     iCapab  = 0;

    if (psdcsd->DEVCSD_ucStructure < MMC_VERSION_4) {
        return  (ERROR_NONE);
    }

    iError = API_SdCoreDevSendExtCSD(psdcoredevice, pucExtCsd);
    if (iError) {
        return  (iError);
    }

    if (pucExtCsd[212] || pucExtCsd[213] || pucExtCsd[214] || pucExtCsd[215]) {
        COREDEV_HIGHSPEED_SET(psdcoredevice);
    }

    cCardType = pucExtCsd[196] & 0xff;

    /*
     * 这里的功能位标使用 sddrvm.h里面的定义,
     * 这样在后面可以直接使用位与的方式得到实际需要使用的功能
     * 对于 HS200/HS400 这两种模式目前只考虑支持 1.8v 电压
     */
    if (cCardType & MMC_HS_200MHZ_1_8V_IO) {
        iCapab |= SDHOST_CAP_HS200
               |  SDHOST_CAP_DATA_4BIT
               |  SDHOST_CAP_DATA_8BIT;
    }
    if (cCardType & MMC_HS_400MHZ_1_8V_IO) {
        iCapab |= SDHOST_CAP_HS400
               |  SDHOST_CAP_DATA_8BIT;
    }
    if (cCardType & MMC_HS_52MHZ_1_8V_3V_IO) {
        iCapab |= MMC_MODE_HS_52MHz_DDR_18_3V
               |  MMC_MODE_HS_52MHz
               |  MMC_MODE_HS
               |  SDHOST_CAP_DATA_8BIT_DDR
               |  SDHOST_CAP_DATA_4BIT_DDR
               |  SDHOST_CAP_DATA_4BIT
               |  SDHOST_CAP_DATA_8BIT;

    } else if (cCardType & MMC_HS_52MHZ) {
        iCapab |= MMC_MODE_HS_52MHz
               |  MMC_MODE_HS
               |  SDHOST_CAP_DATA_4BIT
               |  SDHOST_CAP_DATA_8BIT;
    }

    *piCardCap = iCapab;

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __sdMemMmcBusWidthChangeHS200
** 功能描述: 设置 MMC 卡的总线位宽 (HS200 模式)
** 输    入: psdcoredevice   核心设备对象
**           iCardCap        MMC卡支持的位宽功能
**           psddevextcsd    扩展CSD信息
** 输    出: NONE
** 返    回: ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT __sdMemMmcBusWidthChangeHS200 (PLW_SDCORE_DEVICE  psdcoredevice,
                                          INT                iCardCap,
                                          LW_SDDEV_EXT_CSD  *psddevextcsd)
{
    UINT puiExtCsdBits[] = {
        EXT_CSD_BUS_WIDTH_8,
        EXT_CSD_BUS_WIDTH_4,
    };
    UINT puiBusWidth[] = {
        SDBUS_WIDTH_8,
        SDBUS_WIDTH_4,
    };

    INT iError   = ERROR_NONE;
    INT iHostCap = 0;
    UINT uiIndex, uiBuswidth;

    /*
     *  1. 设置线宽(8线或者4线模式)
     */

    /*
     *  获取主控制器支持的属性
     */
    iError = API_SdmHostCapGet(psdcoredevice, &iHostCap);
    if (iError != ERROR_NONE) {
        return  (PX_ERROR);
    }

    if (!(iHostCap & (SDHOST_CAP_DATA_4BIT | SDHOST_CAP_DATA_8BIT))) {
        return  (PX_ERROR);
    }

    /*
     *  检查主控制器和设备是否都支持 HS200 模式
     */
    iCardCap = iCardCap & iHostCap;
    if (!(iCardCap & SDHOST_CAP_HS200)) {
        return  (PX_ERROR);
    }

    uiIndex = (iCardCap & SDHOST_CAP_DATA_8BIT) ? 0 : 1;

    /*
     *  控制器无法知晓 MMC 设备支持的线宽，因此需要依次尝试设置。
     */
    for (; uiIndex < 2; uiIndex++) {
        iError = API_SdCoreDevSwitch(psdcoredevice,
                                     EXT_CSD_CMD_SET_NORMAL,
                                     EXT_CSD_BUS_WIDTH,
                                     puiExtCsdBits[uiIndex]);
        if (iError == ERROR_NONE) {
            iError = __sdMemSwitchWait(psdcoredevice,
                                       psddevextcsd->DEVEXTCSD_uiCmd6Timeout,
                                       LW_TRUE);
        }

        if (iError) {
            continue;
        }

        uiBuswidth = puiBusWidth[uiIndex];

        iError = API_SdCoreDevCtl(psdcoredevice, SDBUS_CTRL_SETBUSWIDTH, uiBuswidth);
        if (!iError) {
            break;
        }
    }

    if (iError != ERROR_NONE) {
        SDCARD_DEBUG_MSGX(__ERRORMESSAGE_LEVEL, "hs200 switch to 4bit & 8bit buswidth both failed.\r\n");
        return  (PX_ERROR);
    }

    /*
     *  2. 切换至 HS200 模式，设置时钟
     */
    iError = API_SdCoreDevSwitch(psdcoredevice,
                                 EXT_CSD_CMD_SET_NORMAL,
                                 EXT_CSD_HS_TIMING,
                                 EXT_CSD_TIMING_HS200);
    if (iError == ERROR_NONE) {
        iError = API_SdCoreDevCtl(psdcoredevice, SDBUS_CTRL_SETCLK, SDARG_SETCLK_HS200);
    }

    if (iError != ERROR_NONE) {
        SDCARD_DEBUG_MSGX(__ERRORMESSAGE_LEVEL, "switch to hs200 mode failed.\r\n");
        return  (PX_ERROR);
    }

    /*
     *  3. 执行 HS200 TUNING
     */
    iError = API_SdCoreDevCtl(psdcoredevice, SDBUS_CTRL_TUNING_EXEC, SD_SEND_TUNING_BLOCK_HS200);
    if (iError != ERROR_NONE) {
        SDCARD_DEBUG_MSGX(__ERRORMESSAGE_LEVEL, "execute hs200 tuning failed.\r\n");
        return  (PX_ERROR);
    }

    /*
     *  4. 设置设备电压类别
     */
    iError = __sdMemMmcSelectPwrClass(psdcoredevice,
                                      iCardCap,
                                      puiExtCsdBits[uiIndex],
                                      psddevextcsd);

    __sdMemSwitchWait(psdcoredevice,
                      psddevextcsd->DEVEXTCSD_uiCmd6Timeout,
                      LW_TRUE);

    if (iError != ERROR_NONE) {
        SDCARD_DEBUG_MSGX(__ERRORMESSAGE_LEVEL,
                          "hs200 select power class to bus width %d error.\r\n",
                          (puiBusWidth == SDBUS_WIDTH_8) ? 8 : 4);
    }

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __sdMemMmcBusWidthChange
** 功能描述: 设置 MMC 卡的总线位宽
** 输    入: psdcoredevice   核心设备对象
**           iCardCap        MMC卡支持的位宽功能
**           psddevextcsd    扩展CSD信息
** 输    出: NONE
** 返    回: ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT __sdMemMmcBusWidthChange (PLW_SDCORE_DEVICE psdcoredevice,
                                     INT               iCardCap,
                                     LW_SDDEV_EXT_CSD *psddevextcsd)
{
    static UINT puiExtCsdBits[][2] = {
        {EXT_CSD_BUS_WIDTH_8, EXT_CSD_BUS_WIDTH_8_DDR},
        {EXT_CSD_BUS_WIDTH_4, EXT_CSD_BUS_WIDTH_4_DDR},
        {EXT_CSD_BUS_WIDTH_1, EXT_CSD_BUS_WIDTH_1},
    };
    static UINT puiBusWidth[][2] = {
        {SDBUS_WIDTH_8, SDBUS_WIDTH_8_DDR},
        {SDBUS_WIDTH_4, SDBUS_WIDTH_4_DDR},
        {SDBUS_WIDTH_1, SDBUS_WIDTH_1},
    };

    INT iHostCap = 0;
    INT iError   = ERROR_NONE;
    INT iWidth;
    INT iDdr;

    iError = API_SdmHostCapGet(psdcoredevice, &iHostCap);
    if (iError != ERROR_NONE) {
        return  (PX_ERROR);
    }

    if (!(iHostCap & SDHOST_CAP_DATA_4BIT) || (iHostCap & SDHOST_CAP_MMC_FORCE_1BIT)) {
        return  (ERROR_NONE);
    }

    iCardCap = iCardCap & iHostCap;

    if (iCardCap & MMC_MODE_HS) {
        iError = API_SdCoreDevSwitch(psdcoredevice,
                                     EXT_CSD_CMD_SET_NORMAL,
                                     EXT_CSD_HS_TIMING,
                                     EXT_CSD_TIMING_HS);
        if (iError == ERROR_NONE) {
            API_SdCoreDevCtl(psdcoredevice,
                             SDBUS_CTRL_SETCLK,
                             SDARG_SETCLK_MAX);
        }

    } else {
        iError = API_SdCoreDevSwitch(psdcoredevice,
                                     EXT_CSD_CMD_SET_NORMAL,
                                     EXT_CSD_HS_TIMING,
                                     EXT_CSD_TIMING_BC);
    }

    if (iError != ERROR_NONE) {
        SDCARD_DEBUG_MSGX(__ERRORMESSAGE_LEVEL, "warning: switch to %s mode failed.\r\n",
                          iCardCap & MMC_MODE_HS ? "high speed" : "full speed");
        return  (ERROR_NONE);
    }

   if ((iCardCap & SDHOST_CAP_DATA_8BIT_DDR) || (iCardCap & SDHOST_CAP_DATA_4BIT_DDR)) {
        iDdr = 1;

    } else {
        iDdr = 0;
    }

    if ((iCardCap & SDHOST_CAP_DATA_8BIT_DDR) || (iCardCap & SDHOST_CAP_DATA_8BIT)) {
        iWidth = 0;

    } else {
        iWidth = 1;
    }

    for (; iWidth < 3; iWidth++) {
        if (iWidth == 2) {
            iDdr = 0;
        }

        iError = __sdMemMmcSelectPwrClass(psdcoredevice,
                                          iCardCap,
                                          puiExtCsdBits[iWidth][0],
                                          psddevextcsd);

        __sdMemSwitchWait(psdcoredevice,
                          psddevextcsd->DEVEXTCSD_uiCmd6Timeout,
                          LW_TRUE);

        if (iError != ERROR_NONE) {
            SDCARD_DEBUG_MSGX(__ERRORMESSAGE_LEVEL,
                              "warning: select power class to bus width %d (SDR mode) error.\r\n",
                              iWidth == 0 ? 8 : iWidth == 1 ? 4 : 1);
        }

        iError = API_SdCoreDevSwitch(psdcoredevice,
                                     EXT_CSD_CMD_SET_NORMAL,
                                     EXT_CSD_BUS_WIDTH,
                                     puiExtCsdBits[iWidth][0]);

        if (iError == ERROR_NONE) {
            iError = __sdMemSwitchWait(psdcoredevice,
                                       psddevextcsd->DEVEXTCSD_uiCmd6Timeout,
                                       LW_TRUE);
        }

        if (iError == ERROR_NONE) {
            break;
        }
    }

    if ((iError == ERROR_NONE) && iDdr) {
        iError = __sdMemMmcSelectPwrClass(psdcoredevice,
                                          iCardCap,
                                          puiExtCsdBits[iWidth][1],
                                          psddevextcsd);
        if (iError != ERROR_NONE) {
            SDCARD_DEBUG_MSGX(__ERRORMESSAGE_LEVEL,
                              "warning: select power class to bus width %d (DDR mode) error.\r\n",
                              iWidth == 0 ? 8 : iWidth == 1 ? 4 : 1);

        } else {
            iError = API_SdCoreDevSwitch(psdcoredevice,
                                         EXT_CSD_CMD_SET_NORMAL,
                                         EXT_CSD_BUS_WIDTH,
                                         puiExtCsdBits[iWidth][1]);
        }
    }

    if (iError == ERROR_NONE) {
        iError = __sdMemSwitchWait(psdcoredevice,
                                   psddevextcsd->DEVEXTCSD_uiCmd6Timeout,
                                   LW_TRUE);
    }

    if ((iError != ERROR_NONE) || (iWidth > 2)) {
        iWidth = 2;
        iDdr   = 0;
    }

    API_SdCoreDevCtl(psdcoredevice, SDBUS_CTRL_SETBUSWIDTH, puiBusWidth[iWidth][iDdr]);

    SDCARD_DEBUG_MSGX(__PRINTMESSAGE_LEVEL, "MMC card work on %d bus width, %s mode.\r\n",
                      iWidth == 0 ? 8 : iWidth == 1 ? 4 : 1, iDdr ? "DDR" : "SDR");

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __sdMemMmcSelectPwrClass
** 功能描述: 设置 MMC 卡电源类别
** 输    入: psdcoredevice   核心设备对象
**           iCardCap        MMC卡支持的位宽功能
**           uiBusWidth      需要匹配的总线位宽
**           psddevextcsd    扩展CSD信息
** 输    出: NONE
** 返    回: ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT __sdMemMmcSelectPwrClass (PLW_SDCORE_DEVICE psdcoredevice,
                                     INT               iCardCap,
                                     UINT32            uiBusWidth,
                                     LW_SDDEV_EXT_CSD *psddevextcsd)
{
    UINT32  uiClock;
    UINT32  uiPwrClassVal;
    UINT32  uiVdd  = SD_VDD_32_33;
    INT     iError = ERROR_NONE;

    if (iCardCap & MMC_MODE_HS) {
        uiClock = SDARG_SETCLK_MAX;

    } else {
        uiClock = SDARG_SETCLK_NORMAL;
    }

    if (iCardCap & SDHOST_CAP_HS200) {
        uiClock = SDARG_SETCLK_HS200;
    }

    if (uiBusWidth == EXT_CSD_BUS_WIDTH_1) {
        return  (ERROR_NONE);
    }

    API_SdCoreDevCtl(psdcoredevice, SDBUS_CTRL_GETOCR, (LONG)&uiVdd);
    if (uiVdd & SD_VDD_165_195) {
        uiVdd = SD_VDD_165_195;
    } else {
        uiVdd = SD_VDD_32_33;
    }

    switch (uiVdd) {

    case SD_VDD_165_195:
        if (uiClock <= 26000000) {
            uiPwrClassVal = psddevextcsd->DEVEXTCSD_uiRawPwrCl_26_195;

        } else if (uiClock <= 52000000) {
            uiPwrClassVal = (uiBusWidth <= EXT_CSD_BUS_WIDTH_8)
                          ? psddevextcsd->DEVEXTCSD_uiRawPwrCl_52_195
                          : psddevextcsd->DEVEXTCSD_uiRawPwrCl_ddr_52_195;

        } else if (uiClock <= 200000000) {
            uiPwrClassVal = psddevextcsd->DEVEXTCSD_uiRawPwrCl_200_195;
        }
        break;

    case SD_VDD_27_28:
    case SD_VDD_28_29:
    case SD_VDD_29_30:
    case SD_VDD_30_31:
    case SD_VDD_31_32:
    case SD_VDD_32_33:
    case SD_VDD_33_34:
    case SD_VDD_34_35:
    case SD_VDD_35_36:
        if (uiClock <= 26000000) {
            uiPwrClassVal = psddevextcsd->DEVEXTCSD_uiRawPwrCl_26_360;

        } else if (uiClock <= 52000000) {
            uiPwrClassVal = (uiBusWidth <= EXT_CSD_BUS_WIDTH_8)
                          ? psddevextcsd->DEVEXTCSD_uiRawPwrCl_52_360
                          : psddevextcsd->DEVEXTCSD_uiRawPwrCl_ddr_52_360;

        } else if (uiClock <= 200000000) {
            uiPwrClassVal = psddevextcsd->DEVEXTCSD_uiRawPwrCl_200_360;
        }
        break;

    default:
        return  (PX_ERROR);
    }

    if (uiBusWidth & (EXT_CSD_BUS_WIDTH_8 | EXT_CSD_BUS_WIDTH_8_DDR)) {
        uiPwrClassVal = (uiPwrClassVal & EXT_CSD_PWR_CL_8BIT_MASK) >> EXT_CSD_PWR_CL_8BIT_SHIFT;
    } else {
        uiPwrClassVal = (uiPwrClassVal & EXT_CSD_PWR_CL_4BIT_MASK) >> EXT_CSD_PWR_CL_4BIT_SHIFT;
    }

    if (uiPwrClassVal > 0) {
        iError = API_SdCoreDevSwitch(psdcoredevice,
                                     EXT_CSD_CMD_SET_NORMAL,
                                     EXT_CSD_POWER_CLASS,
                                     uiPwrClassVal);
    }

    return  (iError);
}
/*********************************************************************************************************
** 函数名称: __sdMemSwitchWait
** 功能描述: 使用 SWITCH 命令后等待设备操作完成
** 输    入: psdcoredevice   核心设备对象
**           uiTimeout       超时时间(单位为毫秒)
**           bSendStatus     是否查询卡状态
** 输    出: NONE
** 返    回: ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static INT __sdMemSwitchWait (PLW_SDCORE_DEVICE psdcoredevice,
                              UINT              uiTimeout,
                              BOOL              bSendStatus)
{
    UINT    uiStatus;
    INT64   i64Expire;
    INT     iError;

    if (!bSendStatus) {
        API_TimeMSleep(uiTimeout);
        return  (ERROR_NONE);
    }

    i64Expire = API_TimeGet64() + LW_MSECOND_TO_TICK_1(uiTimeout);

    for (;;) {
        iError = API_SdCoreDevGetStatus(psdcoredevice, &uiStatus);
        if (iError != ERROR_NONE) {
            break;
        }

        if ((uiStatus & __SD_CARD_STATUS_MSK) != __SD_CARD_STATUS_PRG) {
            if (uiStatus & __SD_CARD_STATUS_SWITCH_ERR) {
                SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "wait switch status error.\r\n");
                iError = PX_ERROR;
            }
            break;
        }

        if (API_TimeGet64() > i64Expire) {
            SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "wait switch done timeout.\r\n");
            return  (PX_ERROR);
        }
    }

    return  (iError);
}

#endif                                                                  /*  (LW_CFG_DEVICE_EN > 0)      */
                                                                        /*  (LW_CFG_SDCARD_EN > 0)      */
/*********************************************************************************************************
  END
*********************************************************************************************************/

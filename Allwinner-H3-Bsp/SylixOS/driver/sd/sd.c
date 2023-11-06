#include "sd.h"


static MMC_REG * mmc0_reg = (MMC_REG *)MMC0_BASE_ADDR;

static SD_HOST sdhost;
static void *sdmhost = NULL;
static LW_SD_FUNCS sdfuncs;
static SD_CALLBACK sdcallback;
static void *sdarg;

static void sdLibInit (void)
{
    API_SdmLibInit();
    API_SdMemDrvInstall();
}
static int sdHwInit ()
{
    // 硬件初始化
    Mmc0Init();

    return 0;
}
static int sdIoctl (PLW_SD_ADAPTER psdadapter,
                    int cmd,
                    long arg)
{
    switch (cmd)
    {
        case SDBUS_CTRL_POWEROFF:
            // TODO:关闭电源
            break;
        case SDBUS_CTRL_POWERUP:
        case SDBUS_CTRL_POWERON:
            // TODO:使能电源
            break;
        case SDBUS_CTRL_SETBUSWIDTH:
            // TODO:设置线宽
            Mmc0BusWidthSet((int)arg);
            break;
        case SDBUS_CTRL_SETCLK:
            // TODO:设置时钟频率
            Mmc0ClockSet((int)arg);
            break;
        case SDBUS_CTRL_DELAYCLK:
            break;
        case SDBUS_CTRL_GETOCR:
            // TODO:获取支持的电压情况
            *(UINT32 *)(arg) = SD_VDD_30_31 | SD_VDD_31_32 | SD_VDD_32_33 | SD_VDD_33_34;
            break;
        default:
            return -1;
    }
    return 0;
}
static int sdTransfer (PLW_SD_ADAPTER psdadapter,
                       PLW_SD_DEVICE  psddevice,
                       PLW_SD_MESSAGE psdmsg,
                       int            num)
{
    int ret;
    while (num--) {
        ret = Mmc0Xfer(psdmsg);
        if (ret) {
            return -1;
        }
        psdmsg++;
    }

    return 0;
}
static int sdCallBackInstall (SD_HOST     *sdhost,
                              int          type,
                              SD_CALLBACK  callback,
                              void        *arg)
{

    if (type == SDHOST_CALLBACK_CHECK_DEV) {
        sdcallback = callback;
        sdarg = arg;
    }

    return 0;
}

static int sdCallBackUnInstall (SD_HOST *sdhost, int type)
{

    if (type == SDHOST_CALLBACK_CHECK_DEV) {
        sdcallback = NULL;
        sdarg = NULL;
    }

    return 0;
}
static int sdDataInit ()
{
    sdfuncs.SDFUNC_pfuncMasterCtl  = sdIoctl;
    sdfuncs.SDFUNC_pfuncMasterXfer = sdTransfer;

    sdhost.SDHOST_cpcName                = "/bus/sd/0";
    sdhost.SDHOST_iType                  = SDHOST_TYPE_SD;
    sdhost.SDHOST_pfuncCallbackInstall   = sdCallBackInstall;
    sdhost.SDHOST_pfuncCallbackUnInstall = sdCallBackUnInstall;

    return 0;
}

static int Mmc0UpdateClk(void)
{
    UINT32 cmd;
    unsigned long expire = API_TimeGet() + LW_MSECOND_TO_TICK_1(2000);

    cmd = CMDR_START | CMDR_CHANGE_CLK | CMDR_WAIT_PRE_OVER;
    writel(cmd, &mmc0_reg->SD_CMDR);

    while(readl(&mmc0_reg->SD_CMDR) & CMDR_START)
    {
        if(API_TimeGet() > expire)
            return -1;
    }

    writel(readl(&mmc0_reg->SD_RISR), &mmc0_reg->SD_RISR);

    return 0;

}

static int Mmc0TransDataByCpu(LW_SD_MESSAGE *mrq)
{
    UINT32 raw_value;
    LW_SD_DATA *data = mrq->SDMSG_psddata;
    const int reading = (data->SDDAT_uiFlags & SD_DAT_READ);
    const UINT32 status_bit = reading ? STAR_FIFO_EMPTY : STAR_FIFO_FULL;

    unsigned i;
    UINT32 *buff = (UINT32 *)(reading ? mrq->SDMSG_pucRdBuffer : mrq->SDMSG_pucWrtBuffer);
    UINT32 word_cnt = (data->SDDAT_uiBlkSize * data->SDDAT_uiBlkNum) >> 2;
    UINT32 timeout_msecs = word_cnt >> 6;
    UINT32 status;

    if (timeout_msecs < 2000)
        timeout_msecs = 2000;

    /* Always read / write data through the CPU */
    raw_value = readl(&mmc0_reg->SD_GCTL);
    raw_value |= GCTRL_ACCESS_BY_AHB;
    writel(raw_value, &mmc0_reg->SD_GCTL);

    UINT32 expire = API_TimeGet() + LW_MSECOND_TO_TICK_1(timeout_msecs);
    for (i = 0; i < word_cnt;)
    {
        unsigned int in_fifo = 0;

        while ((status = readl(&mmc0_reg->SD_STAR)) & status_bit)
        {
            //_PrintFormat("status: %x\r\n", status);
            if (API_TimeGet() > expire)
                return -1;
        }

        if (!reading)
        {
            writel(buff[i++], &mmc0_reg->SD_FIFO);
            continue;
        }

        in_fifo = MMC_STATUS_FIFO_LEVEL(status);
        if (in_fifo == 0 && (status & STAR_FIFO_FULL))
            in_fifo = 32;
        for (; in_fifo > 0; in_fifo--)
            buff[i++] = readl_relaxed(&mmc0_reg->SD_FIFO);
        KN_SMP_MB();
    }

    return 0;
}

static int Mmc0RINTWait(u32 timeout_msecs, u32 done_bit, const char *what)
{
    UINT32 status;
    UINT32 expire = API_TimeGet() + LW_MSECOND_TO_TICK_1(timeout_msecs);

    do
    {
        status = readl(&mmc0_reg->SD_RISR);
        usleep(1000);

        if ((API_TimeGet() > expire) || (status & RISR_ERROR_BITS))
        {
            _PrintFormat("%s timeout %x\r\n", what,(status & RISR_ERROR_BITS));
            writel(status & RISR_ERROR_BITS, &mmc0_reg->SD_RISR);
            return -ETIMEDOUT;
        }
    }
    while (!(status & done_bit));

    writel(done_bit, &mmc0_reg->SD_RISR);
    return 0;
}

void Mmc0Init(void)
{
    CcuMmc0BusGateEnable(1);
    CcuMmc0ClockSet(24000000);

    writel(GCTL_RST, &mmc0_reg->SD_GCTL);

    UINT32 raw_value = readl(&mmc0_reg->SD_NTSR);
    raw_value |= NTSR_NEWTIMING_MODE;
    writel(raw_value, &mmc0_reg->SD_NTSR);

    raw_value = readl(&mmc0_reg->SD_GCTL);
    raw_value &= ~GCTL_DDR_MODE;
    writel(raw_value, &mmc0_reg->SD_GCTL);
}

void Mmc0BusWidthSet(int width)
{
    if(width != SDARG_SETBUSWIDTH_8 &&
       width != SDARG_SETBUSWIDTH_4 &&
       width != SDARG_SETBUSWIDTH_1)
    return;

    writel(width, &mmc0_reg->SD_BWDR);
}

int Mmc0ClockSet(int hz)
{

    UINT32 raw_value = readl(&mmc0_reg->SD_CKCR);

    raw_value &= ~CKCR_ON;
    writel(raw_value, &mmc0_reg->SD_CKCR);

    if(Mmc0UpdateClk() < 0)
        goto error;
    CcuMmc0ClockSet(hz);

    raw_value &= ~CKCR_DIVIDER_MASK;
    writel(raw_value, &mmc0_reg->SD_CKCR);

    raw_value |= CKCR_ON;
    writel(raw_value, &mmc0_reg->SD_CKCR);
    if(Mmc0UpdateClk() < 0)
            goto error;
    return 0;

error:
    return -1;
}

int Mmc0Xfer(LW_SD_MESSAGE *mrq)
{
    UINT32 reg_cmd = CMDR_START;
    UINT32 timeout_msecs;

    int error = 0;

    UINT32 status = 0;
    UINT32 byte_cnt = 0;

    LW_SD_DATA *data = mrq->SDMSG_psddata;
    LW_SD_COMMAND *cmd = mrq->SDMSG_psdcmdCmd;

    if (cmd->SDCMD_uiOpcode == 12)
        return 0;


    if (!cmd->SDCMD_uiOpcode)
        reg_cmd |= CMDR_SEND_INIT_SEQ;
    if (cmd->SDCMD_uiFlag & SD_RSP_PRESENT)
        reg_cmd |= CMDR_RESP_RCV;
    if (cmd->SDCMD_uiFlag & SD_RSP_136)
        reg_cmd |= CMDR_LONG_RESP;
    if (cmd->SDCMD_uiFlag & SD_RSP_CRC)
        reg_cmd |= CMDR_CHK_RESP_CRC;

    writel(cmd->SDCMD_uiArg, &mmc0_reg->SD_CAGR);

    if (data)
    {
        byte_cnt = data->SDDAT_uiBlkSize * data->SDDAT_uiBlkNum;
        reg_cmd |= (CMDR_DATA_TRANS | CMDR_WAIT_PRE_OVER);
        if (data->SDDAT_uiFlags & SD_DAT_WRITE)
        {
            reg_cmd |= CMDR_TRANS_DIR_W;
        }
        else
        {
            reg_cmd &= (~CMDR_TRANS_DIR_W);
        }

        if (data->SDDAT_uiBlkNum > 1)
            reg_cmd |= CMDR_AUTO_STOP_CMD;
        writel(data->SDDAT_uiBlkSize, &mmc0_reg->SD_BKSR);
        writel(byte_cnt, &mmc0_reg->SD_BYCR);
    }

    if (!data)
    {
        writel(reg_cmd | cmd->SDCMD_uiOpcode, &mmc0_reg->SD_CMDR);
        error = Mmc0RINTWait(1000, RISR_CMD_DONE, "cmd");
    }

    if (data)
    {
        int ret = 0;
        writel(reg_cmd | cmd->SDCMD_uiOpcode, &mmc0_reg->SD_CMDR);
        ret = Mmc0TransDataByCpu(mrq);
        if (ret)
        {
            error = readl(&mmc0_reg->SD_RISR) & RISR_ERROR_BITS;
            _PrintFormat("error %d\r\n", error);
            error = -ETIMEDOUT;
            _PrintFormat("mmc_trans_data_by_cpu fail\r\n");
            goto out;
        }
    }
    

    if (error)
        goto out;

    if (data)
    {
        timeout_msecs = 1000;
        error = Mmc0RINTWait(timeout_msecs,
                             data->SDDAT_uiBlkNum <= 1 ? RISR_CMD_DONE : RISR_DATA_TRANS_DONE,
                             "data");
        if (error)
            goto out;
    }

    if (cmd->SDCMD_uiFlag & SD_RSP_BUSY)
    {
        timeout_msecs = 2000;
        unsigned long expire = API_TimeGet() + LW_MSECOND_TO_TICK_1(timeout_msecs);

        do
        {
            status = readl(&mmc0_reg->SD_STAR);
            if (API_TimeGet() > expire)
            {
                _PrintFormat("busy timeout\r\n");
                error = -ETIMEDOUT;
                goto out;
            }
        }
        while (status & STAR_CARD_BUSY);
    }

    if (cmd->SDCMD_uiFlag & SD_RSP_136)
    {
        cmd->SDCMD_uiResp[0] = readl(&mmc0_reg->SD_RESP3);
        cmd->SDCMD_uiResp[1] = readl(&mmc0_reg->SD_RESP2);
        cmd->SDCMD_uiResp[2] = readl(&mmc0_reg->SD_RESP1);
        cmd->SDCMD_uiResp[3] = readl(&mmc0_reg->SD_RESP0);
    }
    else
    {
        cmd->SDCMD_uiResp[0] = readl(&mmc0_reg->SD_RESP0);
    }
out:
    if (error < 0)
    {
        writel(GCTL_RST, &mmc0_reg->SD_GCTL);
        writel(0xffffffff, &mmc0_reg->SD_RISR);
        Mmc0UpdateClk();
    }
    return error;
}

int sdDevCreate (void)
{

    sdLibInit();
    sdDataInit();
    sdHwInit();
    API_SdAdapterCreate("/bus/sd/0", &sdfuncs);
    sdmhost = API_SdmHostRegister(&sdhost);

    API_SdmEventNotify(sdmhost, SDM_EVENT_BOOT_DEV_INSERT);
    return 0;
}

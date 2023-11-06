#ifndef SD_H
#define SD_H

#define __SYLIXOS_KERNEL
#include <SylixOS.h>
#include <linux/compat.h>
#include <system/device/sdcard/core/sdstd.h>
#include <system/device/sd/sdBus.h>

#include "../ccu/ccu.h"

#define MMC_STATUS_FIFO_LEVEL(reg)      (((reg) >> 17) & 0x3fff)

#define MMC0_BASE_ADDR                  0x01C0F000

#define GCTL_DDR_MODE                   (0x1 << 10)
#define GCTL_SOFT_RST                   (0x1 << 0)
#define GCTL_FIFO_RST                   (0x1 << 1)
#define GCTL_DMA_RST                    (0x1 << 2)
#define GCTL_RST                        (GCTL_SOFT_RST | GCTL_FIFO_RST | GCTL_DMA_RST)
#define GCTRL_ACCESS_BY_AHB             (0x1 << 31)

#define NTSR_NEWTIMING_MODE             (0x1 << 31)

#define BWDR_BIT_WIDTH_1                0
#define BWDR_BIT_WIDTH_4                1
#define BWDR_BIT_WIDTH_8                4

#define CMDR_START                      (0x1 << 31)
#define CMDR_CHANGE_CLK                 (0x1 << 21)
#define CMDR_WAIT_PRE_OVER              (0x1 << 13)
#define CMDR_SEND_INIT_SEQ              (0x1 << 15) //  Send initialization sequence before sending this command
#define CMDR_RESP_RCV                   (0x1 << 6)
#define CMDR_LONG_RESP                  (0x1 << 7)
#define CMDR_CHK_RESP_CRC               (0x1 << 8)  //  Check Response CRC
#define CMDR_DATA_TRANS                 (0x1 << 9)
#define CMDR_TRANS_DIR_W                (0x1 << 10) //  Set transfer direction to write
#define CMDR_AUTO_STOP_CMD              (0x1 << 12) //  Send stop command automatically at end of data transfer

#define CKCR_ON                         (0x1 << 16)
#define CKCR_DIVIDER_MASK               0xFF

#define STAR_FIFO_EMPTY                 (0x1 << 2)
#define STAR_FIFO_FULL                  (0x1 << 3)
#define STAR_CARD_BUSY                  (0x1 << 9)

#define RISR_RESP_ERROR                 (0x1 << 1)
#define RISR_CMD_DONE                   (0x1 << 2)
#define RISR_DATA_TRANS_DONE            (0x1 << 3)
#define RISR_RESP_CRC_ERROR             (0x1 << 6)
#define RISR_DATA_CRC_ERROR             (0x1 << 7)
#define RISR_RESP_TIMEOUT               (0x1 << 8)
#define RISR_DATA_TIMEOUT               (0x1 << 9)
#define RISR_VOTAGE_CHAGE_DONE          (0x1 << 10)
#define RISR_FIFO_RUN_ERROR             (0x1 << 11)
#define RISR_HARDWARE_LOCKED            (0x1 << 12)
#define RISR_START_ERROR                (0x1 << 13)
#define RISR_END_ERROR                  (0x1 << 15)


#define RISR_ERROR_BITS                 (RISR_RESP_ERROR | RISR_RESP_CRC_ERROR | RISR_DATA_CRC_ERROR |     \
                                         RISR_RESP_TIMEOUT | RISR_DATA_TIMEOUT | RISR_VOTAGE_CHAGE_DONE |  \
                                         RISR_FIFO_RUN_ERROR | RISR_HARDWARE_LOCKED | RISR_START_ERROR |   \
                                         RISR_END_ERROR)




struct sd_chan {
    SD_HOST      sdhost;
    void        *sdmhost;
    LW_SD_FUNCS  sdfuncs;
    SD_CALLBACK  sdcallback;
    void        *sdarg;
};

typedef struct mmc_reg{
    UINT32 SD_GCTL;         //  Control register
    UINT32 SD_CKCR;         //  Clock Control register
    UINT32 SD_TMOR;         //  Time out register
    UINT32 SD_BWDR;         //  Bus width register
    UINT32 SD_BKSR;         //  Block size register
    UINT32 SD_BYCR;         //  Byte count register
    UINT32 SD_CMDR;         //  Command register
    UINT32 SD_CAGR;         //  Command argument register
    UINT32 SD_RESP0;        //  Response 0 register
    UINT32 SD_RESP1;        //  Response 1 register
    UINT32 SD_RESP2;        //  Response 2 register
    UINT32 SD_RESP3;        //  Response 3 register
    UINT32 SD_IMKR;         //  Interrupt mask register
    UINT32 SD_MISR;         //  Masked interrupt status register
    UINT32 SD_RISR;         //  Raw interrupt status register
    UINT32 SD_STAR;         //  Status register
    UINT32 SD_FWLR;         //  FIFO Water Level register
    UINT32 SD_FUNS;         //  FIFO Function Select register
    UINT32 SD_RESERVE0[4];
    UINT32 SD_A12A;         //  Auto command 12 argument
    UINT32 SD_NTSR;         //  SD NewTiming Set register
    UINT32 SD_SDBG[6];      //  SD NewTiming Set Debug Register
    UINT32 SD_HWRST[2];     //  Hardware Reset Register
    UINT32 SD_DMAC;         //  BUS Mode Control
    UINT32 SD_DLBA;         //  Descriptor List Base Address
    UINT32 SD_IDST;         //  DMAC Status
    UINT32 SD_IDIE[29];     //  DMAC Interrupt Enable
    UINT32 SD_THLDC[3];     //  Card Threshold Control register
    UINT32 SD_DSBD;         //  eMMC4.41 DDR Start Bit Detection Control
    UINT32 SD_RES_CRC;      //  CRC status from card/eMMC in write operation
    UINT32 SD_DATA7_CRC;    //  CRC Data7 from card/eMMC
    UINT32 SD_DATA6_CRC;    //  CRC Data6 from card/eMMC
    UINT32 SD_DATA5_CRC;    //  CRC Data5 from card/eMMC
    UINT32 SD_DATA4_CRC;    //  CRC Data4 from card/eMMC
    UINT32 SD_DATA3_CRC;    //  CRC Data3 from card/eMMC
    UINT32 SD_DATA2_CRC;    //  CRC Data2 from card/eMMC
    UINT32 SD_DATA1_CRC;    //  CRC Data1 from card/eMMC
    UINT32 SD_DATA0_CRC;    //  CRC Data0 from card/eMMC
    UINT32 SD_CRC_STA;     //  Response CRC from card/eMMC
    UINT32 SD_RESERVE[50];
    UINT32 SD_FIFO;         //  Read/Write FIFO
}MMC_REG;

void Mmc0Init(void);

void Mmc0BusWidthSet(int width);

int Mmc0ClockSet(int hz);

int Mmc0Xfer(LW_SD_MESSAGE *mrq);

int sdDevCreate(void);



#endif //   SD_H

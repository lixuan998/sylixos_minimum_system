#ifndef CCU_H
#define CCU_H

#define  __SYLIXOS_KERNEL
#include <SylixOS.h>
#include <linux/compat.h>

#define CCU_BASE_ADDR             (0x01C20000)
#define CCU_BUS_CLK_GATE_REG_0    (CCU_BASE_ADDR + 0x0060)
#define CCU_SDMMC0_CLK_REG        (CCU_BASE_ADDR + 0x0088)

void CcuMmc0BusGateEnable(int enable);
int CcuMmc0ClockSet(unsigned int hz);

#endif //   CCU_H

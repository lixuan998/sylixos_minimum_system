#include "ccu.h"

void CcuMmc0BusGateEnable (int enable)
{
    u32 val;
    val = readl(CCU_BUS_CLK_GATE_REG_0);

    if (enable)
    {
        writel(val | (1 << 8), CCU_BUS_CLK_GATE_REG_0);
    }
    else
    {
        // smhc0 gate mask
        writel(val & (~(1 << 8)), CCU_BUS_CLK_GATE_REG_0);
    }
}

int CcuMmc0ClockSet (unsigned int hz)
{
    if(hz >= 24000000) hz = 24000000;
    unsigned int pll, pll_hz, div, n;

    if (hz <= 24000000)
    {
        pll = (0 << 24); // select HOSC
        pll_hz = 24000000;
    } else
    {
        pll = (2 << 24); // select PLL_PERI(2X)
        pll_hz = 1200000000; // PLL_PERI(2X) default 1.2GHz
    }

    div = pll_hz / hz;
    if (pll_hz % hz)
        div++;

    n = 0;
    while (div > 16) {
        n++;
        div = (div + 1) / 2;
    }

    if (n > 3) {
        _PrintFormat("mmc0 error cannot set clock to %d\n", hz);
        return -1;
    }

    writel((1 << 31) | pll | (n << 8) | (div << 0), CCU_SDMMC0_CLK_REG);

    return 0;
}

#include "timer.h"

VOID  timerStart (INT32  iNum, UINT32  uiHZ)
{
    UINT32  uiCount;
    UINT32  uiIntvOffset;
    UINT32  uiCtrlOffset;

    if (0 == iNum) {
        uiIntvOffset = TIMER0_INTV;
        uiCtrlOffset = TIMER0_CTRL;
    } else if (1 == iNum) {
        uiIntvOffset = TIMER1_INTV;
        uiCtrlOffset = TIMER1_CTRL;
    } else {
        return ;
    }

    uiCount  = TIMER_FREQ;
    uiCount /= uiHZ;
    writel(uiCount, TIMER_BASE + uiIntvOffset);
    writel(BIT(1) | BIT(2) | BIT(4), TIMER_BASE + uiCtrlOffset);
    while ((readl(TIMER_BASE + uiCtrlOffset) >> 1) & 0x01);
    writel(readl(TIMER_BASE + uiCtrlOffset) | BIT(0), TIMER_BASE + uiCtrlOffset);
    writel(readl(TIMER_BASE + TIMER_IRQ_EN) | BIT(iNum), TIMER_BASE + TIMER_IRQ_EN);
}

VOID  timerIntClear (INT32  iNum)
{
    if ((0 != iNum) && (1 != iNum))
        return ;

    writel(readl(TIMER_BASE + TIMER_IRQ_STA) | BIT(iNum), TIMER_BASE + TIMER_IRQ_STA);
}

BOOL  timerIsIntPending (INT32  iNum)
{
    if ((0 != iNum) && (1 != iNum))
        return  FALSE;

    return  (readl(TIMER_BASE + TIMER_IRQ_STA) & BIT(iNum)) ? TRUE : FALSE;
}


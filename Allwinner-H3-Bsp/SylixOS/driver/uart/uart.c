#include "uart.h"

VOID  uartPutChar (CHAR  cChar)
{
    //  若 FIFO 不满就填入数据，否则等待
    while (!(readl(UART0_BASE + USR) & BIT(1)));

    writel(cChar, UART0_BASE + THR);
}

VOID  uartPutMsg (CPCHAR  cpcMsg)
{
    CHAR  cChar;

    if (!cpcMsg) {
        return;
    }

    while ((cChar = *cpcMsg) != '\0') {
        uartPutChar(cChar);
        cpcMsg++;
    }
}

VOID  uartGetChar (PCHAR  pcChar)
{
    *pcChar = readb(UART0_BASE + RBR);
}

BOOL  uartIsRcvFifoEmpty (VOID)
{
    return  (!(readl(UART0_BASE + USR) & BIT(3)) ? TRUE : FALSE);
}

VOID  uartEnableRcvInt (VOID)
{
    writel(readl(UART0_BASE + LCR) & ~BIT(7), UART0_BASE + LCR);
    writel(BIT(0), UART0_BASE + IER);
}


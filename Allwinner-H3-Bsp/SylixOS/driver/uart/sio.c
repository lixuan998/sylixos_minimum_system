#include "uart.h"

static SIO_CHAN  uartSioChan;
static INT (*uartGetTxChar)(PVOID  pArg, PCHAR  pcChar);
static INT (*uartPutRcvChar)(PVOID  pArg, CHAR  cChar);
static PVOID  pTxArg;
static PVOID  pRxArg;

static INT  uartSioIoctl (SIO_CHAN  *pSioChan, INT  iCmd, PVOID  pArg)
{
    switch (iCmd)
    {
        case SIO_BAUD_SET:
            break;

        case SIO_BAUD_GET:
            *((LONG *)pArg) = 115200;
            break;

        case SIO_HW_OPTS_SET:
            break;

        case SIO_HW_OPTS_GET:
            *(LONG *)pArg = 0;
            break;

        case SIO_OPEN:
            uartEnableRcvInt();
            API_InterVectorEnable(UART_VECTOR(0));
            break;

        case SIO_HUP:
            break;

        default:
            _ErrorHandle(ENOSYS);
            return  (ENOSYS);
    }

    return  (ERROR_NONE);
}

static INT  uartStartup (SIO_CHAN  *pSioChan)
{
    CHAR  cChar;

    while (!uartGetTxChar(pTxArg, &cChar))
    {
        uartPutChar(cChar);
    }

    return  (ERROR_NONE);
}

static INT  uartSioCbInstall (SIO_CHAN  *pSioChan, INT  iCallbackType, VX_SIO_CALLBACK  callbackRoute, PVOID  pvCallbackArg)
{
    switch (iCallbackType)
    {
        case SIO_CALLBACK_GET_TX_CHAR:
            uartGetTxChar = (INT (*)(PVOID, PCHAR))callbackRoute;
            pTxArg = pvCallbackArg;

            break;

        case SIO_CALLBACK_PUT_RCV_CHAR:
            uartPutRcvChar = (INT (*)(PVOID, CHAR))callbackRoute;
            pRxArg = pvCallbackArg;

            break;

        default:
            _ErrorHandle(ENOSYS);
            return  (PX_ERROR);
    }

    return  (ERROR_NONE);
}

static SIO_DRV_FUNCS  uartSioDrvFunc = {
    .ioctl = uartSioIoctl,
    .txStartup = uartStartup,
    .callbackInstall = uartSioCbInstall,
};

static irqreturn_t  uartSioIsr (PVOID  pArg, ULONG  ulVector)
{
    CHAR  cChar;
    while (!uartIsRcvFifoEmpty()) {
        uartGetChar(&cChar);

        uartPutRcvChar(pRxArg, cChar);
    }

    return  (LW_IRQ_HANDLED);
}

SIO_CHAN  *uartSioChanCreate (VOID)
{
    uartSioChan.pDrvFuncs = &uartSioDrvFunc;

    API_InterVectorConnect(UART_VECTOR(0),
                           (PINT_SVR_ROUTINE)uartSioIsr,
                           NULL,
                           "uart_isr");

    return  &uartSioChan;
}

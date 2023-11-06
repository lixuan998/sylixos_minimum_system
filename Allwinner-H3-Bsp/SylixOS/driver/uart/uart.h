#ifndef UART_H
#define UART_H

#define  __SYLIXOS_KERNEL
#include <SylixOS.h>
#include <linux/compat.h>

/*********************************************************************************************************
  基地址定义
*********************************************************************************************************/
#define UART0_BASE            (0x01c28000)
/*********************************************************************************************************
  寄存器偏移
*********************************************************************************************************/
#define RBR                   0x0
#define THR                   0x0
#define USR                   0x7C
#define LCR                   0x0C
#define IER                   0x04

#define UART_VECTOR(x)  ((x) + 32)

VOID  uartPutChar (CHAR  cChar);
VOID  uartPutMsg (CPCHAR  cpcMsg);
VOID  uartGetChar (PCHAR  pcChar);

BOOL  uartIsRcvFifoEmpty (VOID);
VOID  uartEnableRcvInt (VOID);
SIO_CHAN  *uartSioChanCreate (VOID);

#endif  //  UART_H



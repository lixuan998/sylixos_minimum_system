#ifndef TIMER_H
#define TIMER_H

#define  __SYLIXOS_KERNEL
#include <SylixOS.h>
#include <linux/compat.h>

#define TIMER_BASE     0x01c20c00
#define TIMER_IRQ_EN   0x00
#define TIMER_IRQ_STA  0x04
#define TIMER0_CTRL    0x10
#define TIMER0_INTV    0x14
#define TIMER0_CUR     0x18
#define TIMER1_CTRL    0x20
#define TIMER1_INTV    0x24
#define TIMER1_CUR     0x28

#define TIMER_FREQ          (24 * 1000 * 1000 / 2)
#define TIMER_VECTOR(x)     ((x) + 50)

VOID  timerStart (INT32  iNum, UINT32  uiHZ);

VOID  timerIntClear (INT32  iNum);

BOOL  timerIsIntPending (INT32  iNum);

#endif  //  TIMER_H

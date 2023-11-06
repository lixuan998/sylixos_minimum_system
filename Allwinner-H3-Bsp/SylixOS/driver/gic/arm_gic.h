#ifndef ARM_GIC_H
#define ARM_GIC_H

#define  __SYLIXOS_KERNEL
#include <SylixOS.h>
#include <linux/compat.h>

#define ARM_GIC_DISTRIBUTOR_BASE        0x01C81000
#define ARM_GIC_CPU_INTERFACE_BASE      0x01C82000

#define GIC_INT_NUM                     32
#define GIC_INT_GRP_NUM                 8

#define GIC_CPU_INTERFACE_PREEM_EN      0
#define GIC_CPU_INTERFACE_PREEM_DISEN   7

#define GICD_CTLR_SECURE_MASK           1
#define GICD_CTLR_NSECURE_MASK          1 << 1

#define GICC_IAR_IT_ID_MASK             0x1FF

typedef struct{
    volatile UINT32 CTLR;                 //  Distributer Control Register
    volatile UINT32 TYPER;                //  Interrupt Control Type Register
    volatile UINT32 IIDR;                 //  Distributor Implementer Identification Register
    volatile UINT32 Reserved0[29];        //  Reserved
    volatile UINT32 IGROUPRn[32];         //  Interrupt Group Registers
    volatile UINT32 ISENABLERn[32];       //  Interrupt Set-Enable Registers
    volatile UINT32 ICENABLERn[32];       //  Interrupt Clear-Enable Registers
    volatile UINT32 ISPENDRn[32];         //  Interrupt Set-Pending Registers
    volatile UINT32 ICPENDRn[32];         //  Interrupt Clear-Pending Registers
    volatile UINT32 ISACTIVERn[32];       //  GICv2 Interrupt Set-Active Registers
    volatile UINT32 ICACTIVERn[32];       //  Interrupt Clear-Active Registers
    volatile UINT8 IPRIORITYRn[255 * 4];  //  Interrupt Priority Registers
    volatile UINT32 Reserved2;            //  Reserved
    volatile UINT8 ITARGETSRn[255 * 4];   //  Interrupt Processor Targets Registers
    volatile UINT32 Reserved3;            //  Reserved
    volatile UINT32 ICFGRn[64];           //  Interrupt Configuration Registers
    volatile UINT32 Reserved4[64];        //  Reserved
    volatile UINT32 NSACRn[64];           //  Non-secure Access Control Registers, optional
    volatile UINT32 SGIR;                 //  Software Generated Interrupt Register
    volatile UINT32 Reserved5[3];         //  Reserved
    volatile UINT32 CPENDSGIRn[4];        //  SGI Clear-Pending Registers
    volatile UINT32 SPENDSGIRn[4];        //  SGI Set-Pending Registers
    volatile UINT32 Reserved6[52];        //  Reserved
} GIC_DISTRIBUTOR_REGS;

typedef struct{
    volatile UINT32 CTLR;                 //  CPU Interface Control Register
    volatile UINT32 PMR;                  //  Interrupt Priority Mask Register
    volatile UINT32 BPR;                  //  Binary Point Register
    volatile UINT32 IAR;                  //  Interrupt Acknowledge Register
    volatile UINT32 EOIR;                 //  End of Interrupt Register
    volatile UINT32 RPR;                  //  Running Priority Register
    volatile UINT32 HPPIR;                //  Highest Priority Pending Interrupt Register
    volatile UINT32 ABPR;                 //  Aliased Binary Point Register;

    volatile UINT32 Reserved[55];         //  Reserved

    volatile UINT32 IIDR;                 //  CPU Interface Identification Register
    volatile UINT32 DIR;                  //  Deactivate Interrupt Register
} GIC_CPU_INTERFACE_REGS;

GIC_DISTRIBUTOR_REGS * armGicDistributorGet();
GIC_CPU_INTERFACE_REGS * armGicCPUInterfaceGet();

VOID armGicCPUPriorityMaskSet(UINT32 priority);
VOID armGicEnable(BOOL is_enable);
VOID armGicCPUEnable(BOOL is_enable);
VOID armGicInit (VOID);
VOID armGicCPUInit (BOOL  bPreemption, UINT32  uiPriority);

VOID armGicIrqWriteDone (UINT32  uiIrqID);
UINT32 armGicIrqReadAck();

VOID armGicIrqEnable(UINT32 ulVector, BOOL is_enable);
VOID armGicIrqTargetSet(UINT32 ulVector, UINT32 mask);
VOID armGicIrqPrioritySet(UINT32 ulVector, ULONG ulPriority);

VOID armGicIntVecterEnable (ULONG  ulVector, ULONG  ulPriority, ULONG  uiCpuMask);
VOID armGicIntVecterDisable (ULONG  ulVector);
BOOL armGicIrqIsEnable (UINT32  uiIrqID);

#endif  //  ARM_GIC_H

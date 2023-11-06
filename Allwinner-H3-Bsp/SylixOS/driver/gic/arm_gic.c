#include "arm_gic.h"

GIC_DISTRIBUTOR_REGS * armGicDistributorGet()
{
    GIC_DISTRIBUTOR_REGS *pDistributor = (GIC_DISTRIBUTOR_REGS *)ARM_GIC_DISTRIBUTOR_BASE;
    return pDistributor;
}

GIC_CPU_INTERFACE_REGS * armGicCPUInterfaceGet()
{
    GIC_CPU_INTERFACE_REGS *pInterface = (GIC_CPU_INTERFACE_REGS *)ARM_GIC_CPU_INTERFACE_BASE;
    return pInterface;
}

VOID armGicCPUPriorityMaskSet(UINT32 priority)
{
    GIC_CPU_INTERFACE_REGS *pInterface = armGicCPUInterfaceGet();
    write32(priority, (addr_t)&pInterface->PMR);
}

VOID armGicEnable(BOOL is_enable)
{
    GIC_DISTRIBUTOR_REGS *pDistributor = armGicDistributorGet();
    if(is_enable == LW_TRUE)
    {
        write32(~GICD_CTLR_SECURE_MASK, (addr_t)&pDistributor->CTLR);
        write32(~0, (addr_t)&pDistributor->CTLR);
    }
    else
    {
        write32(0, (addr_t)&pDistributor->CTLR);
    }
}

VOID armGicCPUEnable(BOOL is_enable)
{
    REGISTER GIC_CPU_INTERFACE_REGS *pInterface = armGicCPUInterfaceGet();
    if(is_enable == LW_TRUE)
    {
        write32(1, (addr_t)&pInterface->CTLR);
    }
    else
    {
        write32(0, (addr_t)&pInterface->CTLR);
    }
}

VOID armGicInit (VOID)
{
    REGISTER GIC_DISTRIBUTOR_REGS *pDistributor = armGicDistributorGet();
    REGISTER INT iCnt;

    armGicEnable(LW_FALSE);                                       /* First disable the distributor */

    for (iCnt = 0; iCnt < GIC_INT_NUM; iCnt++)
    {                  /* Clear all pending interrupts  */
        write32(~0, (addr_t)&pDistributor->ICPENDRn[iCnt]);
    }

    for (iCnt = 0; iCnt < GIC_INT_NUM; iCnt++)
    {                  /* Disable all interrupts.       */
        write32(~0, (addr_t)&pDistributor->ICENABLERn[iCnt]);
    }

    armGicEnable(LW_TRUE);                                        /* Now enable the distributor    */
}

VOID armGicCPUInit (BOOL  bPreemption, UINT32  uiPriority)
{
    REGISTER  GIC_CPU_INTERFACE_REGS *pInterface = armGicCPUInterfaceGet();

    armGicCPUPriorityMaskSet(uiPriority);                                  /* Init the GIC CPU interface */

    if (bPreemption)
    {
        write32(GIC_CPU_INTERFACE_PREEM_EN, (addr_t)&pInterface->BPR);
    }
    else
    {
        write32(GIC_CPU_INTERFACE_PREEM_DISEN, (addr_t)&pInterface->BPR);
    }

    armGicCPUEnable(LW_TRUE);                                              /* Enable signaling the CPU */
}

UINT32 armGicIrqReadAck()
{
    REGISTER GIC_CPU_INTERFACE_REGS *pInterface = armGicCPUInterfaceGet();
    return (read32((addr_t)&pInterface->IAR));
}

VOID armGicIrqWriteDone (UINT32  uiIrqID)
{
    REGISTER GIC_CPU_INTERFACE_REGS *pInterface = armGicCPUInterfaceGet();
    write32(uiIrqID, (addr_t)&pInterface->EOIR);
}

VOID armGicIrqEnable(UINT32 ulVector, BOOL is_enable)
{
    REGISTER GIC_DISTRIBUTOR_REGS *pDistributor = armGicDistributorGet();
    UINT32 mask = 1 << (ulVector % 32);

    if(is_enable == LW_TRUE)
    {
        write32(mask, (addr_t)&pDistributor->ISENABLERn[ulVector / 32]);
    }

    else
    {
        write32(~mask, (addr_t)&pDistributor->ISENABLERn[ulVector / 32]);
    }
}
VOID armGicIrqTargetSet(UINT32 ulVector, UINT32 mask)
{
    REGISTER GIC_DISTRIBUTOR_REGS *pDistributor = armGicDistributorGet();
    write8((UINT8)mask, (addr_t)&pDistributor->ITARGETSRn[ulVector]);
}
VOID armGicIrqPrioritySet(UINT32 ulVector, ULONG ulPriority)
{
    REGISTER GIC_DISTRIBUTOR_REGS *pDistributor = armGicDistributorGet();
    write8((UINT8)ulPriority, (addr_t)&pDistributor->IPRIORITYRn[ulVector]);
}
VOID armGicIntVecterEnable (ULONG  ulVector, ULONG  ulPriority, ULONG  uiCpuMask)
{
    armGicIrqEnable(ulVector, LW_FALSE);
    armGicIrqPrioritySet(ulVector, ulPriority);
    armGicIrqTargetSet(ulVector, uiCpuMask);
    armGicIrqEnable(ulVector, LW_TRUE);
}
VOID armGicIntVecterDisable (ULONG  ulVector)
{
    armGicIrqEnable(ulVector, LW_FALSE);
    armGicIrqTargetSet(ulVector, 0);
}

BOOL armGicIrqIsEnable (UINT32  uiIrqID)
{
    REGISTER GIC_DISTRIBUTOR_REGS *pDistributor = armGicDistributorGet();
    return ((read32((addr_t)&pDistributor->ICENABLERn[uiIrqID / 32]) & (1 << (uiIrqID % 32))) ? LW_TRUE : LW_FALSE);
}

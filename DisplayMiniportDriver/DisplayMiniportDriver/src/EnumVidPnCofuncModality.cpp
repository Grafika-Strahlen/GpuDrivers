// See https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dkmddi/nc-d3dkmddi-dxgkddi_enumvidpncofuncmodality
#include "EnumVidPnCofuncModality.hpp"
#include "HyDevice.hpp"

#pragma code_seg("PAGE")

NTSTATUS HyEnumVidPnCofuncModality(IN_CONST_HANDLE hAdapter, IN_CONST_PDXGKARG_ENUMVIDPNCOFUNCMODALITY_CONST pEnumCofuncModality)
{
    PAGED_CODE();
    CHECK_IRQL(PASSIVE_LEVEL);

    LOG_DEBUG("HyEnumVidPnCofuncModality\n");

    // If MiniportDeviceContext (hAdapter) is null inform the kernel that the first parameter was invalid.
    // This should probably never happen.
    if(!hAdapter)
    {
        LOG_ERROR("Invalid Parameter to HyEnumVidPnCofuncModality: hAdapter\n");
        return STATUS_INVALID_PARAMETER_1;
    }

    // If pQueryAdapterInfo is null inform the kernel that the second parameter was invalid.
    // This should probably never happen.
    if(!pEnumCofuncModality)
    {
        LOG_ERROR("Invalid Parameter to HyEnumVidPnCofuncModality: pEnumCofuncModality\n");
        return STATUS_INVALID_PARAMETER_2;
    }

    HyMiniportDevice* const deviceContext = HY_MINIPORT_DEVICE_FROM_HANDLE(hAdapter);

    return deviceContext->EnumVidPnCofuncModality(pEnumCofuncModality);
}


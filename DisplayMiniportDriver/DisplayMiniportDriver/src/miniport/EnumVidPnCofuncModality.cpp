// See https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dkmddi/nc-d3dkmddi-dxgkddi_enumvidpncofuncmodality
#include "Common.h"
#include "miniport/EnumVidPnCofuncModality.hpp"
#include "GsDevice.hpp"

#pragma code_seg("PAGE")

NTSTATUS GsEnumVidPnCofuncModality(IN_CONST_HANDLE hAdapter, IN_CONST_PDXGKARG_ENUMVIDPNCOFUNCMODALITY_CONST pEnumCofuncModality)
{
    PAGED_CODE();
    CHECK_IRQL(PASSIVE_LEVEL);

    TRACE_ENTRYPOINT();

    // If MiniportDeviceContext (hAdapter) is null inform the kernel that the first parameter was invalid.
    // This should probably never happen.
    if(!hAdapter)
    {
        LOG_ERROR("Invalid Parameter: hAdapter\n");
        return STATUS_INVALID_PARAMETER_1;
    }

    // If pQueryAdapterInfo is null inform the kernel that the second parameter was invalid.
    // This should probably never happen.
    if(!pEnumCofuncModality)
    {
        LOG_ERROR("Invalid Parameter: pEnumCofuncModality\n");
        return STATUS_INVALID_PARAMETER_2;
    }

    GsMiniportDevice* const deviceContext = GsMiniportDevice::FromHandle(hAdapter);

    return deviceContext->EnumVidPnCofuncModality(pEnumCofuncModality);
}


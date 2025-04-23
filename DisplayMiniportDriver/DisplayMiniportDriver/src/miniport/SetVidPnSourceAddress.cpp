// See https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dkmddi/nc-d3dkmddi-dxgkddi_setvidpnsourceaddress
#include "Common.h"
#include "miniport/SetVidPnSourceAddress.hpp"
#include "HyDevice.hpp"

#pragma code_seg("_KTEXT")

NTSTATUS GsSetVidPnSourceAddress(IN_CONST_HANDLE hAdapter, IN_CONST_PDXGKARG_SETVIDPNSOURCEADDRESS pSetVidPnSourceAddress)
{
    PAGED_CODE();
    CHECK_IRQL(PASSIVE_LEVEL);

    TRACE_ENTRYPOINT();

    // If hAdapter is null inform the kernel that the first parameter was invalid.
    // This should probably never happen.
    if(!hAdapter)
    {
        LOG_ERROR("Invalid Parameter: hAdapter\n");
        return STATUS_INVALID_PARAMETER_1;
    }

    // If pSetVidPnSourceAddress is null inform the kernel that the second parameter was invalid.
    // This should probably never happen.
    if(!pSetVidPnSourceAddress)
    {
        LOG_ERROR("Invalid Parameter: pSetVidPnSourceAddress\n");
        return STATUS_INVALID_PARAMETER_2;
    }

    // Get our context structure.
    GsMiniportDevice* const deviceContext = GsMiniportDevice::FromHandle(hAdapter);

    return deviceContext->SetVidPnSourceAddress(pSetVidPnSourceAddress);
}

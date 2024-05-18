// See https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dkmddi/nc-d3dkmddi-dxgkddi_issupportedvidpn
#include "Common.h"
#include "HyDevice.hpp"
#include "IsSupportedVidPn.hpp"
#include "Logging.h"

#pragma code_seg("PAGE")

NTSTATUS HyIsSupportedVidPn(IN_CONST_HANDLE hAdapter, INOUT_PDXGKARG_ISSUPPORTEDVIDPN pIsSupportedVidPn)
{
    PAGED_CODE();
    CHECK_IRQL(PASSIVE_LEVEL);

    LOG_DEBUG("HyIsSupportedVidPn\n");

    // If hAdapter is null inform the kernel that the first parameter was invalid.
    // This should probably never happen.
    if(!hAdapter)
    {
        LOG_ERROR("Invalid Parameter to HyIsSupportedVidPn: hAdapter\n");
        return STATUS_INVALID_PARAMETER_1;
    }

    // If pIsSupportedVidPn is null inform the kernel that the second parameter was invalid.
    // This should probably never happen.
    if(!hAdapter)
    {
        LOG_ERROR("Invalid Parameter to HyIsSupportedVidPn: pIsSupportedVidPn\n");
        return STATUS_INVALID_PARAMETER_1;
    }

    // Get our context structure.
    HyMiniportDevice* const deviceContext = HY_MINIPORT_DEVICE_FROM_HANDLE(hAdapter);

    return deviceContext->IsSupportedVidPn(pIsSupportedVidPn);
}

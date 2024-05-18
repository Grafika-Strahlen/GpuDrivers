// See https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dkmddi/nc-d3dkmddi-dxgkddi_collectdbginfo
// See https://learn.microsoft.com/en-us/windows-hardware/drivers/display/threading-and-synchronization-zero-level
#include "Common.h"
#include "CollectDbgInfo.hpp"
#include "HyDevice.hpp"
#include "Logging.h"

#pragma code_seg("PAGE")

NTSTATUS HyCollectDbgInfo(IN_CONST_HANDLE hAdapter, IN_CONST_PDXGKARG_COLLECTDBGINFO pCollectDbgInfo)
{
    // This function should collect debug information for various failures and can be
    // called at any time and at high IRQL (that is, the IRQL that DxgkDdiCollectDbgInfo
    // runs at is generally undefined).
    CHECK_IRQL(HIGH_LEVEL);

    LOG_DEBUG("HyCollectDbgInfo\n");

    // If hAdapter is null inform the kernel that the first parameter was invalid.
    // This should probably never happen.
    if(!hAdapter)
    {
        LOG_ERROR("Invalid Parameter to HyCollectDbgInfo: hAdapter\n");
        return STATUS_INVALID_PARAMETER_1;
    }

    HyMiniportDevice* const deviceContext = HY_MINIPORT_DEVICE_FROM_HANDLE(hAdapter);

    return deviceContext->CollectDbgInfo(pCollectDbgInfo);
}

// See https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dkmddi/nc-d3dkmddi-dxgkddi_presentdisplayonly
#include "Common.h"
#include "miniport/PresentDisplayOnly.hpp"
#include "GsDevice.hpp"

#pragma code_seg("PAGE")

#if DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WIN8
NTSTATUS GsPresentDisplayOnly(IN_CONST_HANDLE hAdapter, IN_CONST_PDXGKARG_PRESENT_DISPLAYONLY pPresentDisplayOnly)
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

    // If pPresentDisplayOnly is null inform the kernel that the second parameter was invalid.
    // This should probably never happen.
    if(!pPresentDisplayOnly)
    {
        LOG_ERROR("Invalid Parameter: pPresentDisplayOnly\n");
        return STATUS_INVALID_PARAMETER_1;
    }

    // Get our context structure.
    GsMiniportDevice* const deviceContext = GsMiniportDevice::FromHandle(hAdapter);

    return deviceContext->PresentDisplayOnly(pPresentDisplayOnly);
}
#endif

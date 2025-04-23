// See https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/dispmprt/nc-dispmprt-dxgkddi_dpc_routine
#include "Common.h"
#include "miniport/DpcRoutine.hpp"
#include "GsDevice.hpp"

#pragma code_seg("_KTEXT")

void GsDpcRoutine(IN_CONST_PVOID MiniportDeviceContext)
{
    (void) MiniportDeviceContext;

    CHECK_IRQL(DISPATCH_LEVEL);

    if constexpr(false)
    {
        TRACE_ENTRYPOINT();
    }

    // If MiniportDeviceContext is null inform the kernel that the first parameter was invalid.
    // This should probably never happen.
    if(!MiniportDeviceContext)
    {
        LOG_ERROR("Invalid Parameter: MiniportDeviceContext\n");
        return;
    }

    GsMiniportDevice* const deviceContext = GsMiniportDevice::FromHandle(MiniportDeviceContext);

    deviceContext->DpcRoutine();
}

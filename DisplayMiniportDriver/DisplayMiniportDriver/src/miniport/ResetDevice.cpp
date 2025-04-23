// See https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/dispmprt/nc-dispmprt-dxgkddi_reset_device
#include "Common.h"
#include "miniport/ResetDevice.hpp"
#include "GsDevice.hpp"

void GsResetDevice(IN_CONST_PVOID MiniportDeviceContext)
{
    TRACE_ENTRYPOINT();

    // If MiniportDeviceContext is null inform log that the parameter was invalid.
    // This should probably never happen.
    if(!MiniportDeviceContext)
    {
        LOG_ERROR("Invalid Parameter: MiniportDeviceContext\n");
        return;
    }

    // Get our context structure.
    GsMiniportDevice* const deviceContext = GsMiniportDevice::FromHandle(MiniportDeviceContext);

    deviceContext->ResetDevice();
}

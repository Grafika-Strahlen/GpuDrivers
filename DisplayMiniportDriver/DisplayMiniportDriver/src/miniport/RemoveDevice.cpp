// See https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/dispmprt/nc-dispmprt-dxgkddi_remove_device
#include "Common.h"
#include "miniport/RemoveDevice.hpp"
#include "GsDevice.hpp"

#pragma code_seg("PAGE")

NTSTATUS GsRemoveDevice(IN_CONST_PVOID MiniportDeviceContext)
{
    PAGED_CODE();
    CHECK_IRQL(PASSIVE_LEVEL);

    TRACE_ENTRYPOINT();

    // If the context is null we can skip freeing it.
    if(MiniportDeviceContext)
    {
        // Get our context structure.
        GsMiniportDevice* deviceContext = GsMiniportDevice::FromHandle(MiniportDeviceContext);

        LOG_DEBUG("Freeing MiniportDeviceContext.\n");

        deviceContext->~GsMiniportDevice();

        // Free the context.
        // HY_FREE(deviceContext, POOL_TAG_DEVICE_CONTEXT);
        delete deviceContext;
    }

    return STATUS_SUCCESS;
}

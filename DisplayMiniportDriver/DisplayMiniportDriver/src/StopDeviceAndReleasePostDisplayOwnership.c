// See https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/dispmprt/nc-dispmprt-dxgkddi_stop_device_and_release_post_display_ownership
#ifdef __cplusplus
extern "C" {
#endif

#include <ntddk.h>
#include <dispmprt.h>

#include "AddDevice.h"
#include "StopDevice.h"
#include "SetPowerState.h"
#include "StopDeviceAndReleasePostDisplayOwnership.h"
#include "Logging.h"

#pragma code_seg("PAGE")

NTSTATUS HyStopDeviceAndReleasePostDisplayOwnership(IN_CONST_PVOID MiniportDeviceContext, IN_CONST_D3DDDI_VIDEO_PRESENT_TARGET_ID TargetId, PDXGK_DISPLAY_INFORMATION DisplayInfo)
{
    PAGED_CODE();
    CHECK_IRQL(PASSIVE_LEVEL);

    LOG_DEBUG("HyStopDeviceAndReleasePostDisplayOwnership\n");

    // If MiniportDeviceContext is null inform the kernel that the first parameter was invalid.
    // This should probably never happen.
    if(!MiniportDeviceContext)
    {
        LOG_ERROR("Invalid Parameter to HyStopDeviceAndReleasePostDisplayOwnership: MiniportDeviceContext\n");
        return STATUS_INVALID_PARAMETER_1;
    }

    // Get our context structure.
    const HyMiniportDeviceContext* const deviceContext = MiniportDeviceContext;

    if(deviceContext->MonitorPowerState > PowerDeviceD0)
    {
        HySetPowerState(MiniportDeviceContext, TargetId, PowerDeviceD0, PowerActionNone);
    }

    if(deviceContext->Flags.IsStarted)
    {
        volatile UINT* const displayEnable0 = HyGetDeviceConfigRegister(deviceContext, BASE_REGISTER_DI + SIZE_REGISTER_DI * 0 + OFFSET_REGISTER_DI_ENABLE);

        // Disable Display
        *displayEnable0 = 0;
    }

    // TODO: Blackout the screen.

    *DisplayInfo = deviceContext->PostDisplayInfo;

    return HyStopDevice(MiniportDeviceContext);
}

#ifdef __cplusplus
} /* extern "C" */
#endif

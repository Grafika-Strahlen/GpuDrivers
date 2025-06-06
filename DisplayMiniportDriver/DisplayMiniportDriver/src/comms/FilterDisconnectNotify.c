// See https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/fltkernel/nf-fltkernel-fltcreatecommunicationport
// #include <ntddk.h>
#include <fltKernel.h>

#include "comms/FilterDisconnectNotify.h"
#include "comms/FilterDeviceManager.h"
#include "Logging.h"

#pragma code_seg("PAGE")

void HyFilterDisconnectNotify(PVOID ConnectionPortCookie)
{
    PAGED_CODE();

    TRACE_ENTRYPOINT();

    HyUnregisterFilterClient(ConnectionPortCookie);
}

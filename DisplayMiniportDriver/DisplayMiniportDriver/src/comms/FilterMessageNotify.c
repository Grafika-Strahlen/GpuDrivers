// See https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/fltkernel/nf-fltkernel-fltcreatecommunicationport
// #include <ntddk.h>
#include <fltKernel.h>

#include "comms/FilterMessageNotify.h"
#include "comms/FilterDeviceManager.h"
#include "Logging.h"

#pragma code_seg("PAGE")

NTSTATUS HyFilterMessageNotify(PVOID PortCookie, PVOID InputBuffer, ULONG InputBufferLength, PVOID OutputBuffer, ULONG OutputBufferLength, PULONG ReturnOutputBufferLength)
{
    (void) InputBuffer;
    (void) InputBufferLength;
    (void) OutputBuffer;
    (void) OutputBufferLength;
    (void) ReturnOutputBufferLength;

    PAGED_CODE();

    TRACE_ENTRYPOINT();

    PFLT_PORT port;
    const NTSTATUS getFilterClientStatus = HyGetFilterClient(PortCookie, &port);

    if(!NT_SUCCESS(getFilterClientStatus))
    {
        return getFilterClientStatus;
    }

    return STATUS_SUCCESS;
}

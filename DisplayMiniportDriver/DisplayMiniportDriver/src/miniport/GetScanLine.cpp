// See https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dkmddi/nc-d3dkmddi-dxgkddi_getscanline
// See https://learn.microsoft.com/en-us/windows-hardware/drivers/display/saving-energy-with-vsync-control#display-only-vsync-requirements-for-windows-8-and-later-versions
#include "Common.h"
#include "miniport/GetScanLine.hpp"
#include "GsDevice.hpp"

#pragma code_seg("PAGE")

NTSTATUS HyGetScanLine(IN_CONST_HANDLE hAdapter, INOUT_PDXGKARG_GETSCANLINE pGetScanLine)
{
    CHECK_IRQL(PASSIVE_LEVEL);

    if constexpr(true)
    {
        TRACE_ENTRYPOINT();
    }

    // If hAdapter is null inform the kernel that the first parameter was invalid.
    // This should probably never happen.
    if(!hAdapter)
    {
        LOG_ERROR("Invalid Parameter: hAdapter\n");
        return STATUS_INVALID_PARAMETER_1;
    }

    GsMiniportDevice* const deviceContext = GsMiniportDevice::FromHandle(hAdapter);

    return deviceContext->GetScanLine(pGetScanLine);
}

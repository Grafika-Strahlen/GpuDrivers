// See https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dkmddi/nc-d3dkmddi-dxgkddi_recommendfunctionalvidpn
#include "Common.h"
#include "miniport/RecommendFunctionalVidPn.hpp"
#include "GsDevice.hpp"

#pragma code_seg("PAGE")

NTSTATUS GsRecommendFunctionalVidPn(IN_CONST_HANDLE hAdapter, IN_CONST_PDXGKARG_RECOMMENDFUNCTIONALVIDPN_CONST pRecommendFunctionalVidPn)
{
    PAGED_CODE();
    CHECK_IRQL(PASSIVE_LEVEL);

    TRACE_ENTRYPOINT();

    // If MiniportDeviceContext (hAdapter) is null inform the kernel that the first parameter was invalid.
    // This should probably never happen.
    if(!hAdapter)
    {
        LOG_ERROR("Invalid Parameter: hAdapter\n");
        return STATUS_INVALID_PARAMETER_1;
    }

    // If pRecommendFunctionalVidPn is null inform the kernel that the second parameter was invalid.
    // This should probably never happen.
    if(!pRecommendFunctionalVidPn)
    {
        LOG_ERROR("Invalid Parameter: pRecommendFunctionalVidPn\n");
        return STATUS_INVALID_PARAMETER_2;
    }

    GsMiniportDevice* const deviceContext = GsMiniportDevice::FromHandle(hAdapter);

    return deviceContext->RecommendFunctionalVidPn(pRecommendFunctionalVidPn);
}

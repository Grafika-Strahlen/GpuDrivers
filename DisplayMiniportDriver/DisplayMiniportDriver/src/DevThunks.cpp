#ifdef HY_BUILD_AS_KMDOD
  #undef HY_BUILD_AS_KMDOD
#endif

#define HY_BUILD_AS_KMDOD 1
#include "Common.h"
#include "DevThunks.hpp"

#pragma code_seg("PAGE")

NTSTATUS ThunkHySetPalette(
    IN_CONST_HANDLE                 hAdapter,
    IN_CONST_PDXGKARG_SETPALETTE    pSetPalette
)
{
    (void) hAdapter;
    (void) pSetPalette;
    TRACE_ENTRYPOINT();
    return STATUS_SUCCESS;
}

NTSTATUS ThunkHySetPointerPosition(
    IN_CONST_HANDLE                         hAdapter,
    IN_CONST_PDXGKARG_SETPOINTERPOSITION    pSetPointerPosition
)
{
    (void) hAdapter;
    (void) pSetPointerPosition;
    TRACE_ENTRYPOINT();
    return STATUS_SUCCESS;
}

NTSTATUS ThunkHySetPointerShape(
    IN_CONST_HANDLE                     hAdapter,
    IN_CONST_PDXGKARG_SETPOINTERSHAPE   pSetPointerShape
)
{
    (void) hAdapter;
    (void) pSetPointerShape;
    TRACE_ENTRYPOINT();
    return STATUS_SUCCESS;
}

NTSTATUS ThunkHyEscape(
    IN_CONST_HANDLE                 hAdapter,
    IN_CONST_PDXGKARG_ESCAPE        pEscape
)
{
    (void) hAdapter;
    (void) pEscape;
    TRACE_ENTRYPOINT();
    return STATUS_SUCCESS;
}

NTSTATUS ThunkHyCollectDbgInfo(
    IN_CONST_HANDLE                         hAdapter,
    IN_CONST_PDXGKARG_COLLECTDBGINFO        pCollectDbgInfo
)
{
    (void) hAdapter;
    (void) pCollectDbgInfo;
    TRACE_ENTRYPOINT_ARG("Reason: 0x%08X\n", pCollectDbgInfo->Reason);

    return STATUS_SUCCESS;
}

NTSTATUS ThunkHyRecommendMonitorModes(
    IN_CONST_HANDLE                                 hAdapter,
    IN_CONST_PDXGKARG_RECOMMENDMONITORMODES_CONST   pRecommendMonitorModes
)
{
    (void) hAdapter;
    (void) pRecommendMonitorModes;
    TRACE_ENTRYPOINT();
    return STATUS_SUCCESS;
}

NTSTATUS ThunkHyGetScanLine(
    IN_CONST_HANDLE             hAdapter,
    INOUT_PDXGKARG_GETSCANLINE  pGetScanLine
)
{
    (void) hAdapter;
    (void) pGetScanLine;
    TRACE_ENTRYPOINT();
    return STATUS_SUCCESS;
}

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WIN7)
NTSTATUS ThunkHyQueryVidPnCapability(
    IN_CONST_HANDLE                             hAdapter,
    INOUT_PDXGKARG_QUERYVIDPNHWCAPABILITY       pVidPnHWCaps
)
{
    (void) hAdapter;
    (void) pVidPnHWCaps;
    TRACE_ENTRYPOINT();
    return STATUS_SUCCESS;
}
#endif

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WIN8)
NTSTATUS ThunkHySystemDisplayEnable(
    _In_ PVOID MiniportDeviceContext,
    _In_ D3DDDI_VIDEO_PRESENT_TARGET_ID TargetId,
    _In_ PDXGKARG_SYSTEM_DISPLAY_ENABLE_FLAGS Flags,
    _Out_ UINT* Width,
    _Out_ UINT* Height,
    _Out_ D3DDDIFORMAT* ColorFormat
)
{
    (void) MiniportDeviceContext;
    (void) TargetId;
    (void) Flags;
    (void) Width;
    (void) Height;
    (void) ColorFormat;
    TRACE_ENTRYPOINT();
    return STATUS_SUCCESS;
}
#endif

void ThunkHySystemDisplayWrite(
    _In_ PVOID MiniportDeviceContext,
    _In_reads_bytes_(SourceHeight* SourceStride) PVOID Source,
    _In_ UINT SourceWidth,
    _In_ UINT SourceHeight,
    _In_ UINT SourceStride,
    _In_ UINT PositionX,
    _In_ UINT PositionY
)
{
    (void) MiniportDeviceContext;
    (void) Source;
    (void) SourceWidth;
    (void) SourceHeight;
    (void) SourceStride;
    (void) PositionX;
    (void) PositionY;
    TRACE_ENTRYPOINT();
}

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WIN8)
NTSTATUS ThunkHyGetChildContainerId(
    _In_ PVOID MiniportDeviceContext,
    _In_ ULONG ChildUid,
    _Inout_ PDXGK_CHILD_CONTAINER_ID ContainerId
)
{
    (void) MiniportDeviceContext;
    (void) ChildUid;
    (void) ContainerId;
    TRACE_ENTRYPOINT();
    return STATUS_SUCCESS;
}
#endif

NTSTATUS ThunkHyControlInterrupt(
    IN_CONST_HANDLE                 hAdapter,
    IN_CONST_DXGK_INTERRUPT_TYPE    InterruptType,
    IN_BOOLEAN                      EnableInterrupt
)
{
    (void) hAdapter;
    (void) InterruptType;
    (void) EnableInterrupt;
    TRACE_ENTRYPOINT_ARG("Type: %d, Enable: %d\n", InterruptType, EnableInterrupt);
    //return STATUS_SUCCESS;
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS ThunkHySetPowerComponentFState(
    IN_CONST_HANDLE DriverContext,
    UINT            ComponentIndex,
    UINT            FState
)
{
    (void) DriverContext;
    (void) ComponentIndex;
    (void) FState;
    TRACE_ENTRYPOINT();
    return STATUS_SUCCESS;
}

NTSTATUS ThunkHyPowerRuntimeControlRequest(
    IN_CONST_HANDLE DriverContext,
    IN              LPCGUID PowerControlCode,
    IN OPTIONAL     PVOID InBuffer,
    IN              SIZE_T InBufferSize,
    OUT OPTIONAL    PVOID OutBuffer,
    IN              SIZE_T OutBufferSize,
    OUT OPTIONAL    PSIZE_T BytesReturned
)
{
    (void) DriverContext;
    (void) PowerControlCode;
    (void) InBuffer;
    (void) InBufferSize;
    (void) OutBuffer;
    (void) OutBufferSize;
    (void) BytesReturned;
    TRACE_ENTRYPOINT();
    return STATUS_SUCCESS;
}

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WIN8)
NTSTATUS ThunkHyNotifySurpriseRemoval(
    _In_ PVOID MiniportDeviceContext,
    _In_ DXGK_SURPRISE_REMOVAL_TYPE RemovalType
)
{
    (void) MiniportDeviceContext;
    (void) RemovalType;
    TRACE_ENTRYPOINT();
    return STATUS_SUCCESS;
}
#endif

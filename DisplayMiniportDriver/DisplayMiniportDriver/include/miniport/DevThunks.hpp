#pragma once

#include "Common.h"

NTSTATUS ThunkGsSetPalette(
    IN_CONST_HANDLE                 hAdapter,
    IN_CONST_PDXGKARG_SETPALETTE    pSetPalette
);

NTSTATUS ThunkGsSetPointerPosition(
    IN_CONST_HANDLE                         hAdapter,
    IN_CONST_PDXGKARG_SETPOINTERPOSITION    pSetPointerPosition
);

NTSTATUS ThunkGsSetPointerShape(
    IN_CONST_HANDLE                     hAdapter,
    IN_CONST_PDXGKARG_SETPOINTERSHAPE   pSetPointerShape
);

NTSTATUS ThunkGsEscape(
    IN_CONST_HANDLE                 hAdapter,
    IN_CONST_PDXGKARG_ESCAPE        pEscape
);

NTSTATUS ThunkGsCollectDbgInfo(
    IN_CONST_HANDLE                         hAdapter,
    IN_CONST_PDXGKARG_COLLECTDBGINFO        pCollectDbgInfo
);

NTSTATUS ThunkGsRecommendMonitorModes(
    IN_CONST_HANDLE                                 hAdapter,
    IN_CONST_PDXGKARG_RECOMMENDMONITORMODES_CONST   pRecommendMonitorModes
);

NTSTATUS ThunkGsGetScanLine(
    IN_CONST_HANDLE             hAdapter,
    INOUT_PDXGKARG_GETSCANLINE  pGetScanLine
);

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WIN7)
NTSTATUS ThunkGsQueryVidPnCapability(
    IN_CONST_HANDLE                             hAdapter,
    INOUT_PDXGKARG_QUERYVIDPNHWCAPABILITY       pVidPnHWCaps
);
#endif

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WIN8)
NTSTATUS ThunkGsSystemDisplayEnable(
    _In_ PVOID MiniportDeviceContext,
    _In_ D3DDDI_VIDEO_PRESENT_TARGET_ID TargetId,
    _In_ PDXGKARG_SYSTEM_DISPLAY_ENABLE_FLAGS Flags,
    _Out_ UINT* Width,
    _Out_ UINT* Height,
    _Out_ D3DDDIFORMAT* ColorFormat
);
#endif

void ThunkGsSystemDisplayWrite(
    _In_ PVOID MiniportDeviceContext,
    _In_reads_bytes_(SourceHeight* SourceStride) PVOID Source,
    _In_ UINT SourceWidth,
    _In_ UINT SourceHeight,
    _In_ UINT SourceStride,
    _In_ UINT PositionX,
    _In_ UINT PositionY
);

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WIN8)
NTSTATUS ThunkGsGetChildContainerId(
    _In_ PVOID MiniportDeviceContext,
    _In_ ULONG ChildUid,
    _Inout_ PDXGK_CHILD_CONTAINER_ID ContainerId
);
#endif

NTSTATUS ThunkGsControlInterrupt(
    IN_CONST_HANDLE                 hAdapter,
    IN_CONST_DXGK_INTERRUPT_TYPE    InterruptType,
    IN_BOOLEAN                      EnableInterrupt
);

NTSTATUS ThunkGsSetPowerComponentFState(
    IN_CONST_HANDLE DriverContext,
    UINT            ComponentIndex,
    UINT            FState
);

NTSTATUS ThunkGsPowerRuntimeControlRequest(
    IN_CONST_HANDLE DriverContext,
    IN              LPCGUID PowerControlCode,
    IN OPTIONAL     PVOID InBuffer,
    IN              SIZE_T InBufferSize,
    OUT OPTIONAL    PVOID OutBuffer,
    IN              SIZE_T OutBufferSize,
    OUT OPTIONAL    PSIZE_T BytesReturned
);

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WIN8)
NTSTATUS ThunkGsNotifySurpriseRemoval(
    _In_ PVOID MiniportDeviceContext,
    _In_ DXGK_SURPRISE_REMOVAL_TYPE RemovalType
);
#endif

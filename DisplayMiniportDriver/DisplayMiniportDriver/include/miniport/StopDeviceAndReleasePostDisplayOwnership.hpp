#pragma once

#include "Common.h"

NTSTATUS GsStopDeviceAndReleasePostDisplayOwnership(
    IN_CONST_PVOID MiniportDeviceContext, 
    IN_CONST_D3DDDI_VIDEO_PRESENT_TARGET_ID TargetId, 
    PDXGK_DISPLAY_INFORMATION DisplayInfo
);

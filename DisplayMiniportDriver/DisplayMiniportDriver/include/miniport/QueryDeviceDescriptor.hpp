#pragma once

#include "Common.h"

NTSTATUS GsQueryDeviceDescriptor(
    IN_CONST_PVOID MiniportDeviceContext, 
    IN_ULONG ChildUid, 
    INOUT_PDXGK_DEVICE_DESCRIPTOR DeviceDescriptor
);

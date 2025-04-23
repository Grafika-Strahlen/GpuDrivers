#pragma once

#include "Common.h"

NTSTATUS GsAddDevice(
    IN_CONST_PDEVICE_OBJECT PhysicalDeviceObject, 
    OUT_PPVOID MiniportDeviceContext
);

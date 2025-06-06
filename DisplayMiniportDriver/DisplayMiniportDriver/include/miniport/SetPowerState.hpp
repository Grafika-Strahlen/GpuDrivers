#pragma once

#include "Common.h"

NTSTATUS GsSetPowerState(
    IN_CONST_PVOID MiniportDeviceContext, 
    IN_ULONG DeviceUid, 
    IN_DEVICE_POWER_STATE DevicePowerState, 
    IN_POWER_ACTION ActionType
);

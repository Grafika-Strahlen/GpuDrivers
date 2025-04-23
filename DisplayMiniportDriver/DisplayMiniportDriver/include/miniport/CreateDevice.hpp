#pragma once

#include "Common.h"

NTSTATUS GsCreateDevice(
    IN_CONST_HANDLE hAdapter, 
    INOUT_PDXGKARG_CREATEDEVICE pCreateDevice
);

#pragma once

#include "Common.h"

NTSTATUS GsQueryChildStatus(
    IN_CONST_PVOID MiniportDeviceContext, 
    INOUT_PDXGK_CHILD_STATUS ChildStatus, 
    IN_BOOLEAN NonDestructiveOnly
);

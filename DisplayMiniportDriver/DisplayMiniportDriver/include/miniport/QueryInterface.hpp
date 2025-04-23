#pragma once

#include "Common.h"

NTSTATUS GsQueryInterface(
    IN_CONST_PVOID MiniportDeviceContext, 
    IN_PQUERY_INTERFACE QueryInterface
);

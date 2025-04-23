#pragma once

#include "Common.h"

NTSTATUS GsQueryChildRelations(
    IN_CONST_PVOID MiniportDeviceContext, 
    PDXGK_CHILD_DESCRIPTOR ChildRelations, 
    ULONG ChildRelationsSize
);

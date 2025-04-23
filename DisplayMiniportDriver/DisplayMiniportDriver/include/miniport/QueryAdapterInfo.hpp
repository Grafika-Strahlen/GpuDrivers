#pragma once

#include "Common.h"

NTSTATUS GsQueryAdapterInfo(
    IN_CONST_HANDLE hAdapter, 
    IN_CONST_PDXGKARG_QUERYADAPTERINFO pQueryAdapterInfo
);

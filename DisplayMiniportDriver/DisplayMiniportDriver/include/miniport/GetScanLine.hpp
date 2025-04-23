#pragma once

#include "Common.h"

NTSTATUS GsGetScanLine(
    IN_CONST_HANDLE hAdapter, 
    INOUT_PDXGKARG_GETSCANLINE pGetScanLine
);

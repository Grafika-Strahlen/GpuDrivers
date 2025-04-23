#pragma once

#include "Common.h"

NTSTATUS GsCollectDbgInfo(
    IN_CONST_HANDLE hAdapter, 
    IN_CONST_PDXGKARG_COLLECTDBGINFO pCollectDbgInfo
);

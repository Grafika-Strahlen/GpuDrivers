#pragma once

#include "Common.h"

#if DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WIN8
NTSTATUS GsPresentDisplayOnly(
    IN_CONST_HANDLE hAdapter, 
    IN_CONST_PDXGKARG_PRESENT_DISPLAYONLY pPresentDisplayOnly
);
#endif

#pragma once

#include "Common.h"

NTSTATUS GsControlInterrupt(
    IN_CONST_HANDLE hAdapter, 
    IN_CONST_DXGK_INTERRUPT_TYPE InterruptType, 
    IN_BOOLEAN EnableInterrupt
);

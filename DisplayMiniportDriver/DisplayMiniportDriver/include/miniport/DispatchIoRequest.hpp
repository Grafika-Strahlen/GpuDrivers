#pragma once

#include "Common.h"

NTSTATUS GsDispatchIoRequest(
    IN_CONST_PVOID MiniportDeviceContext, 
    IN_ULONG VidPnSourceId, 
    IN_PVIDEO_REQUEST_PACKET VideoRequestPacket
);

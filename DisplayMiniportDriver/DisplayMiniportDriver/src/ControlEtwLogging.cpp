// See https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/dispmprt/nc-dispmprt-dxgkddi_control_etw_logging

#include "ControlEtwLogging.hpp"
#include "Logging.h"

#pragma code_seg("PAGE")

void HyControlEtwLogging(IN_BOOLEAN Enable, IN_ULONG Flags, IN_UCHAR Level)
{
    (void) Enable;
    (void) Flags;
    (void) Level;

    CHECK_IRQL(PASSIVE_LEVEL);

    LOG_DEBUG("HyControlEtwLogging\n");
}
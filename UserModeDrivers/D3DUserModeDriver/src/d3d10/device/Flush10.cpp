// See https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3d10umddi/nc-d3d10umddi-pfnd3d10ddi_flush
// See https://learn.microsoft.com/en-us/windows-hardware/drivers/display/windows-vista-and-later-display-driver-model-operation-flow
// See https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dumddi/nc-d3dumddi-pfnd3dddi_rendercb
// See https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dumddi/ns-d3dumddi-_d3dddicb_render
#include "d3d10/device/Flush10.hpp"
#include "d3d10/GsDevice10.hpp"
#include "Logging.hpp"

void APIENTRY GsFlush10(
    const D3D10DDI_HDEVICE hDevice
)
{
    TRACE_ENTRYPOINT_ARG(
        u8"hDevice: 0x{XP0}",
        hDevice.pDrvPrivate
    );

    if(!hDevice.pDrvPrivate)
    {
        LOG_ERROR(u8"hDevice was not set.");
        return;
    }

    GsDevice10::FromHandle(hDevice)->Flush();
}

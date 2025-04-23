// See https://docs.microsoft.com/en-us/windows-hardware/drivers/display/driverentry-of-display-miniport-driver
#include "Common.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <process.h>

#ifdef __cplusplus
} /* extern "C" */
#endif

#include "comms/DeviceComms.hpp"
#include "DriverConfig.hpp"

#pragma code_seg(push)
#pragma code_seg("INIT")
#ifdef __cplusplus
extern "C" {
#endif
DRIVER_INITIALIZE DriverEntry;
#ifdef __cplusplus
} /* extern "C" */
#endif


_Use_decl_annotations_ NTSTATUS DriverEntryFull3D(
    IN PDRIVER_OBJECT DriverObject, 
    IN PUNICODE_STRING RegistryPath
);
_Use_decl_annotations_ NTSTATUS DriverEntryDisplayOnly(
    IN PDRIVER_OBJECT DriverObject, 
    IN PUNICODE_STRING RegistryPath
);
_Use_decl_annotations_ NTSTATUS DriverEntryResilientDisplayOnly(
    IN PDRIVER_OBJECT DriverObject, 
    IN PUNICODE_STRING RegistryPath
);

_Use_decl_annotations_ NTSTATUS DriverEntryReal(
    IN PDRIVER_OBJECT DriverObject, 
    IN PUNICODE_STRING RegistryPath
)
{
    PAGED_CODE();

    //
    // Opt-in to using non-executable pool memory on Windows 8 and later.
    // This has to be done before any allocations (which our logging functions can potentially do).
    // https://msdn.microsoft.com/en-us/library/windows/hardware/hh920402(v=vs.85).aspx
    //
    ExInitializeDriverRuntime(DrvRtPoolNxOptIn);

    CHECK_IRQL(PASSIVE_LEVEL);

    LOG_DEBUG("DriverEntryReal - " __TIMESTAMP__ ", WDDM Version: 0x%04X\n", DXGKDDI_INTERFACE_VERSION);

    // If DriverObject is null inform the kernel that the first parameter was invalid.
    // This should probably never happen.
    if(!ARGUMENT_PRESENT(DriverObject))
    {
        LOG_ERROR("Invalid Parameter to DriverEntry: DriverObject\n");
        return STATUS_INVALID_PARAMETER_1;
    }

    // If RegistryPath is null inform the kernel that the second parameter was invalid.
    // This should probably never happen.
    if(!ARGUMENT_PRESENT(RegistryPath))
    {
        LOG_ERROR("Invalid Parameter to DriverEntry: RegistryPath\n");
        return STATUS_INVALID_PARAMETER_2;
    }

    {
        // Initialize the emulation comms if necessary.
        const NTSTATUS deviceCommsStatus = InitDeviceComms(DriverObject);
        if(!NT_SUCCESS(deviceCommsStatus))
        {
            return deviceCommsStatus;
        }
    }

    GsConfigManager& configManager = GsConfigManager::Instance();

    {
        NTSTATUS status = configManager.Init(RegistryPath);

        if(!NT_SUCCESS(status))
        {
            configManager.DriverType() = GsDriverConfig::Full3D;
        }
    }

    if(configManager.DriverType() == GsDriverConfig::Full3D)
    {
        return DriverEntryFull3D(DriverObject, RegistryPath);
    }
    else if(configManager.DriverType() == GsDriverConfig::DisplayOnly)
    {
        return DriverEntryDisplayOnly(DriverObject, RegistryPath);
    }
    else if(configManager.DriverType() == GsDriverConfig::ResilientDisplayOnly)
    {
        return DriverEntryResilientDisplayOnly(DriverObject, RegistryPath);
    }

    return STATUS_FAILED_DRIVER_ENTRY;
}

#ifdef __cplusplus
extern "C" {
#endif

_Use_decl_annotations_ NTSTATUS DriverEntry(
    IN PDRIVER_OBJECT DriverObject, 
    IN PUNICODE_STRING RegistryPath
)
{
    __security_init_cookie();
    return DriverEntryReal(DriverObject, RegistryPath);
}

#ifdef __cplusplus
} /* extern "C" */
#endif

// See https://docs.microsoft.com/en-us/windows-hardware/drivers/display/driverentry-of-display-miniport-driver
#ifdef HY_BUILD_AS_KMDOD
  #undef HY_BUILD_AS_KMDOD
#endif

#ifdef DXGKDDI_INTERFACE_VERSION
  #undef DXGKDDI_INTERFACE_VERSION
#endif

#define HY_BUILD_AS_KMDOD 1
#include "Common.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <process.h>

#ifdef __cplusplus
} /* extern "C" */
#endif

_Use_decl_annotations_ NTSTATUS DriverEntryResilientDisplayOnly(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
    PAGED_CODE();

    //
    // Opt-in to using non-executable pool memory on Windows 8 and later.
    // This has to be done before any allocations (which our logging functions can potentially do).
    // https://msdn.microsoft.com/en-us/library/windows/hardware/hh920402(v=vs.85).aspx
    //
    ExInitializeDriverRuntime(DrvRtPoolNxOptIn);

    CHECK_IRQL(PASSIVE_LEVEL);

    LOG_DEBUG("DriverEntryResilientDisplayOnly - " __TIMESTAMP__ ", WDDM Version: 0x%04X\n", DXGKDDI_INTERFACE_VERSION);

    // If DriverObject is null inform the kernel that the first parameter was invalid.
    // This should probably never happen.
    if(!ARGUMENT_PRESENT(DriverObject))
    {
        LOG_ERROR("Invalid Parameter to DriverEntryResilientDisplayOnly: DriverObject\n");
        return STATUS_INVALID_PARAMETER_1;
    }

    // If RegistryPath is null inform the kernel that the second parameter was invalid.
    // This should probably never happen.
    if(!ARGUMENT_PRESENT(RegistryPath))
    {
        LOG_ERROR("Invalid Parameter to DriverEntryResilientDisplayOnly: RegistryPath\n");
        return STATUS_INVALID_PARAMETER_2;
    }

    return STATUS_FAILED_DRIVER_ENTRY;
}

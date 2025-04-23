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

#pragma code_seg(push)
#pragma code_seg("INIT")
#ifdef __cplusplus
extern "C" {
#endif
DRIVER_INITIALIZE DriverEntry;
#ifdef __cplusplus
} /* extern "C" */
#endif

struct DriverConfig final
{
    enum EDriverType : DWORD
    {
        Undefined = 0,
        Full3D,
        DisplayOnly,
        ResilientDisplayOnly,
        Disabled
    };

    EDriverType UserControlDriverType;
    DWORD FullDriverErrorMask;
    DWORD DisplayOnlyDriverErrorMask;
    DWORD ResilientDisplayOnlyDriverErrorMask;
    EDriverType DriverType;
};

// Is this the fastest method? Probably; any "optimization" you do just prevents the compiler from automatically generating the popcnt instruction.
// https://www.youtube.com/watch?v=bSkpMdDe4g4
constexpr static UINT PopCount(UINT Value) noexcept
{
    UINT count = 0;
    for(; Value != 0; ++count)
    {
        Value &= Value - 1;
    }

    return count;
}

static ULONG GetKeySize(HANDLE hRegistryKey, UNICODE_STRING* const pKeyName) noexcept
{
    if(!hRegistryKey)
    {
        return 0;
    }

    if(!pKeyName)
    {
        return 0;
    }

    ULONG keySize;
    NTSTATUS status = ZwQueryValueKey(hRegistryKey, pKeyName, KeyValueFullInformation, 0, 0, &keySize);
    if(status == STATUS_BUFFER_TOO_SMALL || status == STATUS_BUFFER_OVERFLOW)
    {
        return keySize;
    }

    LOG_ERROR("Failed to get key size: 0x%08X\n", status);

    return 0;
}

template<typename T>
static NTSTATUS ReadDriverConfigT(HANDLE hRegistryKey, UNICODE_STRING* const pKeyName, T* pData) noexcept
{
    if(!hRegistryKey)
    {
        return STATUS_INVALID_PARAMETER_1;
    }

    if(!pKeyName)
    {
        return STATUS_INVALID_PARAMETER_2;
    }

    if(!pData)
    {
        return STATUS_INVALID_PARAMETER_3;
    }

    const ULONG keySize = GetKeySize(hRegistryKey, pKeyName);
    if(keySize == 0)
    {
        return STATUS_NOT_FOUND;
    }

    if(keySize != sizeof(T))
    {
        return STATUS_INVALID_BUFFER_SIZE;
    }

    const PKEY_VALUE_FULL_INFORMATION dwordFullInfo = static_cast<PKEY_VALUE_FULL_INFORMATION>(HyAllocateZeroed(NonPagedPool, keySize, POOL_TAG_KERNEL_DRIVER));
    if(!dwordFullInfo)
    {
        return STATUS_NO_MEMORY;
    }

    ULONG readSize;
    NTSTATUS status = ZwQueryValueKey(
        hRegistryKey, 
        pKeyName, 
        KeyValueFullInformation, 
        dwordFullInfo, 
        keySize, 
        &readSize
    );
    if(!NT_SUCCESS(status) || keySize != readSize)
    {
        HyDeallocate(dwordFullInfo, POOL_TAG_KERNEL_DRIVER);
        if(keySize != readSize)
        {
            return STATUS_INFO_LENGTH_MISMATCH;
        }
        return STATUS_UNSUCCESSFUL;
    }

    *pData = *reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(dwordFullInfo) + dwordFullInfo->DataOffset);
    HyDeallocate(dwordFullInfo, POOL_TAG_KERNEL_DRIVER);

    return STATUS_SUCCESS;
}

static bool IsTooManyErrors(const UINT mask) noexcept
{
    return (mask & 0x7) == 0x7 || PopCount(mask & 0xFF) > 4;
}

static NTSTATUS ReadDriverConfig(PUNICODE_STRING RegistryPath, DriverConfig& config) noexcept
{
    if(!RegistryPath)
    {
        return STATUS_INVALID_PARAMETER_1;
    }

    OBJECT_ATTRIBUTES objectAttributes { };
    InitializeObjectAttributes(&objectAttributes, RegistryPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, nullptr, nullptr);

    HANDLE hRegistryKey = nullptr;
    NTSTATUS status = ZwOpenKey(&hRegistryKey, KEY_QUERY_VALUE, &objectAttributes);
    if(status)
    {
        LOG_ERROR("Registry key failed to open: 0x%08X\n", status);
        return STATUS_FAILED_DRIVER_ENTRY;
    }

    {
        UNICODE_STRING keyName = RTL_CONSTANT_STRING(L"UC_DriverType");
        if(!NT_SUCCESS(ReadDriverConfigT(hRegistryKey, &keyName, &config.UserControlDriverType)))
        {
            config.UserControlDriverType = DriverConfig::Undefined;
        }
    }

    {
        UNICODE_STRING keyName = RTL_CONSTANT_STRING(L"FullErrorMask");
        if(!NT_SUCCESS(ReadDriverConfigT(hRegistryKey, &keyName, &config.FullDriverErrorMask)))
        {
            config.FullDriverErrorMask = 0;
        }
    }

    {
        UNICODE_STRING keyName = RTL_CONSTANT_STRING(L"DODErrorMask");
        if(!NT_SUCCESS(ReadDriverConfigT(hRegistryKey, &keyName, &config.DisplayOnlyDriverErrorMask)))
        {
            config.DisplayOnlyDriverErrorMask = 0;
        }
    }

    {
        UNICODE_STRING keyName = RTL_CONSTANT_STRING(L"ReDODErrorMask");
        if(!NT_SUCCESS(ReadDriverConfigT(hRegistryKey, &keyName, &config.ResilientDisplayOnlyDriverErrorMask)))
        {
            config.ResilientDisplayOnlyDriverErrorMask = 0;
        }
    }

    if(config.UserControlDriverType != DriverConfig::Undefined)
    {
        config.DriverType = config.UserControlDriverType;
    }
    else
    {
        config.DriverType = DriverConfig::Full3D;
    }

    bool disableFullDriver = false;
    bool disableDODDriver = false;
    bool disableResilientDODDriver = false;

    if(IsTooManyErrors(config.FullDriverErrorMask))
    {
        disableFullDriver = true;
    }

    if(IsTooManyErrors(config.DisplayOnlyDriverErrorMask))
    {
        disableDODDriver = true;
    }

    if(IsTooManyErrors(config.ResilientDisplayOnlyDriverErrorMask))
    {
        disableResilientDODDriver = true;
    }

    if(disableFullDriver && config.DriverType == DriverConfig::Full3D)
    {
        config.DriverType = DriverConfig::DisplayOnly;
    }

    if(disableDODDriver && config.DriverType == DriverConfig::DisplayOnly)
    {
        config.DriverType = DriverConfig::ResilientDisplayOnly;
    }

    if(disableResilientDODDriver && config.DriverType == DriverConfig::ResilientDisplayOnly)
    {
        config.DriverType = DriverConfig::Disabled;
    }

    (void) ZwClose(hRegistryKey);

    return STATUS_SUCCESS;
}

_Use_decl_annotations_ NTSTATUS DriverEntryFull3D(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath);
_Use_decl_annotations_ NTSTATUS DriverEntryDisplayOnly(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath);
_Use_decl_annotations_ NTSTATUS DriverEntryResilientDisplayOnly(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath);

_Use_decl_annotations_ NTSTATUS DriverEntryReal(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath) noexcept
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

    DriverConfig config { };

    {
        NTSTATUS status = ReadDriverConfig(RegistryPath, config);

        if(!NT_SUCCESS(status))
        {
            config.DriverType = DriverConfig::Full3D;
        }
    }

    if(config.DriverType == DriverConfig::Full3D)
    {
        return DriverEntryFull3D(DriverObject, RegistryPath);
    }
    else if(config.DriverType == DriverConfig::DisplayOnly)
    {
        return DriverEntryDisplayOnly(DriverObject, RegistryPath);
    }
    else if(config.DriverType == DriverConfig::ResilientDisplayOnly)
    {
        return DriverEntryResilientDisplayOnly(DriverObject, RegistryPath);
    }

    return STATUS_FAILED_DRIVER_ENTRY;
}

#ifdef __cplusplus
extern "C" {
#endif

_Use_decl_annotations_ NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
    __security_init_cookie();
    return DriverEntryReal(DriverObject, RegistryPath);
}

#ifdef __cplusplus
} /* extern "C" */
#endif

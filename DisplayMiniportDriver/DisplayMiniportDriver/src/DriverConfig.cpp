#include "DriverConfig.hpp"

#pragma code_seg(push)
#pragma code_seg("_KTEXT")

GsConfigManager& GsConfigManager::Instance() noexcept
{
    static GsConfigManager instance;
    return instance;
}

// Is this the fastest method? Probably; any "optimization" you do just prevents the compiler from automatically generating the popcnt instruction.
// https://www.youtube.com/watch?v=bSkpMdDe4g4
constexpr static UINT PopCount(
    UINT Value
) noexcept
{
    UINT count = 0;
    for(; Value != 0; ++count)
    {
        Value &= Value - 1;
    }

    return count;
}

static ULONG GetKeySize(
    HANDLE hRegistryKey,
    UNICODE_STRING* const pKeyName
) noexcept
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
static NTSTATUS ReadDriverConfigT(
    HANDLE hRegistryKey,
    UNICODE_STRING* const pKeyName,
    T* pData
) noexcept
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

static bool IsTooManyErrors(
    const UINT mask
) noexcept
{
    return (mask & 0x7) == 0x7 || PopCount(mask & 0xFF) > 4;
}

static UNICODE_STRING fullErrorMaskName = RTL_CONSTANT_STRING(L"FullErrorMask");
static UNICODE_STRING dodErrorMaskName = RTL_CONSTANT_STRING(L"DODErrorMask");
static UNICODE_STRING reDodErrorMaskName = RTL_CONSTANT_STRING(L"ReDODErrorMask");

NTSTATUS GsConfigManager::Init(
    PUNICODE_STRING RegistryPath
) noexcept
{
    if(!RegistryPath)
    {
        return STATUS_INVALID_PARAMETER_1;
    }

    if(!NT_SUCCESS(RtlUnicodeStringCopy(&m_RegistryPath, RegistryPath)))
    {
        return STATUS_FAILED_DRIVER_ENTRY;
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
        if(!NT_SUCCESS(ReadDriverConfigT(hRegistryKey, &keyName, &m_Config.UserControlDriverType)))
        {
            m_Config.UserControlDriverType = GsDriverConfig::Undefined;
        }
    }

    {
        if(!NT_SUCCESS(ReadDriverConfigT(hRegistryKey, &fullErrorMaskName, &m_Config.FullDriverErrorMask)))
        {
            m_Config.FullDriverErrorMask = 0;
        }
    }

    {
        if(!NT_SUCCESS(ReadDriverConfigT(hRegistryKey, &dodErrorMaskName, &m_Config.DisplayOnlyDriverErrorMask)))
        {
            m_Config.DisplayOnlyDriverErrorMask = 0;
        }
    }

    {
        if(!NT_SUCCESS(ReadDriverConfigT(hRegistryKey, &reDodErrorMaskName, &m_Config.ResilientDisplayOnlyDriverErrorMask)))
        {
            m_Config.ResilientDisplayOnlyDriverErrorMask = 0;
        }
    }

    if(m_Config.UserControlDriverType != GsDriverConfig::Undefined)
    {
        m_Config.DriverType = m_Config.UserControlDriverType;
    }
    else
    {
        m_Config.DriverType = GsDriverConfig::Full3D;
    }

    bool disableFullDriver = false;
    bool disableDODDriver = false;
    bool disableResilientDODDriver = false;

    if(IsTooManyErrors(m_Config.FullDriverErrorMask))
    {
        disableFullDriver = true;
    }

    if(IsTooManyErrors(m_Config.DisplayOnlyDriverErrorMask))
    {
        disableDODDriver = true;
    }

    if(IsTooManyErrors(m_Config.ResilientDisplayOnlyDriverErrorMask))
    {
        disableResilientDODDriver = true;
    }

    if(disableFullDriver && m_Config.DriverType == GsDriverConfig::Full3D)
    {
        m_Config.DriverType = GsDriverConfig::DisplayOnly;
    }

    if(disableDODDriver && m_Config.DriverType == GsDriverConfig::DisplayOnly)
    {
        m_Config.DriverType = GsDriverConfig::ResilientDisplayOnly;
    }

    if(disableResilientDODDriver && m_Config.DriverType == GsDriverConfig::ResilientDisplayOnly)
    {
        m_Config.DriverType = GsDriverConfig::Disabled;
    }

    // Shift over the current driver type error mask 1 bit, then set it to failed.
    // It will be cleared later.
    switch(m_Config.DriverType)
    {
        case GsDriverConfig::Full3D:
            m_Config.FullDriverErrorMask <<= 1;
            m_Config.FullDriverErrorMask |= 0x1;
            (void) ZwSetValueKey(
                hRegistryKey,
                &fullErrorMaskName,
                0,
                REG_DWORD,
                &m_Config.FullDriverErrorMask,
                sizeof(m_Config.FullDriverErrorMask)
            );
            break;
        case GsDriverConfig::DisplayOnly:
            m_Config.DisplayOnlyDriverErrorMask <<= 1;
            m_Config.DisplayOnlyDriverErrorMask |= 0x1;
            (void) ZwSetValueKey(
                hRegistryKey,
                &dodErrorMaskName,
                0,
                REG_DWORD,
                &m_Config.DisplayOnlyDriverErrorMask,
                sizeof(m_Config.DisplayOnlyDriverErrorMask)
            );
            break;
        case GsDriverConfig::ResilientDisplayOnly:
            m_Config.ResilientDisplayOnlyDriverErrorMask <<= 1;
            m_Config.ResilientDisplayOnlyDriverErrorMask |= 0x1;
            (void) ZwSetValueKey(
                hRegistryKey,
                &reDodErrorMaskName,
                0,
                REG_DWORD,
                &m_Config.ResilientDisplayOnlyDriverErrorMask,
                sizeof(m_Config.ResilientDisplayOnlyDriverErrorMask)
            );
            break;
        default: break;
    }

    (void) ZwClose(hRegistryKey);

    m_Initialized = true;

    return STATUS_SUCCESS;
}

void GsConfigManager::SetSuccessfulLaunch() noexcept
{
    //   The error trackers will not be correct if we did not initialize
    // successfully; thus, we should just exit.
    if(!m_Initialized)
    {
        return;
    }

    OBJECT_ATTRIBUTES objectAttributes { };
    InitializeObjectAttributes(&objectAttributes, &m_RegistryPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, nullptr, nullptr);

    HANDLE hRegistryKey = nullptr;
    NTSTATUS status = ZwOpenKey(&hRegistryKey, KEY_QUERY_VALUE, &objectAttributes);
    if(status)
    {
        LOG_ERROR("Registry key failed to open: 0x%08X\n", status);
        return;
    }

    // Clear the latest failure bit, this effectively says the launch was not a failure.
    switch(m_Config.DriverType)
    {
        case GsDriverConfig::Full3D:
            m_Config.FullDriverErrorMask &= ~0x1;
            (void) ZwSetValueKey(
                hRegistryKey,
                &fullErrorMaskName,
                0,
                REG_DWORD,
                &m_Config.FullDriverErrorMask,
                sizeof(m_Config.FullDriverErrorMask)
            );
            break;
        case GsDriverConfig::DisplayOnly:
            m_Config.DisplayOnlyDriverErrorMask &= ~0x1;
            (void) ZwSetValueKey(
                hRegistryKey,
                &dodErrorMaskName,
                0,
                REG_DWORD,
                &m_Config.DisplayOnlyDriverErrorMask,
                sizeof(m_Config.DisplayOnlyDriverErrorMask)
            );
            break;
        case GsDriverConfig::ResilientDisplayOnly:
            m_Config.ResilientDisplayOnlyDriverErrorMask &= ~0x1;
            (void) ZwSetValueKey(
                hRegistryKey,
                &reDodErrorMaskName,
                0,
                REG_DWORD,
                &m_Config.ResilientDisplayOnlyDriverErrorMask,
                sizeof(m_Config.ResilientDisplayOnlyDriverErrorMask)
            );
            break;
        default: break;
    }
}

#pragma code_seg(pop)

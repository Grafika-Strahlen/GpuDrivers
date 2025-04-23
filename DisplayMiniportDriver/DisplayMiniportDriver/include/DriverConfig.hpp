#pragma once

#include "Common.h"

struct GsDriverConfig final
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

class GsConfigManager final
{
    DEFAULT_CONSTRUCT(GsConfigManager);
    DEFAULT_DESTRUCT(GsConfigManager);
    DEFAULT_CM_PU(GsConfigManager);
public:
    static GsConfigManager& Instance() noexcept;
public:
    [[nodiscard]] const GsDriverConfig& Config() const noexcept { return m_Config; }
    [[nodiscard]] GsDriverConfig::EDriverType& DriverType() noexcept { return m_Config.DriverType; }
    [[nodiscard]] GsDriverConfig::EDriverType DriverType() const noexcept { return m_Config.DriverType; }

    NTSTATUS Init(
        PUNICODE_STRING RegistryPath
    ) noexcept;

    void SetSuccessfulLaunch() noexcept;
private:
private:
    bool m_Initialized = false;
    UNICODE_STRING m_RegistryPath;
    GsDriverConfig m_Config;
};

// See https://docs.microsoft.com/en-us/windows-hardware/drivers/display/driverentry-of-display-miniport-driver
#ifdef HY_BUILD_AS_KMDOD
  #undef HY_BUILD_AS_KMDOD
#endif

#if defined(DXGKDDI_INTERFACE_VERSION) && DXGKDDI_INTERFACE_VERSION < DXGKDDI_INTERFACE_VERSION_WIN8
#define GS_SKIP_DOD
#endif

#define HY_BUILD_AS_KMDOD 1
#include "Common.h"

#include "AddDevice.hpp"
#include "StartDevice.hpp"
#include "StopDevice.hpp"
#include "RemoveDevice.hpp"
#include "DispatchIoRequest.hpp"
#include "InterruptRoutine.hpp"
#include "DpcRoutine.hpp"
#include "QueryChildRelations.hpp"
#include "QueryChildStatus.hpp"
#include "QueryDeviceDescriptor.hpp"
#include "SetPowerState.hpp"
#include "NotifyAcpiEvent.hpp"
#include "ResetDevice.hpp"
#include "QueryInterface.hpp"
#include "ControlEtwLogging.hpp"
#include "QueryAdapterInfo.hpp"

#include "CollectDbgInfo.hpp"
#include "IsSupportedVidPn.hpp"
#include "RecommendFunctionalVidPn.hpp"
#include "EnumVidPnCofuncModality.hpp"

#include "SetVidPnSourceVisibility.hpp"

#include "CommitVidPn.hpp"
#include "UpdateActiveVidPnPresentPath.hpp"
#include "RecommendMonitorModes.hpp"
#include "GetScanLine.hpp"

#if HY_BUILD_AS_KMDOD
#include "PresentDisplayOnly.hpp"
#endif

#include "ControlInterrupt.hpp"

#include "StopDeviceAndReleasePostDisplayOwnership.hpp"

#include "DevThunks.hpp"

NTSTATUS DdiNoOpNTSTATUS();  // NOLINT(clang-diagnostic-strict-prototypes)
void DdiNoOpVoid();  // NOLINT(clang-diagnostic-strict-prototypes)
BOOLEAN DdiNoOpBool();  // NOLINT(clang-diagnostic-strict-prototypes)
void GsUnload();

#pragma code_seg(push)
#pragma code_seg("INIT")

_Use_decl_annotations_ NTSTATUS DriverEntryDisplayOnly(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
    PAGED_CODE();
    CHECK_IRQL(PASSIVE_LEVEL);

    LOG_DEBUG("DriverEntryDisplayOnly - " __TIMESTAMP__ ", WDDM Version: 0x%04X\n", DXGKDDI_INTERFACE_VERSION);

    // If DriverObject is null inform the kernel that the first parameter was invalid.
    // This should probably never happen.
    if(!ARGUMENT_PRESENT(DriverObject))
    {
        LOG_ERROR("Invalid Parameter to DriverEntryDisplayOnly: DriverObject\n");
        return STATUS_INVALID_PARAMETER_1;
    }

    // If RegistryPath is null inform the kernel that the second parameter was invalid.
    // This should probably never happen.
    if(!ARGUMENT_PRESENT(RegistryPath))
    {
        LOG_ERROR("Invalid Parameter to DriverEntryDisplayOnly: RegistryPath\n");
        return STATUS_INVALID_PARAMETER_2;
    }

#ifndef GS_SKIP_DOD
    // Allocate (on the stack) and zero out the list pointers required for Display Miniport Display-Only Driver.
    KMDDOD_INITIALIZATION_DATA driverInitializationData;
    (void) RtlZeroMemory(&driverInitializationData, sizeof(driverInitializationData));

    // Set the version to whatever is currently in the header we're compiling with.
    driverInitializationData.Version = DXGKDDI_INTERFACE_VERSION;

    driverInitializationData.DxgkDdiAddDevice = HyAddDevice;
    driverInitializationData.DxgkDdiStartDevice = HyStartDevice;
    driverInitializationData.DxgkDdiStopDevice = HyStopDevice;
    driverInitializationData.DxgkDdiRemoveDevice = HyRemoveDevice;
    driverInitializationData.DxgkDdiDispatchIoRequest = HyDispatchIoRequest;
    driverInitializationData.DxgkDdiInterruptRoutine = HyInterruptRoutine;
    driverInitializationData.DxgkDdiDpcRoutine = HyDpcRoutine;
    driverInitializationData.DxgkDdiQueryChildRelations = HyQueryChildRelations;
    driverInitializationData.DxgkDdiQueryChildStatus = HyQueryChildStatus;
    driverInitializationData.DxgkDdiQueryDeviceDescriptor = HyQueryDeviceDescriptor;
    driverInitializationData.DxgkDdiSetPowerState = HySetPowerState;
    driverInitializationData.DxgkDdiNotifyAcpiEvent = HyNotifyAcpiEvent;
    driverInitializationData.DxgkDdiResetDevice = HyResetDevice;
    driverInitializationData.DxgkDdiUnload = GsUnload;
    driverInitializationData.DxgkDdiQueryInterface = HyQueryInterface;
    driverInitializationData.DxgkDdiControlEtwLogging = HyControlEtwLogging;
    driverInitializationData.DxgkDdiQueryAdapterInfo = HyQueryAdapterInfo;

    driverInitializationData.DxgkDdiSetPalette = ThunkHySetPalette;
    driverInitializationData.DxgkDdiSetPointerPosition = ThunkHySetPointerPosition;
    driverInitializationData.DxgkDdiSetPointerShape = ThunkHySetPointerShape;
    driverInitializationData.DxgkDdiEscape = ThunkHyEscape;
    driverInitializationData.DxgkDdiCollectDbgInfo = HyCollectDbgInfo;
    driverInitializationData.DxgkDdiIsSupportedVidPn = HyIsSupportedVidPn;
    driverInitializationData.DxgkDdiRecommendFunctionalVidPn = HyRecommendFunctionalVidPn;
    driverInitializationData.DxgkDdiEnumVidPnCofuncModality = HyEnumVidPnCofuncModality;

    driverInitializationData.DxgkDdiSetVidPnSourceVisibility = HySetVidPnSourceVisibility;

    driverInitializationData.DxgkDdiCommitVidPn = HyCommitVidPn;
    driverInitializationData.DxgkDdiUpdateActiveVidPnPresentPath = HyUpdateActiveVidPnPresentPath;
    driverInitializationData.DxgkDdiRecommendMonitorModes = HyRecommendMonitorModes;
#if HY_KMDOD_ENABLE_VSYNC_INTERRUPTS
    driverInitializationData.DxgkDdiGetScanLine = HyGetScanLine;
#else
    driverInitializationData.DxgkDdiGetScanLine = nullptr;
#endif
    driverInitializationData.DxgkDdiQueryVidPnHWCapability = ThunkHyQueryVidPnCapability;

#if DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WIN8
    // driverInitializationData.DxgkDdiPresentDisplayOnly = HyPresentDisplayOnly;
#endif

    driverInitializationData.DxgkDdiStopDeviceAndReleasePostDisplayOwnership = HyStopDeviceAndReleasePostDisplayOwnership;

    driverInitializationData.DxgkDdiSystemDisplayEnable = ThunkHySystemDisplayEnable;
    driverInitializationData.DxgkDdiSystemDisplayWrite = ThunkHySystemDisplayWrite;

    driverInitializationData.DxgkDdiGetChildContainerId = ThunkHyGetChildContainerId;

#if HY_KMDOD_ENABLE_VSYNC_INTERRUPTS
    driverInitializationData.DxgkDdiControlInterrupt = HyControlInterrupt;
#else
    driverInitializationData.DxgkDdiControlInterrupt = nullptr;
#endif

    driverInitializationData.DxgkDdiSetPowerComponentFState = ThunkHySetPowerComponentFState;
    //driverInitializationData.DxgkDdiPowerRuntimeControlRequest = ThunkHyPowerRuntimeControlRequest;

    //driverInitializationData.DxgkDdiNotifySurpriseRemoval = ThunkHyNotifySurpriseRemoval;

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_0)
    //
    // Display only drivers support P-State management.
    //
    driverInitializationData.DxgkDdiPowerRuntimeSetDeviceHandle = (PDXGKDDI_POWERRUNTIMESETDEVICEHANDLE) DdiNoOpNTSTATUS;
#endif

    // Initialize DXGK and return the result back to the kernel.
    return DxgkInitializeDisplayOnlyDriver(DriverObject, RegistryPath, &driverInitializationData);
#else
    return STATUS_FAILED_DRIVER_ENTRY;
#endif
}

#pragma code_seg(pop)

// See https://docs.microsoft.com/en-us/windows-hardware/drivers/display/driverentry-of-display-miniport-driver
#include "Common.h"

#include "miniport/AddDevice.hpp"
#include "miniport/StartDevice.hpp"
#include "miniport/StopDevice.hpp"
#include "miniport/RemoveDevice.hpp"
#include "miniport/DispatchIoRequest.hpp"
#include "miniport/InterruptRoutine.hpp"
#include "miniport/DpcRoutine.hpp"
#include "miniport/QueryChildRelations.hpp"
#include "miniport/QueryChildStatus.hpp"
#include "miniport/QueryDeviceDescriptor.hpp"
#include "miniport/SetPowerState.hpp"
#include "miniport/NotifyAcpiEvent.hpp"
#include "miniport/ResetDevice.hpp"
#include "miniport/QueryInterface.hpp"
#include "miniport/ControlEtwLogging.hpp"
#include "miniport/QueryAdapterInfo.hpp"
#include "miniport/CreateDevice.hpp"
#include "miniport/CreateAllocation.hpp"
#include "miniport/DestroyAllocation.hpp"

#include "miniport/CollectDbgInfo.hpp"
#include "miniport/IsSupportedVidPn.hpp"
#include "miniport/RecommendFunctionalVidPn.hpp"
#include "miniport/EnumVidPnCofuncModality.hpp"
#include "miniport/SetVidPnSourceAddress.hpp"
#include "miniport/SetVidPnSourceVisibility.hpp"

#include "miniport/CommitVidPn.hpp"
#include "miniport/UpdateActiveVidPnPresentPath.hpp"
#include "miniport/RecommendMonitorModes.hpp"
#include "miniport/GetScanLine.hpp"

#if HY_BUILD_AS_KMDOD
#include "miniport/PresentDisplayOnly.hpp"
#endif

#include "miniport/ControlInterrupt.hpp"
#include "miniport/DestroyDevice.hpp"
#include "miniport/OpenAllocation.hpp"
#include "miniport/CloseAllocation.hpp"

#include "miniport/CreateContext.hpp"
#include "miniport/DestroyContext.hpp"

#include "miniport/StopDeviceAndReleasePostDisplayOwnership.hpp"

#include "miniport/DevThunks.hpp"

#pragma code_seg(push)
#pragma code_seg("PAGE")

#if !HY_BUILD_AS_KMDOD
static void PrefillDriverInitializationData(DRIVER_INITIALIZATION_DATA& driverInitializationData) noexcept;
#endif

NTSTATUS DdiNoOpNTSTATUS()  // NOLINT(clang-diagnostic-strict-prototypes)
{
    LOG_DEBUG("DdiNoOpNTSTATUS\n");
    return STATUS_SUCCESS;
}

void DdiNoOpVoid()  // NOLINT(clang-diagnostic-strict-prototypes)
{
    LOG_DEBUG("DdiNoOpVoid\n");
}

BOOLEAN DdiNoOpBool()  // NOLINT(clang-diagnostic-strict-prototypes)
{
    LOG_DEBUG("DdiNoOpBool\n");
    return TRUE;
}

void GsUnload()
{
    CHECK_IRQL(PASSIVE_LEVEL);

    LOG_DEBUG("HyUnload\n");
}

#pragma code_seg(pop)

#pragma code_seg(push)
#pragma code_seg("INIT")
_Use_decl_annotations_ NTSTATUS DriverEntryFull3D(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
    PAGED_CODE();
    CHECK_IRQL(PASSIVE_LEVEL);

    LOG_DEBUG("DriverEntryReal - " __TIMESTAMP__ ", WDDM Version: 0x%04X\n", DXGKDDI_INTERFACE_VERSION);

    // If DriverObject is null inform the kernel that the first parameter was invalid.
    // This should probably never happen.
    if(!ARGUMENT_PRESENT(DriverObject))
    {
        LOG_ERROR("Invalid Parameter to DriverEntryFull3D: DriverObject\n");
        return STATUS_INVALID_PARAMETER_1;
    }

    // If RegistryPath is null inform the kernel that the second parameter was invalid.
    // This should probably never happen.
    if(!ARGUMENT_PRESENT(RegistryPath))
    {
        LOG_ERROR("Invalid Parameter to DriverEntryFull3D: RegistryPath\n");
        return STATUS_INVALID_PARAMETER_2;
    }

#if HY_BUILD_AS_KMDOD
    // Allocate (on the stack) and zero out the list pointers required for Display Miniport Display-Only Driver.
    KMDDOD_INITIALIZATION_DATA driverInitializationData;
    (void) RtlZeroMemory(&driverInitializationData, sizeof(driverInitializationData));

    // Set the version to whatever is currently in the header we're compiling with.
    driverInitializationData.Version = DXGKDDI_INTERFACE_VERSION;

    driverInitializationData.DxgkDdiAddDevice = GsAddDevice;
    driverInitializationData.DxgkDdiStartDevice = GsStartDevice;
    driverInitializationData.DxgkDdiStopDevice = GsStopDevice;
    driverInitializationData.DxgkDdiRemoveDevice = GsRemoveDevice;
    driverInitializationData.DxgkDdiDispatchIoRequest = GsDispatchIoRequest;
    driverInitializationData.DxgkDdiInterruptRoutine = GsInterruptRoutine;
    driverInitializationData.DxgkDdiDpcRoutine = GsDpcRoutine;
    driverInitializationData.DxgkDdiQueryChildRelations = GsQueryChildRelations;
    driverInitializationData.DxgkDdiQueryChildStatus = GsQueryChildStatus;
    driverInitializationData.DxgkDdiQueryDeviceDescriptor = GsQueryDeviceDescriptor;
    driverInitializationData.DxgkDdiSetPowerState = GsSetPowerState;
    driverInitializationData.DxgkDdiNotifyAcpiEvent = GsNotifyAcpiEvent;
    driverInitializationData.DxgkDdiResetDevice = GsResetDevice;
    driverInitializationData.DxgkDdiUnload = GsUnload;
    driverInitializationData.DxgkDdiQueryInterface = GsQueryInterface;
    driverInitializationData.DxgkDdiControlEtwLogging = GsControlEtwLogging;
    driverInitializationData.DxgkDdiQueryAdapterInfo = GsQueryAdapterInfo;

    driverInitializationData.DxgkDdiSetPalette = ThunkGsSetPalette;
    driverInitializationData.DxgkDdiSetPointerPosition = ThunkGsSetPointerPosition;
    driverInitializationData.DxgkDdiSetPointerShape = ThunkGsSetPointerShape;
    driverInitializationData.DxgkDdiEscape = ThunkGsEscape;
    driverInitializationData.DxgkDdiCollectDbgInfo = GsCollectDbgInfo;
    driverInitializationData.DxgkDdiIsSupportedVidPn = GsIsSupportedVidPn;
    driverInitializationData.DxgkDdiRecommendFunctionalVidPn = GsRecommendFunctionalVidPn;
    driverInitializationData.DxgkDdiEnumVidPnCofuncModality = GsEnumVidPnCofuncModality;

    driverInitializationData.DxgkDdiSetVidPnSourceVisibility = GsSetVidPnSourceVisibility;

    driverInitializationData.DxgkDdiCommitVidPn = GsCommitVidPn;
    driverInitializationData.DxgkDdiUpdateActiveVidPnPresentPath = GsUpdateActiveVidPnPresentPath;
    driverInitializationData.DxgkDdiRecommendMonitorModes = GsRecommendMonitorModes;
#if HY_KMDOD_ENABLE_VSYNC_INTERRUPTS
    driverInitializationData.DxgkDdiGetScanLine = GsGetScanLine;
#else
    driverInitializationData.DxgkDdiGetScanLine = nullptr;
#endif
    driverInitializationData.DxgkDdiQueryVidPnHWCapability = ThunkGsQueryVidPnCapability;

#if DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WIN8
    driverInitializationData.DxgkDdiPresentDisplayOnly = GsPresentDisplayOnly;
#endif

    driverInitializationData.DxgkDdiStopDeviceAndReleasePostDisplayOwnership = GsStopDeviceAndReleasePostDisplayOwnership;

    driverInitializationData.DxgkDdiSystemDisplayEnable = ThunkGsSystemDisplayEnable;
    driverInitializationData.DxgkDdiSystemDisplayWrite = ThunkGsSystemDisplayWrite;

    driverInitializationData.DxgkDdiGetChildContainerId = ThunkGsGetChildContainerId;

#if HY_KMDOD_ENABLE_VSYNC_INTERRUPTS
    driverInitializationData.DxgkDdiControlInterrupt = GsControlInterrupt;
#else
    driverInitializationData.DxgkDdiControlInterrupt = nullptr;
#endif

    driverInitializationData.DxgkDdiSetPowerComponentFState = ThunkGsSetPowerComponentFState;
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
    // Allocate (on the stack) and zero out the list pointers required for Display Miniport Driver.
    DRIVER_INITIALIZATION_DATA driverInitializationData;
    (void) RtlZeroMemory(&driverInitializationData, sizeof(driverInitializationData));
    PrefillDriverInitializationData(driverInitializationData);

    // Set the version to whatever is currently in the header we're compiling with.
    driverInitializationData.Version = DXGKDDI_INTERFACE_VERSION;
    // driverInitializationData.Version = DXGKDDI_INTERFACE_VERSION_VISTA;

    driverInitializationData.DxgkDdiAddDevice = GsAddDevice;
    driverInitializationData.DxgkDdiStartDevice = GsStartDevice;
    driverInitializationData.DxgkDdiStopDevice = GsStopDevice;
    driverInitializationData.DxgkDdiRemoveDevice = GsRemoveDevice;
    driverInitializationData.DxgkDdiDispatchIoRequest = GsDispatchIoRequest;
    driverInitializationData.DxgkDdiInterruptRoutine = GsInterruptRoutine;
    driverInitializationData.DxgkDdiDpcRoutine = GsDpcRoutine;
    driverInitializationData.DxgkDdiQueryChildRelations = GsQueryChildRelations;
    driverInitializationData.DxgkDdiQueryChildStatus = GsQueryChildStatus;
    driverInitializationData.DxgkDdiQueryDeviceDescriptor = GsQueryDeviceDescriptor;
    driverInitializationData.DxgkDdiSetPowerState = GsSetPowerState;
    driverInitializationData.DxgkDdiNotifyAcpiEvent = GsNotifyAcpiEvent;
    driverInitializationData.DxgkDdiResetDevice = GsResetDevice;
    driverInitializationData.DxgkDdiUnload = GsUnload;
    driverInitializationData.DxgkDdiQueryInterface = GsQueryInterface;
    driverInitializationData.DxgkDdiControlEtwLogging = GsControlEtwLogging;

    driverInitializationData.DxgkDdiQueryAdapterInfo = GsQueryAdapterInfo;
    driverInitializationData.DxgkDdiCreateDevice = GsCreateDevice;
    driverInitializationData.DxgkDdiCreateAllocation = GsCreateAllocation;
    driverInitializationData.DxgkDdiDestroyAllocation = GsDestroyAllocation;
    // driverInitializationData.DxgkDdiDescribeAllocation = (PDXGKDDI_DESCRIBEALLOCATION) DdiNoOpNTSTATUS;
    // driverInitializationData.DxgkDdiGetStandardAllocationDriverData = (PDXGKDDI_GETSTANDARDALLOCATIONDRIVERDATA) DdiNoOpNTSTATUS;
    // driverInitializationData.DxgkDdiAcquireSwizzlingRange = (PDXGKDDI_ACQUIRESWIZZLINGRANGE) DdiNoOpNTSTATUS;
    // driverInitializationData.DxgkDdiReleaseSwizzlingRange = (PDXGKDDI_RELEASESWIZZLINGRANGE) DdiNoOpNTSTATUS;
    // driverInitializationData.DxgkDdiPatch = (PDXGKDDI_PATCH) DdiNoOpNTSTATUS;
    // driverInitializationData.DxgkDdiSubmitCommand = (PDXGKDDI_SUBMITCOMMAND) DdiNoOpNTSTATUS;
    // driverInitializationData.DxgkDdiPreemptCommand = (PDXGKDDI_PREEMPTCOMMAND) DdiNoOpNTSTATUS;
    // driverInitializationData.DxgkDdiBuildPagingBuffer = (PDXGKDDI_BUILDPAGINGBUFFER) DdiNoOpNTSTATUS;
    driverInitializationData.DxgkDdiSetPalette = ThunkGsSetPalette;
    driverInitializationData.DxgkDdiSetPointerPosition = ThunkGsSetPointerPosition;
    driverInitializationData.DxgkDdiSetPointerShape = ThunkGsSetPointerShape;
    // driverInitializationData.DxgkDdiResetFromTimeout = (PDXGKDDI_RESETFROMTIMEOUT) DdiNoOpNTSTATUS;
    // driverInitializationData.DxgkDdiRestartFromTimeout = (PDXGKDDI_RESTARTFROMTIMEOUT) DdiNoOpNTSTATUS;
    driverInitializationData.DxgkDdiEscape = ThunkGsEscape;
    driverInitializationData.DxgkDdiCollectDbgInfo = GsCollectDbgInfo;
    // driverInitializationData.DxgkDdiQueryCurrentFence = (PDXGKDDI_QUERYCURRENTFENCE) DdiNoOpNTSTATUS;
    driverInitializationData.DxgkDdiIsSupportedVidPn = GsIsSupportedVidPn;
    driverInitializationData.DxgkDdiRecommendFunctionalVidPn = GsRecommendFunctionalVidPn;
    driverInitializationData.DxgkDdiEnumVidPnCofuncModality = GsEnumVidPnCofuncModality;
    driverInitializationData.DxgkDdiSetVidPnSourceAddress = GsSetVidPnSourceAddress;
    driverInitializationData.DxgkDdiSetVidPnSourceVisibility = GsSetVidPnSourceVisibility;
    driverInitializationData.DxgkDdiCommitVidPn = GsCommitVidPn;
    driverInitializationData.DxgkDdiUpdateActiveVidPnPresentPath = GsUpdateActiveVidPnPresentPath;
    driverInitializationData.DxgkDdiRecommendMonitorModes = GsRecommendMonitorModes;
    // driverInitializationData.DxgkDdiRecommendVidPnTopology = (PDXGKDDI_RECOMMENDVIDPNTOPOLOGY) DdiNoOpNTSTATUS;
    driverInitializationData.DxgkDdiGetScanLine = GsGetScanLine;
    // driverInitializationData.DxgkDdiStopCapture = (PDXGKDDI_STOPCAPTURE) DdiNoOpNTSTATUS;
    driverInitializationData.DxgkDdiControlInterrupt = GsControlInterrupt;
    // driverInitializationData.DxgkDdiCreateOverlay = (PDXGKDDI_CREATEOVERLAY) DdiNoOpNTSTATUS;
    //
    driverInitializationData.DxgkDdiDestroyDevice = GsDestroyDevice;
    driverInitializationData.DxgkDdiOpenAllocation = GsOpenAllocation;
    driverInitializationData.DxgkDdiCloseAllocation = GsCloseAllocation;
    // driverInitializationData.DxgkDdiRender = (PDXGKDDI_RENDER) DdiNoOpNTSTATUS;
    // driverInitializationData.DxgkDdiPresent = (PDXGKDDI_PRESENT) DdiNoOpNTSTATUS;
    //
    // driverInitializationData.DxgkDdiUpdateOverlay = (PDXGKDDI_UPDATEOVERLAY) DdiNoOpNTSTATUS;
    // driverInitializationData.DxgkDdiFlipOverlay = (PDXGKDDI_FLIPOVERLAY) DdiNoOpNTSTATUS;
    // driverInitializationData.DxgkDdiDestroyOverlay = (PDXGKDDI_DESTROYOVERLAY) DdiNoOpNTSTATUS;
    //
    driverInitializationData.DxgkDdiCreateContext = GsCreateContext;
    driverInitializationData.DxgkDdiDestroyContext = GsDestroyContext;

    // driverInitializationData.DxgkDdiLinkDevice = (PDXGKDDI_LINK_DEVICE) DdiNoOpNTSTATUS;
    // driverInitializationData.DxgkDdiSetDisplayPrivateDriverFormat = (PDXGKDDI_SETDISPLAYPRIVATEDRIVERFORMAT) DdiNoOpNTSTATUS;

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WIN8)
    driverInitializationData.DxgkDdiSetPowerComponentFState = ThunkGsSetPowerComponentFState;

    driverInitializationData.DxgkDdiStopDeviceAndReleasePostDisplayOwnership = GsStopDeviceAndReleasePostDisplayOwnership;

    driverInitializationData.DxgkDdiSystemDisplayEnable = ThunkGsSystemDisplayEnable;
    driverInitializationData.DxgkDdiSystemDisplayWrite = ThunkGsSystemDisplayWrite;
#endif

    // Initialize DXGK and return the result back to the kernel.
    return DxgkInitialize(DriverObject, RegistryPath, &driverInitializationData);
#endif
}

#pragma code_seg(pop)

#pragma code_seg(push)
#pragma code_seg("PAGE")

#define GEN_NOOP_NTSTATUS(PTR) \
    { \
        using fun_t = NTSTATUS(*)(); \
        fun_t fun = []() -> NTSTATUS \
        { \
            LOG_DEBUG(#PTR ## "\n"); \
            return STATUS_SUCCESS; \
        }; \
        (PTR) = reinterpret_cast<decltype(PTR)>(fun); \
    }

#if !HY_BUILD_AS_KMDOD
static void PrefillDriverInitializationData(DRIVER_INITIALIZATION_DATA& driverInitializationData) noexcept
{
    // GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiCreateDevice);
    // GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiCreateAllocation);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiDestroyAllocation);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiDescribeAllocation);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiGetStandardAllocationDriverData);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiAcquireSwizzlingRange);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiReleaseSwizzlingRange);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiPatch);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSubmitCommand);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiPreemptCommand);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiBuildPagingBuffer);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiResetFromTimeout);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiRestartFromTimeout);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiQueryCurrentFence);

    // GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSetVidPnSourceAddress);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiUpdateActiveVidPnPresentPath);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiRecommendMonitorModes);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiRecommendVidPnTopology);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiStopCapture);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiCreateOverlay);

    // GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiDestroyDevice);
    // GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiOpenAllocation);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiCloseAllocation);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiRender);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiPresent);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiUpdateOverlay);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiFlipOverlay);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiDestroyOverlay);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiCreateContext);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiDestroyContext);
    // This is an optional function that should only be supported by Linked Display Adapters.
    // GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiLinkDevice);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSetDisplayPrivateDriverFormat);

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WIN7)
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiDescribePageTable);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiUpdatePageTable);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiUpdatePageDirectory);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiMovePageDirectory);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSubmitRender);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiCreateAllocation2);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiRenderKm);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiQueryVidPnHWCapability);
#endif

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WIN8)
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiQueryDependentEngineGroup);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiQueryEngineStatus);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiResetEngine);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiCancelCommand);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiGetChildContainerId);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiPowerRuntimeControlRequest);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSetVidPnSourceAddressWithMultiPlaneOverlay);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiNotifySurpriseRemoval);
#endif

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM1_3)
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiGetNodeMetadata);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSetPowerPState);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiControlInterrupt2);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiCheckMultiPlaneOverlaySupport);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiCalibrateGpuClock);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiFormatHistoryBuffer);
#endif

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_0)
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiRenderGdi);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSubmitCommandVirtual);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSetRootPageTable);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiGetRootPageTableSize);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiMapCpuHostAperture);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiUnmapCpuHostAperture);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiCheckMultiPlaneOverlaySupport2);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiCreateProcess);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiDestroyProcess);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSetVidPnSourceAddressWithMultiPlaneOverlay2);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiPowerRuntimeSetDeviceHandle);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSetStablePowerState);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSetVideoProtectedRegion);
#endif

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_1)
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiCheckMultiPlaneOverlaySupport3);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSetVidPnSourceAddressWithMultiPlaneOverlay3);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiPostMultiPlaneOverlayPresent);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiValidateUpdateAllocationProperty);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiControlModeBehavior);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiUpdateMonitorLinkInfo);
#endif

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_2)
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiCreateHwContext);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiDestroyHwContext);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiCreateHwQueue);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiDestroyHwQueue);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSubmitCommandToHwQueue);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSwitchToHwContextList);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiResetHwEngine);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiCreatePeriodicFrameNotification);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiDestroyPeriodicFrameNotification);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSetTimingsFromVidPn);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSetTargetGamma);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSetTargetContentType);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSetTargetAnalogCopyProtection);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSetTargetAdjustedColorimetry);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiDisplayDetectControl);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiQueryConnectionChange);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiExchangePreStartInfo);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiGetMultiPlaneOverlayCaps);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiGetPostCompositionCaps);
#endif

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_3)
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiUpdateHwContextState);

    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiCreateProtectedSession);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiDestroyProtectedSession);
#endif

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_4)
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSetSchedulingLogBuffer);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSetupPriorityBands);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiNotifyFocusPresent);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSetContextSchedulingProperties);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSuspendContext);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiResumeContext);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSetVirtualMachineData);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiBeginExclusiveAccess);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiEndExclusiveAccess);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiQueryDiagnosticTypesSupport);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiControlDiagnosticReporting);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiResumeHwEngine);
#endif

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_5)
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSignalMonitoredFence);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiPresentToHwQueue);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiValidateSubmitCommand);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSetTargetAdjustedColorimetry2);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSetTrackedWorkloadPowerLevel);
#endif

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_6)
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSaveMemoryForHotUpdate);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiRestoreMemoryForHotUpdate);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiCollectDiagnosticInfo);
#endif

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_7)
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiControlInterrupt3);
#endif

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM2_9)
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSetFlipQueueLogBuffer);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiUpdateFlipQueueLog);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiCancelQueuedFlips);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSetInterruptTargetPresentId);
#endif

#if (DXGKDDI_INTERFACE_VERSION >= DXGKDDI_INTERFACE_VERSION_WDDM3_0)
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiSetAllocationBackingStore);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiCreateCpuEvent);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiDestroyCpuEvent);
    GEN_NOOP_NTSTATUS(driverInitializationData.DxgkDdiCancelFlips);
#endif
}
#endif
#pragma code_seg(pop)

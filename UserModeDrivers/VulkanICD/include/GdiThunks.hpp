#pragma once

#include <Windows.h>
#include <d3dkmthk.h>

extern PFND3DKMT_CREATEALLOCATION GDICreateAllocation;
extern PFND3DKMT_DESTROYALLOCATION GDIDestroyAllocation;
extern PFND3DKMT_SETALLOCATIONPRIORITY GDISetAllocationPriority;
extern PFND3DKMT_QUERYALLOCATIONRESIDENCY GDIQueryAllocationResidency;
extern PFND3DKMT_QUERYRESOURCEINFO GDIQueryResourceInfo;
extern PFND3DKMT_OPENRESOURCE GDIOpenResource;
extern PFND3DKMT_CREATEDEVICE GDICreateDevice;
extern PFND3DKMT_DESTROYDEVICE GDIDestroyDevice;
extern PFND3DKMT_QUERYADAPTERINFO GDIQueryAdapterInfo;
extern PFND3DKMT_LOCK GDILock;
extern PFND3DKMT_UNLOCK GDIUnlock;
extern PFND3DKMT_GETDISPLAYMODELIST GDIGetDisplayModeList;
extern PFND3DKMT_SETDISPLAYMODE GDISetDisplayMode;
extern PFND3DKMT_GETMULTISAMPLEMETHODLIST GDIGetMultisampleMethodList;
extern PFND3DKMT_PRESENT GDIPresent;
extern PFND3DKMT_RENDER GDIRender;
extern PFND3DKMT_OPENADAPTERFROMHDC GDIOpenAdapterFromHdc;
extern PFND3DKMT_OPENADAPTERFROMDEVICENAME GDIOpenAdapterFromDeviceName;
extern PFND3DKMT_CLOSEADAPTER GDICloseAdapter;
extern PFND3DKMT_GETSHAREDPRIMARYHANDLE GDIGetSharedPrimaryHandle;
extern PFND3DKMT_ESCAPE GDIEscape;
extern PFND3DKMT_SETVIDPNSOURCEOWNER GDISetVidPnSourceOwner;

extern PFND3DKMT_CREATEOVERLAY GDICreateOverlay;
extern PFND3DKMT_UPDATEOVERLAY GDIUpdateOverlay;
extern PFND3DKMT_FLIPOVERLAY GDIFlipOverlay;
extern PFND3DKMT_DESTROYOVERLAY GDIDestroyOverlay;
extern PFND3DKMT_WAITFORVERTICALBLANKEVENT GDIWaitForVerticalBlankEvent;
extern PFND3DKMT_SETGAMMARAMP GDISetGammaRamp;
extern PFND3DKMT_GETDEVICESTATE GDIGetDeviceState;
extern PFND3DKMT_CREATEDCFROMMEMORY GDICreateDCFromMemory;
extern PFND3DKMT_DESTROYDCFROMMEMORY GDIDestroyDCFromMemory;
extern PFND3DKMT_SETCONTEXTSCHEDULINGPRIORITY GDISetContextSchedulingPriority;
extern PFND3DKMT_GETCONTEXTSCHEDULINGPRIORITY GDIGetContextSchedulingPriority;
extern PFND3DKMT_SETPROCESSSCHEDULINGPRIORITYCLASS GDISetProcessSchedulingPriorityClass;
extern PFND3DKMT_GETPROCESSSCHEDULINGPRIORITYCLASS GDIGetProcessSchedulingPriorityClass;
extern PFND3DKMT_RELEASEPROCESSVIDPNSOURCEOWNERS GDIReleaseProcessVidPnSourceOwners;
extern PFND3DKMT_GETSCANLINE GDIGetScanLine;
extern PFND3DKMT_POLLDISPLAYCHILDREN GDIPollDisplayChildren;
extern PFND3DKMT_SETQUEUEDLIMIT GDISetQueuedLimit;
extern PFND3DKMT_INVALIDATEACTIVEVIDPN GDIInvalidateActiveVidPn;
extern PFND3DKMT_CHECKOCCLUSION GDICheckOcclusion;
extern PFND3DKMT_GETPRESENTHISTORY GDIGetPresentHistory;
extern PFND3DKMT_CREATECONTEXT GDICreateContext;
extern PFND3DKMT_DESTROYCONTEXT GDIDestroyContext;
extern PFND3DKMT_CREATESYNCHRONIZATIONOBJECT GDICreateSynchronizationObject;
extern PFND3DKMT_DESTROYSYNCHRONIZATIONOBJECT GDIDestroySynchronizationObject;
extern PFND3DKMT_WAITFORSYNCHRONIZATIONOBJECT GDIWaitForSynchronizationObject;
extern PFND3DKMT_SIGNALSYNCHRONIZATIONOBJECT GDISignalSynchronizationObject;
extern PFND3DKMT_CHECKMONITORPOWERSTATE GDICheckMonitorPowerState;
extern PFND3DKMT_OPENADAPTERFROMGDIDISPLAYNAME GDIOpenAdapterFromGDIDisplayName;
extern PFND3DKMT_CHECKEXCLUSIVEOWNERSHIP GDICheckExclusiveOwnership;
extern PFND3DKMT_SETDISPLAYPRIVATEDRIVERFORMAT GDISetDisplayPrivateDriverFormat;
extern PFND3DKMT_SHAREDPRIMARYLOCKNOTIFICATION GDISharedPrimaryLockNotification;
extern PFND3DKMT_SHAREDPRIMARYUNLOCKNOTIFICATION GDISharedPrimaryUnLockNotification;

extern PFND3DKMT_ENUMADAPTERS GDIEnumAdapters;
extern PFND3DKMT_ENUMADAPTERS2 GDIEnumAdapters2;

[[nodiscard]] bool InitKernelThunks() noexcept;
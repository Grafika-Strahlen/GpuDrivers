#pragma once

#include "Common.hpp"
#include <Objects.hpp>


class GsDeviceContext10 final
{
    DEFAULT_CONSTRUCT_PU(GsDeviceContext10);
    DEFAULT_DESTRUCT(GsDeviceContext10);
    DEFAULT_MOVE(GsDeviceContext10);
    DELETE_COPY(GsDeviceContext10);
public:
    GsDeviceContext10(
        const HANDLE handle,
        void* commandBuffer,
        const UINT commandBufferSize,
        D3DDDI_ALLOCATIONLIST* allocationList,
        const UINT allocationListSize,
        D3DDDI_PATCHLOCATIONLIST* patchLocationList,
        const UINT patchLocationListSize
    ) noexcept;

    void RecordAutoSubmit() noexcept;
    void RecordManualSubmit() noexcept;
    void ResetSubmissionTracker() noexcept;

    [[nodiscard]] bool ShouldUpdateCommandBufferSize() const noexcept;
    [[nodiscard]] bool ShouldUpdateAllocationListSize() const noexcept;
    [[nodiscard]] bool ShouldUpdatePatchListSize() const noexcept;

    [[nodiscard]] HANDLE& Handle()       noexcept { return m_Handle; }
    [[nodiscard]] HANDLE  Handle() const noexcept { return m_Handle; }
    [[nodiscard]] void*& CommandBuffer()       noexcept { return m_CommandBuffer; }
    [[nodiscard]] void*  CommandBuffer() const noexcept { return m_CommandBuffer; }
    [[nodiscard]] UINT& CommandBufferSize()       noexcept { return m_CommandBufferSize; }
    [[nodiscard]] UINT  CommandBufferSize() const noexcept { return m_CommandBufferSize; }
    [[nodiscard]] D3DDDI_ALLOCATIONLIST*& AllocationList()       noexcept { return m_AllocationList; }
    [[nodiscard]] D3DDDI_ALLOCATIONLIST*  AllocationList() const noexcept { return m_AllocationList; }
    [[nodiscard]] UINT& AllocationListSize()       noexcept { return m_AllocationListSize; }
    [[nodiscard]] UINT  AllocationListSize() const noexcept { return m_AllocationListSize; }
    [[nodiscard]] D3DDDI_PATCHLOCATIONLIST*& PatchLocationList()       noexcept { return m_PatchLocationList; }
    [[nodiscard]] D3DDDI_PATCHLOCATIONLIST*  PatchLocationList() const noexcept { return m_PatchLocationList; }
    [[nodiscard]] UINT& PatchLocationListSize()       noexcept { return m_PatchLocationListSize; }
    [[nodiscard]] UINT  PatchLocationListSize() const noexcept { return m_PatchLocationListSize; }

    [[nodiscard]] UINT  CommandBufferSubmitTracker() const noexcept { return m_CommandBufferSubmitTracker;  }
    [[nodiscard]] UINT AllocationListSubmitTracker() const noexcept { return m_AllocationListSubmitTracker; }
    [[nodiscard]] UINT      PatchListSubmitTracker() const noexcept { return m_PatchListSubmitTracker;      }
private:
    [[nodiscard]] static bool ShouldUpdateSize(const UINT tracker) noexcept;
private:
    void RecordSubmit() noexcept;
private:
    HANDLE m_Handle;
    void* m_CommandBuffer;
    UINT m_CommandBufferSize;
    D3DDDI_ALLOCATIONLIST* m_AllocationList;
    UINT m_AllocationListSize;
    D3DDDI_PATCHLOCATIONLIST* m_PatchLocationList;
    UINT m_PatchLocationListSize;

    UINT m_CommandBufferSubmitTracker;
    UINT m_AllocationListSubmitTracker;
    UINT m_PatchListSubmitTracker;
};

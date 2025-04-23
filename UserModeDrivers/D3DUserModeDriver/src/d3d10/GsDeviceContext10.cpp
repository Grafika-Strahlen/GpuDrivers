#include "d3d10/GsDeviceContext10.hpp"
#include <bit>

GsDeviceContext10::GsDeviceContext10(
    const HANDLE handle,
    void* commandBuffer,
    const UINT commandBufferSize,
    D3DDDI_ALLOCATIONLIST* allocationList,
    const UINT allocationListSize,
    D3DDDI_PATCHLOCATIONLIST* patchLocationList,
    const UINT patchLocationListSize
) noexcept
    : m_Handle(handle)
    , m_CommandBuffer(commandBuffer)
    , m_CommandBufferSize(commandBufferSize)
    , m_AllocationList(allocationList)
    , m_AllocationListSize(allocationListSize)
    , m_PatchLocationList(patchLocationList)
    , m_PatchLocationListSize(patchLocationListSize)
    , m_CommandBufferSubmitTracker(0)
    , m_AllocationListSubmitTracker(0)
    , m_PatchListSubmitTracker(0)
{ }

void GsDeviceContext10::RecordAutoSubmit() noexcept
{
    RecordSubmit();
    m_CommandBufferSubmitTracker |= 1;
    m_AllocationListSubmitTracker |= 1;
    m_PatchListSubmitTracker |= 1;
}

void GsDeviceContext10::RecordManualSubmit() noexcept
{
    RecordSubmit();
}

void GsDeviceContext10::ResetSubmissionTracker() noexcept
{
    m_CommandBufferSubmitTracker = 0;
    m_AllocationListSubmitTracker = 0;
    m_PatchListSubmitTracker = 0;
}

bool GsDeviceContext10::ShouldUpdateCommandBufferSize() const noexcept
{
    return ShouldUpdateSize(m_CommandBufferSubmitTracker);
}

bool GsDeviceContext10::ShouldUpdateAllocationListSize() const noexcept
{
    return ShouldUpdateSize(m_AllocationListSubmitTracker);
}

bool GsDeviceContext10::ShouldUpdatePatchListSize() const noexcept
{
    return ShouldUpdateSize(m_PatchListSubmitTracker);
}

// Sourced from https://stackoverflow.com/a/2381064/5899776 (phkahler)
// And https://stackoverflow.com/a/2380818/5899776 (Ignacio Vazquez-Abrams)
static UINT CountTrailingOnes(const UINT i) noexcept
{
    UINT b = ~i & (i + 1);   // this gives a 1 to the left of the trailing 1's
    b--;              // this gets us just the trailing 1's that need counting
    b = (b & 0x55555555) + ((b >>  1) & 0x55555555); // 2  bit sums of 1 bit numbers
    b = (b & 0x33333333) + ((b >>  2) & 0x33333333); // 4  bit sums of 2 bit numbers
    b = (b & 0x0f0f0f0f) + ((b >>  4) & 0x0f0f0f0f); // 8  bit sums of 4 bit numbers
    b = (b & 0x00ff00ff) + ((b >>  8) & 0x00ff00ff); // 16 bit sums of 8 bit numbers
    b = (b & 0x0000ffff) + ((b >> 16) & 0x0000ffff); // sum of 16-bit numbers

    return b;
}

bool GsDeviceContext10::ShouldUpdateSize(const UINT tracker) noexcept
{
    //   If more than half of the submits have been from overflows, then we
    // should update the size.
    if(::std::popcount(tracker) > static_cast<int>(sizeof(UINT) * CHAR_BIT / 2))
    {
        return true;
    }

    //   If at least 3 submissions in a row were from overflows, then we
    // should resize the buffer. We only care about the last 3 submissions
    // in this case. This is both faster, and lets us avoid resetting the
    // tracker when resizing.
    if(CountTrailingOnes(tracker) >= 3)
    {
        return true;
    }

    return false;
}

void GsDeviceContext10::RecordSubmit() noexcept
{
    m_CommandBufferSubmitTracker <<= 1;
    m_AllocationListSubmitTracker <<= 1;
    m_PatchListSubmitTracker <<= 1;
}

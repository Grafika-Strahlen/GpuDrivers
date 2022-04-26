#include <ConPrinter.hpp>

#include "LogicalDeviceManager.hpp"
#include "ConfigMacros.hpp"
#include "InstanceManager.hpp"
#include "DriverAlignment.hpp"

namespace vk {

VkResult DriverVkCreateDevice(const VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* const pCreateInfo, const VkAllocationCallbacks* const pAllocator, VkDevice* const pDevice) noexcept
{
#if DRIVER_DEBUG_LOG
	ConPrinter::Print("Application is creating VkDevice.\n");
#endif


	// A block of memory to create our driver logical device from.
	void* placement;
	// Whether pAllocator was successfully used, this may be false if pAllocator returns null.
	bool isCustomAllocated = false;


	// pAllocator is not necessarily non-null. If it is not, we'll use the user supplied allocator.
	if(pAllocator)
	{
		// Allocate a block of memory for placement new.
		placement = pAllocator->pfnAllocation(pAllocator->pUserData, sizeof(DriverVkLogicalDevice), DriverVkAlignmentSz, VK_SYSTEM_ALLOCATION_SCOPE_DEVICE);

		// pAllocator can fail.
		if(!placement)
		{
			// Fallback to using an aligned new. We'll ensure alignment to the system word size (not was NASM calls WORD) because why not.
			placement = ::operator new(sizeof(DriverVkLogicalDevice), DriverVkAlignment, ::std::nothrow);

			// If new failed then our address space is entirely out of memory. Consider using 64 bit for 48 or 52 bits of address space.
			if(!placement)
			{
				return VK_ERROR_OUT_OF_HOST_MEMORY;
			}
		}
		else
		{
			// Custom allocation succeeded.
			isCustomAllocated = true;

			// The information callbacks are not necessarily non-null.
			if(pAllocator->pfnInternalAllocation)
			{
				// Inform the allocator about our allocation.
				pAllocator->pfnInternalAllocation(pAllocator->pUserData, sizeof(DriverVkLogicalDevice), VK_INTERNAL_ALLOCATION_TYPE_EXECUTABLE, VK_SYSTEM_ALLOCATION_SCOPE_DEVICE);
			}
		}
	}
	else // Use the C++ allocator.
	{
		// Call the more explicit new to get a raw void*. We'll ensure alignment to the system word size (not was NASM calls WORD) because why not.
		placement = ::operator new(sizeof(DriverVkLogicalDevice), DriverVkAlignment, ::std::nothrow);

		// If new failed then our virtual address space is entirely out of memory. Consider using 64-bit application for 48 or 52 bits of address space.
		if(!placement)
		{
			return VK_ERROR_OUT_OF_HOST_MEMORY;
		}
	}

	// Create and initialize our internal version of VkInstance;
	DriverVkLogicalDevice* const driverLogicalDevice = new(placement) DriverVkLogicalDevice(DriverVkPhysicalDevice::FromVkPhysicalDevice(physicalDevice), isCustomAllocated);

	*pDevice = reinterpret_cast<VkDevice>(driverLogicalDevice);

	return VK_SUCCESS;
}

void DriverVkDestroyDevice(const VkDevice device, const VkAllocationCallbacks* const pAllocator) noexcept
{
	// If device is null fast-exit.
	if(!device)
	{
		return;
	}

#if DRIVER_DEBUG_LOG
	ConPrinter::Print(u"Application is freeing VkDevice.\n");
#endif

	// Invoke the destructor as we're essentially going to free a void*.
	DriverVkLogicalDevice::FromVkDevice(device)->~DriverVkLogicalDevice();

	// If this was custom allocated we need to deallocate with the allocator callbacks.
	if(DriverVkLogicalDevice::IsCustomAllocated(device))
	{
		// We're allowed to assume that pAllocator is not null if instance was allocated with a custom allocator.
		pAllocator->pfnFree(pAllocator->pUserData, device);

		// The information callbacks are not necessarily non-null.
		if(pAllocator->pfnInternalFree)
		{
			// Inform the allocator about our freeing of an allocation.
			pAllocator->pfnInternalFree(pAllocator->pUserData, sizeof(DriverVkLogicalDevice), VK_INTERNAL_ALLOCATION_TYPE_EXECUTABLE, VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE);
		}
	}
	else // Call the C++ delete instead.
	{
		// We have to use the more explicit delete as we had set alignments, and also used the more explicit new.
		::operator delete(device, DriverVkAlignment, ::std::nothrow);
	}
}

}

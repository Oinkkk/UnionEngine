#include "PhysicalDevice.h"

namespace VK
{
	PhysicalDevice::PhysicalDevice(VulkanInstance &vulkanInstance, const VkPhysicalDevice handle) noexcept :
		Handle{ handle }, __vulkanInstance{ vulkanInstance }
	{}

	void PhysicalDevice::vkGetProperties2(VkPhysicalDeviceProperties2 *const pProperties) const noexcept
	{
		__vulkanInstance.vkGetPhysicalDeviceProperties2(getHandle(), pProperties);
	}

	void PhysicalDevice::vkGetQueueFamilyProperties(
		uint32_t *const pQueueFamilyPropertyCount,
		VkQueueFamilyProperties *const pQueueFamilyProperties) const noexcept
	{
		__vulkanInstance.vkGetPhysicalDeviceQueueFamilyProperties(
			getHandle(), pQueueFamilyPropertyCount, pQueueFamilyProperties);
	}

	VkBool32 PhysicalDevice::vkGetWin32PresentationSupportKHR(const uint32_t queueFamilyIndex) const noexcept
	{
		return __vulkanInstance.vkGetPhysicalDeviceWin32PresentationSupportKHR(getHandle(), queueFamilyIndex);
	}
}
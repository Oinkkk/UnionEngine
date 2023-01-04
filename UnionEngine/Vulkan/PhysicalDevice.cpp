#include "PhysicalDevice.h"

namespace VK
{
	PhysicalDevice::PhysicalDevice(VulkanInstance &vulkanInstance, const VkPhysicalDevice handle) noexcept :
		Handle{ handle }, __vulkanInstance{ vulkanInstance }
	{}

	void PhysicalDevice::vkGetPhysicalDeviceProperties2(
		VkPhysicalDeviceProperties2 *const pProperties) const noexcept
	{
		__vulkanInstance.vkGetPhysicalDeviceProperties2(getHandle(), pProperties);
	}
}
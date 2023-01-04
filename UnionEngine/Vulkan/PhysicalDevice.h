#pragma once

#include "VulkanInstance.h"

namespace VK
{
	class PhysicalDevice : public Infra::Handle<VkPhysicalDevice>
	{
	public:
		PhysicalDevice(VulkanInstance &vulkanInstance, const VkPhysicalDevice handle) noexcept;
		virtual ~PhysicalDevice() noexcept = default;

		void vkGetProperties2(VkPhysicalDeviceProperties2 *const pProperties) const noexcept;

		void vkGetQueueFamilyProperties(
			uint32_t *const pQueueFamilyPropertyCount,
			VkQueueFamilyProperties *const pQueueFamilyProperties) const noexcept;

		[[nodiscard]]
		VkBool32 vkGetWin32PresentationSupportKHR(const uint32_t queueFamilyIndex) const noexcept;

	private:
		VulkanInstance &__vulkanInstance;
	};
}
#pragma once

#include "VulkanInstance.h"

namespace VK
{
	class PhysicalDevice : public Infra::Handle<VkPhysicalDevice>
	{
	public:
		PhysicalDevice(VulkanInstance &vulkanInstance, const VkPhysicalDevice handle) noexcept;
		virtual ~PhysicalDevice() noexcept = default;

		void vkGetPhysicalDeviceProperties2(
			VkPhysicalDeviceProperties2 *const pProperties) const noexcept;

	private:
		VulkanInstance &__vulkanInstance;
	};
}
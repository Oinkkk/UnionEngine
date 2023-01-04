#pragma once

#include "../Vulkan/VulkanInstance.h"
#include <vector>

namespace Engine
{
	class PhysicalDevicePicker final
	{
	public:
		PhysicalDevicePicker() = delete;

		[[nodiscard]]
		static VkPhysicalDevice pick(const VK::VulkanInstance &vulkanInstance) noexcept;

	private:
		[[nodiscard]]
		static std::vector<VkPhysicalDevice> __retrieveDevices(const VK::VulkanInstance &vulkanInstance) noexcept;

		static std::vector<VkQueueFamilyProperties>
			__retrieveQueueFamilyProps(
				const VK::VulkanInstance &vulkanInstance, const VkPhysicalDevice physicalDevice) noexcept;

		[[nodiscard]]
		static bool __checkVersionSupport(
			const VkPhysicalDeviceProperties &prop) noexcept;

		[[nodiscard]]
		static bool __checkQueueSupport(
			const std::vector<VkQueueFamilyProperties> &queueFamilyProps) noexcept;

		[[nodiscard]]
		static uint32_t __getScoreOf(const VkPhysicalDeviceProperties &prop) noexcept;
	};
}
#pragma once

#include "../Vulkan/Instance.h"
#include <vector>

namespace Engine
{
	class PhysicalDevicePicker final
	{
	public:
		PhysicalDevicePicker() = delete;

		[[nodiscard]]
		static VkPhysicalDevice pick(VK::Instance &instance) noexcept;

	private:
		[[nodiscard]]
		static std::vector<VkPhysicalDevice> __retrieveDevices(VK::Instance &instance) noexcept;

		static std::vector<VkQueueFamilyProperties>
			__retrieveQueueFamilyProps(
			VK::Instance &instance, const VkPhysicalDevice physicalDevice) noexcept;

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
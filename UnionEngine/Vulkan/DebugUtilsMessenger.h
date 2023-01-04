#pragma once

#include "VulkanInstance.h"

namespace VK
{
	class DebugUtilsMessenger : public Infra::Handle<VkDebugUtilsMessengerEXT>, public Infra::Deletable
	{
	public:
		DebugUtilsMessenger(VulkanInstance &vulkanInstance, const VkDebugUtilsMessengerCreateInfoEXT &createInfo);
		virtual ~DebugUtilsMessenger() noexcept;

	private:
		VulkanInstance &__vulkanInstance;

		[[nodiscard]]
		static VkDebugUtilsMessengerEXT __create(
			VulkanInstance &instance, const VkDebugUtilsMessengerCreateInfoEXT &createInfo);
	};
}
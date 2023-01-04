#include "DebugUtilsMessenger.h"

namespace VK
{
	DebugUtilsMessenger::DebugUtilsMessenger(
		VulkanInstance &vulkanInstance, const VkDebugUtilsMessengerCreateInfoEXT &createInfo) :
		Handle{ __create(vulkanInstance, createInfo) }, __vulkanInstance{ vulkanInstance }
	{}


	DebugUtilsMessenger::~DebugUtilsMessenger() noexcept
	{
		__vulkanInstance.vkDestroyDebugUtilsMessengerEXT(getHandle(), nullptr);
	}

	VkDebugUtilsMessengerEXT DebugUtilsMessenger::__create(
		VulkanInstance &vulkanInstance, const VkDebugUtilsMessengerCreateInfoEXT &createInfo)
	{
		VkDebugUtilsMessengerEXT retVal{};
		vulkanInstance.vkCreateDebugUtilsMessengerEXT(&createInfo, nullptr, &retVal);

		if (!retVal)
			throw std::exception{ "Cannot create a VkDebugUtilsMessengerEXT." };

		return retVal;
	}
}
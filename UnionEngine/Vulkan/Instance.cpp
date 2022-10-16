#include "Instance.h"
#include <stdexcept>

namespace VK
{
	Instance::Instance(const VkInstanceCreateInfo &createInfo) :
		Handle{ __create(createInfo) }
	{
		__queryProc();
	}

	Instance::~Instance() noexcept
	{
		__proc.vkDestroyInstance(getHandle(), nullptr);
	}

	void Instance::__queryProc() noexcept
	{
		__proc.vkDestroyInstance = vkGetInstanceProcAddr<PFN_vkDestroyInstance>("vkDestroyInstance");
	}

	VkInstance Instance::__create(const VkInstanceCreateInfo &createInfo)
	{
		const GlobalProc &globalProc{ VulkanLoader::getInstance().getGlobalProc() };

		VkInstance retVal{};
		globalProc.vkCreateInstance(&createInfo, nullptr, &retVal);

		if (!retVal)
			throw std::runtime_error{ "Cannot create a VkInstance." };

		return retVal;
	}
}
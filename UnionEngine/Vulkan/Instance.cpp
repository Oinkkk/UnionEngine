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

	VkResult Instance::vkCreateDebugUtilsMessengerEXT(
		const VkDebugUtilsMessengerCreateInfoEXT *const pCreateInfo,
		const VkAllocationCallbacks *const pAllocator,
		VkDebugUtilsMessengerEXT *const pMessenger) noexcept
	{
		return __proc.vkCreateDebugUtilsMessengerEXT(getHandle(), pCreateInfo, pAllocator, pMessenger);
	}

	void Instance::vkDestroyDebugUtilsMessengerEXT(
		const VkDebugUtilsMessengerEXT messenger,
		const VkAllocationCallbacks *const pAllocator) noexcept
	{
		__proc.vkDestroyDebugUtilsMessengerEXT(getHandle(), messenger, pAllocator);
	}

	VkResult Instance::vkEnumeratePhysicalDevices(
		uint32_t *const pPhysicalDeviceCount,
		VkPhysicalDevice *const pPhysicalDevices) const noexcept
	{
		return __proc.vkEnumeratePhysicalDevices(getHandle(), pPhysicalDeviceCount, pPhysicalDevices);
	}

	void Instance::vkGetPhysicalDeviceProperties(
		const VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceProperties *const pProperties) const noexcept
	{
		__proc.vkGetPhysicalDeviceProperties(physicalDevice, pProperties);
	}

	void Instance::vkGetPhysicalDeviceQueueFamilyProperties(
		const VkPhysicalDevice physicalDevice,
		uint32_t *const pQueueFamilyPropertyCount,
		VkQueueFamilyProperties *const pQueueFamilyProperties) const noexcept
	{
		__proc.vkGetPhysicalDeviceQueueFamilyProperties(
			physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
	}

	void Instance::__queryProc() noexcept
	{
		__proc.vkDestroyInstance =
			vkGetInstanceProcAddr<PFN_vkDestroyInstance>("vkDestroyInstance");

		__proc.vkCreateDebugUtilsMessengerEXT =
			vkGetInstanceProcAddr<PFN_vkCreateDebugUtilsMessengerEXT>("vkCreateDebugUtilsMessengerEXT");

		__proc.vkDestroyDebugUtilsMessengerEXT =
			vkGetInstanceProcAddr<PFN_vkDestroyDebugUtilsMessengerEXT>("vkDestroyDebugUtilsMessengerEXT");

		__proc.vkEnumeratePhysicalDevices =
			vkGetInstanceProcAddr<PFN_vkEnumeratePhysicalDevices>("vkEnumeratePhysicalDevices");

		__proc.vkGetPhysicalDeviceProperties =
			vkGetInstanceProcAddr<PFN_vkGetPhysicalDeviceProperties>("vkGetPhysicalDeviceProperties");

		__proc.vkGetPhysicalDeviceQueueFamilyProperties =
			vkGetInstanceProcAddr<PFN_vkGetPhysicalDeviceQueueFamilyProperties>("vkGetPhysicalDeviceQueueFamilyProperties");
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
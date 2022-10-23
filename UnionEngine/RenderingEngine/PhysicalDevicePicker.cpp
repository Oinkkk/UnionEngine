#include "PhysicalDevicePicker.h"
#include <map>
#include <vector>

namespace Engine
{
	VkPhysicalDevice PhysicalDevicePicker::pick(VK::Instance &instance) noexcept
	{
		const std::vector<VkPhysicalDevice> devices{ __retrieveDevices(instance) };
		std::multimap<uint32_t, VkPhysicalDevice> scoreMap;

		for (const VkPhysicalDevice device : devices)
		{
			VkPhysicalDeviceProperties prop{};
			instance.vkGetPhysicalDeviceProperties(device, &prop);

			const std::vector<VkQueueFamilyProperties>
				queueFamilyProps{ __retrieveQueueFamilyProps(instance, device) };

			if (!__checkVersionSupport(prop))
				continue;

			if (!__checkQueueSupport(queueFamilyProps))
				continue;

			const uint32_t score{ __getScoreOf(prop) };
			scoreMap.emplace(score, device);
		}

		if (scoreMap.empty())
			return VK_NULL_HANDLE;

		return scoreMap.rbegin()->second;
	}

	std::vector<VkPhysicalDevice> PhysicalDevicePicker::__retrieveDevices(VK::Instance &instance) noexcept
	{
		uint32_t numDevices{};
		instance.vkEnumeratePhysicalDevices(&numDevices, nullptr);

		std::vector<VkPhysicalDevice> retVal;
		retVal.resize(numDevices);
		instance.vkEnumeratePhysicalDevices(&numDevices, retVal.data());

		return retVal;
	}

	std::vector<VkQueueFamilyProperties>
		PhysicalDevicePicker::__retrieveQueueFamilyProps(
		VK::Instance &instance, const VkPhysicalDevice physicalDevice) noexcept
	{
		uint32_t numProps{};
		instance.vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &numProps, nullptr);

		std::vector<VkQueueFamilyProperties> retVal;
		retVal.resize(numProps);
		instance.vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &numProps, retVal.data());

		return retVal;
	}

	bool PhysicalDevicePicker::__checkVersionSupport(const VkPhysicalDeviceProperties &prop) noexcept
	{
		const uint32_t deviceVersion{ prop.apiVersion };
		const uint32_t major{ VK_API_VERSION_MAJOR(deviceVersion) };
		const uint32_t minor{ VK_API_VERSION_MINOR(deviceVersion) };

		if (major > 1U)
			return true;

		return ((major == 1U) && (minor >= 3U));
	}

	bool PhysicalDevicePicker::__checkQueueSupport(
		const std::vector<VkQueueFamilyProperties> &queueFamilyProps) noexcept
	{
		for (const auto &prop : queueFamilyProps)
		{
			if (prop.queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT)
				return true;
		}

		return false;
	}

	uint32_t PhysicalDevicePicker::__getScoreOf(const VkPhysicalDeviceProperties &prop) noexcept
	{
		uint32_t retVal{};

		switch (prop.deviceType)
		{
		case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
			retVal += 10000U;
			break;

		case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
			retVal += 1000U;
			break;
		}

		return retVal;
	}
}
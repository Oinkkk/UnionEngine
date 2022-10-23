#include "PhysicalDevice.h"

namespace VK
{
	PhysicalDevice::PhysicalDevice(Instance &instance, const VkPhysicalDevice handle) noexcept :
		Handle{ handle }, __instance{ instance }
	{}
}
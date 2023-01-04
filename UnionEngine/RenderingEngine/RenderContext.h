#pragma once

#include "../Vulkan/PhysicalDevice.h"

namespace Engine
{
	class RenderContext
	{
	public:
		VK::VulkanInstance *pVulkanInstance{};
		VK::PhysicalDevice *pPhysicalDevice{};
	};
}
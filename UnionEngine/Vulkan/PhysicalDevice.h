#pragma once

#include "Instance.h"

namespace VK
{
	class PhysicalDevice : public Infra::Handle<VkPhysicalDevice>
	{
	public:
		PhysicalDevice(Instance &instance, const VkPhysicalDevice handle) noexcept;
		virtual ~PhysicalDevice() noexcept = default;

	private:
		Instance &__instance;
	};
}
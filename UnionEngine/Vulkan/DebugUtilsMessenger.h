#pragma once

#include "Instance.h"

namespace VK
{
	class DebugUtilsMessenger : public Infra::Handle<VkDebugUtilsMessengerEXT>, public Infra::Deletable
	{
	public:
		DebugUtilsMessenger(Instance &instance, const VkDebugUtilsMessengerCreateInfoEXT &createInfo);
		virtual ~DebugUtilsMessenger() noexcept;

	private:
		Instance &__instance;

		[[nodiscard]]
		static VkDebugUtilsMessengerEXT __create(
			Instance &instance, const VkDebugUtilsMessengerCreateInfoEXT &createInfo);
	};
}
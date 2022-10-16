#pragma once

#include "VulkanLoader.h"
#include "../Infra/Handle.h"
#include <string_view>

namespace VK
{
	class Instance : public Infra::Handle<VkInstance>
	{
	public:
		Instance(const VkInstanceCreateInfo &createInfo);
		virtual ~Instance() noexcept;

	private:
		InstanceProc __proc{};
		
		template <typename $Func>
		[[nodiscard]]
		$Func vkGetInstanceProcAddr(const std::string_view name) const noexcept;
		
		void __queryProc() noexcept;

		static VkInstance __create(const VkInstanceCreateInfo &createInfo);
	};

	template <typename $Func>
	$Func Instance::vkGetInstanceProcAddr(const std::string_view name) const noexcept
	{
		const GlobalProc &globalProc{ VulkanLoader::getInstance().getGlobalProc() };
		return reinterpret_cast<$Func>(globalProc.vkGetInstanceProcAddr(getHandle(), name.data()));
	}
}
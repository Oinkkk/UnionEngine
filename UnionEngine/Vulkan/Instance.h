#pragma once

#include "VulkanLoader.h"
#include "../Infra/Handle.h"
#include "../Infra/Deletable.h"
#include <string_view>

namespace VK
{
	class Instance : public Infra::Handle<VkInstance>, public Infra::Deletable
	{
	public:
		Instance(const VkInstanceCreateInfo &createInfo);
		virtual ~Instance() noexcept;

		VkResult vkCreateDebugUtilsMessengerEXT(
			const VkDebugUtilsMessengerCreateInfoEXT *const pCreateInfo,
			const VkAllocationCallbacks *const pAllocator,
			VkDebugUtilsMessengerEXT *const pMessenger) noexcept;

		void vkDestroyDebugUtilsMessengerEXT(
			const VkDebugUtilsMessengerEXT messenger,
			const VkAllocationCallbacks *const pAllocator) noexcept;

		VkResult vkEnumeratePhysicalDevices(
			uint32_t *const pPhysicalDeviceCount,
			VkPhysicalDevice *const pPhysicalDevices) const noexcept;

		void vkGetPhysicalDeviceProperties(
			const VkPhysicalDevice physicalDevice,
			VkPhysicalDeviceProperties *const pProperties) const noexcept;

		void vkGetPhysicalDeviceQueueFamilyProperties(
			const VkPhysicalDevice physicalDevice,
			uint32_t *const pQueueFamilyPropertyCount,
			VkQueueFamilyProperties *const pQueueFamilyProperties) const noexcept;

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
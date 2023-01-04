#pragma once

#include "../Infra/Logger.h"
#include "../Vulkan/DebugUtilsMessenger.h"
#include "../Vulkan/PhysicalDevice.h"
#include "RenderContext.h"
#include <string_view>
#include <vector>

namespace Engine
{
	class RenderingEngine : public Infra::Unique
	{
	public:
		RenderingEngine(const std::string_view appName, const std::string_view engineName);
		virtual ~RenderingEngine() noexcept;

	private:
		const std::string __appName;
		const std::string __engineName;

		RenderContext __renderContext;
		uint32_t __instanceVersion{};

		std::unique_ptr<VK::VulkanInstance> __pVulkanInstance;
		std::unique_ptr<VK::PhysicalDevice> __pPhysicalDevice;
		VkPhysicalDeviceProperties2 __physicalDeviceProp2{};
		VkPhysicalDeviceVulkan11Properties __physicalDevice11Prop{};
		VkPhysicalDeviceVulkan12Properties __physicalDevice12Prop{};
		VkPhysicalDeviceVulkan13Properties __physicalDevice13Prop{};

		uint32_t __queueFamilyIndex{};

#ifndef NDEBUG
		VkDebugUtilsMessengerCreateInfoEXT __debugMessengerCreateInfo{};
		std::unique_ptr<VK::DebugUtilsMessenger> __pDebugUtilsMessenger;

		void __populateDebugMessengerCreateInfo() noexcept;
		void __createDebugUtilsMessenger();
#endif

		void __checkInstanceVersion();
		void __createVulkanInstance();
		void __pickPhysicalDevice();
		void __queryPhysicalDeviceProps() noexcept;
		void __pickGraphicsQueueFamily() noexcept;

		[[nodiscard]]
		static constexpr Infra::LogSeverityType __convertVulkanSeverityType(
			const VkDebugUtilsMessageSeverityFlagBitsEXT severityType) noexcept;

		static VkBool32 vkDebugUtilsMessengerCallbackEXT(
			const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			const VkDebugUtilsMessageTypeFlagsEXT messageTypes,
			const VkDebugUtilsMessengerCallbackDataEXT *const pCallbackData,
			void *const pUserData) noexcept;
	};

	constexpr Infra::LogSeverityType RenderingEngine::__convertVulkanSeverityType(
		const VkDebugUtilsMessageSeverityFlagBitsEXT severityType) noexcept
	{
		Infra::LogSeverityType retVal{};
		switch (severityType)
		{
		case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			retVal = Infra::LogSeverityType::VERBOSE;
			break;

		case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			retVal = Infra::LogSeverityType::INFO;
			break;

		case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			retVal = Infra::LogSeverityType::WARNING;
			break;

		case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			retVal = Infra::LogSeverityType::FATAL;
			break;
		}

		return retVal;
	}
}
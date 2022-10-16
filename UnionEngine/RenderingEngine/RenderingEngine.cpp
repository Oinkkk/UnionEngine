#include "RenderingEngine.h"
#include "../Vulkan/VulkanLoader.h"
#include <format>
#include <stdexcept>
#include <vector>
#include "InstanceSupport.h"

namespace Engine
{
	RenderingEngine::RenderingEngine(const std::string_view appName, const std::string_view engineName) :
		__appName{ appName },
		__engineName{ engineName }
	{
		__checkInstanceVersion();

#ifndef NDEBUG
		__populateDebugMessengerCreateInfo();
#endif

		__createInstance();
	}

	RenderingEngine::~RenderingEngine() noexcept
	{
		__pInstance = nullptr;
	}

	void RenderingEngine::__checkInstanceVersion()
	{
		const VK::GlobalProc &globalProc
		{
			VK::VulkanLoader::getInstance().getGlobalProc()
		};

		if (!(globalProc.vkEnumerateInstanceVersion))
			__instanceVersion = VK_API_VERSION_1_0;
		else
			globalProc.vkEnumerateInstanceVersion(&__instanceVersion);

		const uint32_t major{ VK_API_VERSION_MAJOR(__instanceVersion) };
		const uint32_t minor{ VK_API_VERSION_MINOR(__instanceVersion) };
		const uint32_t patch{ VK_API_VERSION_PATCH(__instanceVersion) };
		const uint32_t variant{ VK_API_VERSION_VARIANT(__instanceVersion) };

		if (major > 1U)
			return;

		if ((major == 1U) && (minor >= 3U))
			return;

		const std::string errMsg
		{
			std::format(
				"RenderingEngine: Unsupported vulkan version detected. Expected: >= 1.3, Actual: {}.{}.{}.{}",
				major, minor, patch, variant)
		};

		throw std::runtime_error{ errMsg };
	}

	void RenderingEngine::__populateDebugMessengerCreateInfo() noexcept
	{
		__debugMessengerCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		__debugMessengerCreateInfo.messageSeverity =
		(
			VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT		|
			VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT		|
			VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
		);

		__debugMessengerCreateInfo.messageType =
		(
			VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT		|
			VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT	|
			VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
		);

		__debugMessengerCreateInfo.pfnUserCallback = vkDebugUtilsMessengerCallbackEXT;
	}

	void RenderingEngine::__createInstance()
	{
		const VK::GlobalProc &globalProc{ VK::VulkanLoader::getInstance().getGlobalProc() };
		InstanceSupport &instanceSupport{ InstanceSupport::getInstance() };

		std::vector<const char *> enabledLayers;
		std::vector<const char *> enabledExtensions;
		const void *pNext{};

#ifndef NDEBUG
		static constexpr std::string_view VK_LAYER_KHRONOS_VALIDATION_LAYER_NAME{ "VK_LAYER_KHRONOS_validation" };

		if (!(instanceSupport.isSupportedLayer(VK_LAYER_KHRONOS_VALIDATION_LAYER_NAME)))
			throw std::runtime_error{ "RenderingEngine: Validation layer not supported" };

		if (!(instanceSupport.isSupportedExtension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)))
			throw std::runtime_error{ "RenderingEngine: Debug util extension not supported" };

		enabledLayers.emplace_back(VK_LAYER_KHRONOS_VALIDATION_LAYER_NAME.data());
		enabledExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		pNext = &__debugMessengerCreateInfo;

		static constexpr VkValidationFeatureEnableEXT enabledValidationFeatures[]
		{
			VkValidationFeatureEnableEXT::VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT,
			VkValidationFeatureEnableEXT::VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT,
			VkValidationFeatureEnableEXT::VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT
		};

		static constexpr VkValidationFeatureDisableEXT disabledValidationFeatures[]
		{
			VkValidationFeatureDisableEXT::VK_VALIDATION_FEATURE_DISABLE_SHADER_VALIDATION_CACHE_EXT
		};

		const VkValidationFeaturesEXT validationFeatures
		{
			.sType = VkStructureType::VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT,
			.pNext = pNext,
			.enabledValidationFeatureCount = uint32_t(std::size(enabledValidationFeatures)),
			.pEnabledValidationFeatures = enabledValidationFeatures,
			.disabledValidationFeatureCount = uint32_t(std::size(disabledValidationFeatures)),
			.pDisabledValidationFeatures = disabledValidationFeatures
		};

		pNext = &validationFeatures;
#endif

		if (!(instanceSupport.isSupportedExtension(VK_KHR_SURFACE_EXTENSION_NAME)))
			throw std::runtime_error{ "RenderingEngine: WSI not supported" };

		if (!(instanceSupport.isSupportedExtension(VK_KHR_WIN32_SURFACE_EXTENSION_NAME)))
			throw std::runtime_error{ "RenderingEngine: WSI not supported" };

		enabledExtensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);
		enabledExtensions.emplace_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

		const VkApplicationInfo appInfo
		{
			.sType = VkStructureType::VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pApplicationName = __appName.c_str(),
			.applicationVersion = VK_API_VERSION_1_0,
			.pEngineName = __engineName.c_str(),
			.engineVersion = VK_API_VERSION_1_0,
			.apiVersion = __instanceVersion
		};

		const VkInstanceCreateInfo createInfo
		{
			.sType = VkStructureType::VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pNext = pNext,
			.pApplicationInfo = &appInfo,
			.enabledLayerCount = uint32_t(enabledLayers.size()),
			.ppEnabledLayerNames = enabledLayers.data(),
			.enabledExtensionCount = uint32_t(enabledExtensions.size()),
			.ppEnabledExtensionNames = enabledExtensions.data()
		};

		__pInstance = std::make_unique<VK::Instance>(createInfo);
	}

	VkBool32 RenderingEngine::vkDebugUtilsMessengerCallbackEXT(
		const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		const VkDebugUtilsMessageTypeFlagsEXT messageTypes,
		const VkDebugUtilsMessengerCallbackDataEXT *const pCallbackData,
		void *const pUserData) noexcept
	{
		Infra::Logger &logger{ Infra::Logger::getInstance() };
		const Infra::LogSeverityType severityType{ __convertVulkanSeverityType(messageSeverity) };

		logger.log(severityType, pCallbackData->pMessage);
		return VK_FALSE;
	}
}
#include "RenderingEngine.h"
#include "../Vulkan/VulkanLoader.h"
#include <format>
#include <stdexcept>
#include <vector>
#include "InstanceSupport.h"
#include "PhysicalDevicePicker.h"

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

		__createVulkanInstance();

#ifndef NDEBUG
		__createDebugUtilsMessenger();
#endif

		__pickPhysicalDevice();
		__queryPhysicalDeviceProps();
		__pickGraphicsQueueFamily();
	}

	RenderingEngine::~RenderingEngine() noexcept
	{
#ifndef NDEBUG
		__pDebugUtilsMessenger = nullptr;
#endif

		__pVulkanInstance = nullptr;
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

	void RenderingEngine::__createVulkanInstance()
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
			// 퍼포먼스 떨어지는 코드 경고
			VkValidationFeatureEnableEXT::VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT,

			// 셰이더에서 debugPrintfEXT() 호출할 수 있게 해줌
			VkValidationFeatureEnableEXT::VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT,

			// 메모리 해저드 발생 경고
			VkValidationFeatureEnableEXT::VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT
		};

		static constexpr VkValidationFeatureDisableEXT disabledValidationFeatures[]
		{
			// 셰이더 검증에 대한 캐시 생성
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

		__pVulkanInstance = std::make_unique<VK::VulkanInstance>(createInfo);
		__renderContext.pVulkanInstance = __pVulkanInstance.get();
	}

	void RenderingEngine::__pickPhysicalDevice()
	{
		const VkPhysicalDevice handle{ PhysicalDevicePicker::pick(*__pVulkanInstance) };
		if (!handle)
			throw std::runtime_error{ "No suitable physical device" };

		__pPhysicalDevice = std::make_unique<VK::PhysicalDevice>(*__pVulkanInstance, handle);
		__renderContext.pPhysicalDevice = __pPhysicalDevice.get();
	}

	void RenderingEngine::__queryPhysicalDeviceProps() noexcept
	{
		__physicalDeviceProp2.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
		__physicalDeviceProp2.pNext = &__physicalDevice11Prop;

		__physicalDevice11Prop.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES;
		__physicalDevice11Prop.pNext = &__physicalDevice12Prop;

		__physicalDevice12Prop.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES;
		__physicalDevice12Prop.pNext = &__physicalDevice13Prop;

		__physicalDevice13Prop.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES;
		__physicalDevice13Prop.pNext = nullptr;

		__pPhysicalDevice->vkGetProperties2(&__physicalDeviceProp2);
	}

	void RenderingEngine::__pickGraphicsQueueFamily() noexcept
	{
		uint32_t numProps{};
		__pPhysicalDevice->vkGetQueueFamilyProperties(&numProps, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilyProps;
		queueFamilyProps.resize(numProps);
		__pPhysicalDevice->vkGetQueueFamilyProperties(&numProps, queueFamilyProps.data());

		bool found{};
		for (uint32_t propIter = 0U; propIter < numProps; propIter++)
		{
			const VkQueueFamilyProperties &queueFamilyProp{ queueFamilyProps[propIter] };

			if (!(queueFamilyProp.queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT))
				continue;

			const VkBool32 win32SupportResult
			{
				__pPhysicalDevice->vkGetWin32PresentationSupportKHR(propIter)
			};

			if (!win32SupportResult)
				continue;

			__queueFamilyIndex = propIter;
			found = true;
			break;
		}

		if (!found)
			throw std::runtime_error{ "Cannot find any suitable graphics queue" };
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

	void RenderingEngine::__createDebugUtilsMessenger()
	{
		__pDebugUtilsMessenger = std::make_unique<VK::DebugUtilsMessenger>(*__pVulkanInstance, __debugMessengerCreateInfo);
	}
}
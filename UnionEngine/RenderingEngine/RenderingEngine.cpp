#include "RenderingEngine.h"
#include "../Vulkan/VulkanLoader.h"
#include <format>
#include <stdexcept>

namespace Engine
{
	RenderingEngine::RenderingEngine(const std::string_view appName, const std::string_view engineName) :
		__appName{ appName },
		__engineName{ engineName }
	{
		__getInstanceVersion();
		__checkInstanceVersionSupport();
	}

	RenderingEngine::~RenderingEngine() noexcept
	{

	}

	void RenderingEngine::__getInstanceVersion() noexcept
	{
		const VK::GlobalProcedure &globalProc
		{
			VK::VulkanLoader::getInstance().getGlobalProcedure()
		};

		if (!(globalProc.vkEnumerateInstanceVersion))
		{
			__instanceVersion = VK_API_VERSION_1_0;
			return;
		}

		globalProc.vkEnumerateInstanceVersion(&__instanceVersion);
	}

	void RenderingEngine::__checkInstanceVersionSupport() const
	{
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
				"The instance version is too low. Expected: >= 1.3, Actual: {}.{}.{}.{}",
				major, minor, patch, variant)
		};

		throw std::runtime_error{ errMsg };
	}
}
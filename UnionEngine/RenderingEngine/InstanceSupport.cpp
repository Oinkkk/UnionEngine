#include "InstanceSupport.h"

namespace Engine
{
	InstanceSupport::InstanceSupport() noexcept
	{
		__queryLayers();
		__queryExtensions();
		__populateMaps();
	}

	bool InstanceSupport::isSupportedLayer(const std::string_view name) const noexcept
	{
		return __layerMap.contains(name);
	}

	bool InstanceSupport::isSupportedExtension(const std::string_view name) const noexcept
	{
		return __extensionMap.contains(name);
	}

	InstanceSupport &InstanceSupport::getInstance() noexcept
	{
		static InstanceSupport instance;
		return instance;
	}

	void InstanceSupport::__queryLayers() noexcept
	{
		const VK::GlobalProc &globalProc{ VK::VulkanLoader::getInstance().getGlobalProc() };

		uint32_t numLayers{};
		globalProc.vkEnumerateInstanceLayerProperties(&numLayers, nullptr);

		__layers.resize(numLayers);
		globalProc.vkEnumerateInstanceLayerProperties(&numLayers, __layers.data());
	}

	void InstanceSupport::__queryExtensions() noexcept
	{
		const VK::GlobalProc &globalProc{ VK::VulkanLoader::getInstance().getGlobalProc() };

		uint32_t numExtensions{};
		globalProc.vkEnumerateInstanceExtensionProperties(nullptr, &numExtensions, nullptr);

		__extensions.resize(numExtensions);
		globalProc.vkEnumerateInstanceExtensionProperties(nullptr, &numExtensions, __extensions.data());
	}

	void InstanceSupport::__populateMaps() noexcept
	{
		for (const auto &layer : __layers)
			__layerMap[layer.layerName] = &layer;

		for (const auto &extension : __extensions)
			__extensionMap[extension.extensionName] = &extension;
	}
}
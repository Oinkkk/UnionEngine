#pragma once
#include "../Infra/Unique.h"
#include <string_view>
#include <vector>
#include "../Vulkan/VulkanLoader.h"
#include <unordered_map>

namespace Engine
{
	class InstanceSupport : public Infra::Unique
	{
	public:
		virtual ~InstanceSupport() noexcept = default;

		[[nodiscard]]
		bool isSupportedLayer(const std::string_view name) const noexcept;

		[[nodiscard]]
		bool isSupportedExtension(const std::string_view name) const noexcept;

		[[nodiscard]]
		static InstanceSupport &getInstance() noexcept;

	private:
		std::vector<VkLayerProperties> __layers;
		std::vector<VkExtensionProperties> __extensions;

		std::unordered_map<std::string_view, const VkLayerProperties *> __layerMap;
		std::unordered_map<std::string_view, const VkExtensionProperties *> __extensionMap;

		InstanceSupport() noexcept;

		void __queryLayers() noexcept;
		void __queryExtensions() noexcept;
		void __populateMaps() noexcept;
	};
}
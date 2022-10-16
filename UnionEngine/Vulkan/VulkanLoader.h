#pragma once

#include "../Infra/Unique.h"
#include "VulkanProc.h"

namespace VK
{
	class VulkanLoader : public Infra::Unique
	{
	public:
		[[nodiscard]]
		static VulkanLoader &getInstance() noexcept;

		[[nodiscard]]
		constexpr const GlobalProc &getGlobalProc() const noexcept;

	private:
		HMODULE __handle{};
		GlobalProc __globalProc;

		VulkanLoader();
		virtual ~VulkanLoader() noexcept;

		void __loadGlobalProc() noexcept;
	};

	constexpr const GlobalProc &VulkanLoader::getGlobalProc() const noexcept
	{
		return __globalProc;
	}
}
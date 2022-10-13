#pragma once

#include "../Infra/Unique.h"
#include "VulkanProcedure.h"
#include <Windows.h>

namespace VK
{
	class VulkanLoader : public Infra::Unique
	{
	public:
		[[nodiscard]]
		static VulkanLoader &getInstance() noexcept;

	private:
		HMODULE __handle{};
		GlobalProcedure __globalProc;

		VulkanLoader();
		virtual ~VulkanLoader() noexcept;

		void __loadGlobalProc() noexcept;
	};
}
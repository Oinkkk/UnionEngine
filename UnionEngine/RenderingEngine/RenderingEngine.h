#pragma once

#include "../Infra/Unique.h"
#include <string_view>

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

		uint32_t __instanceVersion{};

		void __getInstanceVersion() noexcept;
		void __checkInstanceVersionSupport() const;
	};
}
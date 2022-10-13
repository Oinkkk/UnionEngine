#include "Logger.h"
#include <iostream>
#include <format>

namespace Infra
{
	void Logger::log(const LogSeverityType severityType, const std::string_view message) noexcept
	{
		const int severityIndex{ static_cast<int>(severityType) };
		const auto &color{ __severityTypeColors[severityIndex] };
		const auto &severity{ __severityTypeStrings[severityIndex] };

		const std::string &outString
		{
			std::format("{}[{}]{}{}", color, severity, message, __resetColor)
		};

		std::lock_guard lck{ __mutex };
		std::cerr << outString << std::endl;
	}

	Logger &Logger::getInstance() noexcept
	{
		static Logger instance;
		return instance;
	}
}
#include "../Infra/Logger.h"

int main()
{
	Infra::Logger &logger{ Infra::Logger::getInstance() };
	logger.log(Infra::LogSeverityType::VERBOSE, "VERBOSE");
	logger.log(Infra::LogSeverityType::INFO, "INFO");
	logger.log(Infra::LogSeverityType::WARNING, "WARNING");
	logger.log(Infra::LogSeverityType::FATAL, "FATAL");

	return 0;
}
#include "../Vulkan/VulkanLoader.h"
#include "../RenderingEngine/RenderingEngine.h"

int main() try
{
	Infra::Logger &logger{ Infra::Logger::getInstance() };

	VK::VulkanLoader &vulkanLoader{ VK::VulkanLoader::getInstance() };
	Engine::RenderingEngine renderingEngine{ "UnionEngineSample", "UnionEngine" };

	logger.log(Infra::LogSeverityType::INFO, "The application is finished successfully");

	return 0;
}
catch (const std::exception &e)
{
	Infra::Logger &logger{ Infra::Logger::getInstance() };

	logger.log(Infra::LogSeverityType::FATAL, e.what());
	logger.log(Infra::LogSeverityType::FATAL, "The application is crashed");
}
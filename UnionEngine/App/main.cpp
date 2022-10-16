#include "../Vulkan/VulkanLoader.h"
#include "../RenderingEngine/RenderingEngine.h"

int main() try
{
	VK::VulkanLoader &vulkanLoader{ VK::VulkanLoader::getInstance() };
	Engine::RenderingEngine renderingEngine{ "UnionEngineSample", "UnionEngine" };

	return 0;
}
catch (const std::exception &e)
{
	Infra::Logger::getInstance().log(Infra::LogSeverityType::FATAL, e.what());
}
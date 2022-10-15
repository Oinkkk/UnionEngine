#include "../Infra/Logger.h"
#include "../Vulkan/VulkanLoader.h"
#include "../RenderingEngine/RenderingEngine.h"

int main()
{
	VK::VulkanLoader &vulkanLoader{ VK::VulkanLoader::getInstance() };
	Engine::RenderingEngine renderingEngine{ "UnionEngineSample", "UnionEngine" };

	return 0;
}
#include "../Infra/Logger.h"
#include "../Vulkan/VulkanLoader.h"

int main()
{
	VK::VulkanLoader &vulkanLoader{ VK::VulkanLoader::getInstance() };

	return 0;
}
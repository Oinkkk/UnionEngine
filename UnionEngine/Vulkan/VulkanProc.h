#pragma once

#include "Vulkan.h"

namespace VK
{
	class GlobalProc
	{
	public:
		PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr{};
		PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion{};
		PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties{};
		PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties{};
		PFN_vkCreateInstance vkCreateInstance{};
	};

	class InstanceProc
	{
	public:
		PFN_vkDestroyInstance vkDestroyInstance{};
		PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT{};
		PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT{};
	};

	class DeviceProc
	{
	public:
		
	};
}
//
//  Created by Sausty on 12. May. 2021.
//

#include "Engine/Renderer/Apis/Vulkan/Core/VulkanInstance.h"
#include "Engine/Renderer/Apis/Vulkan/Core/DeletionQueue.h"
#include "Engine/Renderer/Apis/Vulkan/Core/VulkanUtils.h"

namespace gp1 {
	namespace vkcore {

		VulkanInstance::VulkanInstance()
		{
			// Initialise Volk
			VkCheckError(volkInitialize());

#ifdef _DEBUG
			bool useValidationLayers = 1;
#else
			bool useValidationLayers = 0;
#endif
			// Validation layers

			uint32_t instanceLayerCount = 0;
			uint32_t layerFound = 0;
			char* instanceValidationLayers[] = {
				"VK_LAYER_KHRONOS_validation"
			};

			VkCheckError(vkEnumerateInstanceLayerProperties(&instanceLayerCount, NULL));

			if (instanceLayerCount > 0) {
				std::vector<VkLayerProperties> props;
				props.resize(instanceLayerCount);

				VkCheckError(vkEnumerateInstanceLayerProperties(&instanceLayerCount, props.data()));

				for (auto &layer : props) {
					if (layer.layerName == instanceValidationLayers[0]) {
						layerFound = 1;

						InstanceLayers.push_back("VK_LAYER_KHRONOS_validation");
					}
				}
			}

			// Extensions
			uint32_t instanceExtensionCount = 0;
			VkCheckError(vkEnumerateInstanceExtensionProperties(NULL, &instanceExtensionCount, NULL));

			if (instanceExtensionCount > 0) {
				std::vector<VkExtensionProperties> props;
				props.resize(instanceExtensionCount);

				VkCheckError(vkEnumerateInstanceExtensionProperties(NULL, &instanceExtensionCount, props.data()));

				for (auto &extension : props) {
					if (!strcmp(extension.extensionName, VK_KHR_SURFACE_EXTENSION_NAME))
						InstanceExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
					if (!strcmp(extension.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME))
						InstanceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

					// Platform specific surfaces
					if (!strcmp(extension.extensionName, "VK_KHR_win32_surface"))
						InstanceExtensions.push_back("VK_KHR_win32_surface");

					if (!strcmp(extension.extensionName, "VK_KHR_wayland_surface"))
						InstanceExtensions.push_back("VK_KHR_wayland_surface");

					if (!strcmp(extension.extensionName, "VK_KHR_xcb_surface"))
						InstanceExtensions.push_back("VK_KHR_xcb_surface");

					if (!strcmp(extension.extensionName, "VK_KHR_xlib_surface"))
						InstanceExtensions.push_back("VK_KHR_xlib_surface");
				}
			}

			VkApplicationInfo appInfo{};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = "Game-Project 1";
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = "Game-Project 1 Engine";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_2;

			VkInstanceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;

			if (useValidationLayers)
			{
				createInfo.enabledLayerCount = static_cast<uint32_t>(InstanceLayers.size());
				createInfo.ppEnabledLayerNames = InstanceLayers.data();
			}

			createInfo.enabledExtensionCount = static_cast<uint32_t>(InstanceExtensions.size());
			createInfo.ppEnabledExtensionNames = InstanceExtensions.data();

			VkCheckError(vkCreateInstance(&createInfo, NULL, &m_Instance));

			volkLoadInstance(m_Instance);

			VulkanInstance instance = *this;
			DeletionQueue::PushFunction([instance]() mutable
			{
				vkDestroyInstance(instance.GetInstance(), nullptr);
			});
		}

	}
}

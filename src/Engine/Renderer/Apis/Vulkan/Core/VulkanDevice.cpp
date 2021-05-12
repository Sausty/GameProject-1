//
//	Created by Sausty on 12. May. 2021
//

#include "Engine/Renderer/Apis/Vulkan/Core/VulkanDevice.h"

#include "Engine/Renderer/Apis/Vulkan/VulkanRenderer.h"
#include "Engine/Renderer/Apis/Vulkan/Core/VulkanGPU.h"
#include "Engine/Renderer/Apis/Vulkan/Core/VulkanUtils.h"
#include "Engine/Renderer/Apis/Vulkan/Core/DeletionQueue.h"

#include <set>

namespace gp1 {
	namespace vkcore {

		VulkanDevice::VulkanDevice()
		{
			VkInstance& instance = VulkanRenderer::GetVulkanRendererData().Instance->GetInstance();
			VkPhysicalDevice& physicalDevice = VulkanRenderer::GetVulkanRendererData().GPU->GetVulkanGPU();
			VulkanGPU::QueueFamilies indices = VulkanGPU::QueryQueueIndices(physicalDevice);

			std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
			std::set<uint32_t> uniqueQueueFamilies = { indices.GraphicsQueueIndex.value(), indices.PresentQueueIndex.value() };

			float queuePriority = 1.0f;
			for (uint32_t queueFamily : uniqueQueueFamilies) {
				VkDeviceQueueCreateInfo queueCreateInfo{};
				queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueCreateInfo.queueFamilyIndex = queueFamily;
				queueCreateInfo.queueCount = 1;
				queueCreateInfo.pQueuePriorities = &queuePriority;
				queueCreateInfos.push_back(queueCreateInfo);
			}

			VkPhysicalDeviceFeatures deviceFeatures = VulkanRenderer::GetVulkanRendererData().GPU->GetGPUFeatures();

			std::vector<const char*> validationLayers = VulkanRenderer::GetVulkanRendererData().Instance->GetInstanceLayers();

			char* extensions[] = {
				VK_KHR_SWAPCHAIN_EXTENSION_NAME
			};

			VkDeviceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
			createInfo.pQueueCreateInfos = queueCreateInfos.data();
			createInfo.pEnabledFeatures = &deviceFeatures;
			createInfo.enabledExtensionCount = 1;
			createInfo.ppEnabledExtensionNames = extensions;

			#ifdef _DEBUG
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
			#endif

			VkCheckError(vkCreateDevice(physicalDevice, &createInfo, NULL, &m_Device));

			volkLoadDevice(m_Device);

			vkGetDeviceQueue(m_Device, indices.GraphicsQueueIndex.value(), 0, &m_GraphicsQueue);
			vkGetDeviceQueue(m_Device, indices.PresentQueueIndex.value(), 0, &m_PresentQueue);

			VulkanDevice inst = *this;
			DeletionQueue::PushFunction([inst, &instance]() mutable {
				vkDestroyDevice(inst.GetVulkanDevice(), NULL);
			});
		}
	}
}

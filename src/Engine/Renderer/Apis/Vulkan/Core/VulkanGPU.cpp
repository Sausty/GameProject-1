//
//	Created by Sausty on 12. May. 2021
//

#include "Engine/Renderer/Apis/Vulkan/Core/VulkanGPU.h"
#include "Engine/Renderer/Apis/Vulkan/Core/DeletionQueue.h"
#include "Engine/Renderer/Apis/Vulkan/Core/VulkanUtils.h"
#include "Engine/Renderer/Apis/Vulkan/VulkanRenderer.h"

namespace gp1 {
	namespace vkcore {

		static bool IsDeviceSuitable(VkPhysicalDevice device)
		{
			VulkanGPU::QueueFamilies indices = VulkanGPU::QueryQueueIndices(device);

			return indices.IsComplete();
		}

		VulkanGPU::VulkanGPU()
		{
			VkInstance& instance = VulkanRenderer::GetVulkanRendererData()->Instance.GetInstance();

			uint32_t deviceCount = 0;
			vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

			if (deviceCount == 0) {
				VulkanRenderer::VulkanOutputLogger.LogError("Failed to find a suitable physical device!");
			}

			std::vector<VkPhysicalDevice> devices(deviceCount);
			vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

			for (const auto& device : devices) {
				if (IsDeviceSuitable(device)) {
					m_Device = device;
					break;
				}
			}
		}

		VulkanGPU::QueueFamilies VulkanGPU::QueryQueueIndices(VkPhysicalDevice gpu)
		{
			VulkanGPU::QueueFamilies indices;

			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueFamilyCount, queueFamilies.data());

			int i = 0;
			for (const auto& queueFamily : queueFamilies) {
				if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					indices.GraphicsQueueIndex = i;
				}

				VkBool32 presentSupport = false;
				//vkGetPhysicalDeviceSurfaceSupportKHR(gpu, i, surface, &presentSupport);

				if (presentSupport) {
					indices.PresentQueueIndex = i;
				}

				if (indices.IsComplete()) {
					break;
				}

				i++;
			}

			return indices;
		}

	}
}

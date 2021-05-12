//
//	Created by Sausty on 12. May. 2021
//

#include "Engine/Renderer/Apis/Vulkan/Core/VulkanGPU.h"
#include "Engine/Renderer/Apis/Vulkan/Core/DeletionQueue.h"
#include "Engine/Renderer/Apis/Vulkan/Core/VulkanUtils.h"
#include "Engine/Renderer/Apis/Vulkan/VulkanRenderer.h"
#include <set>

namespace gp1 {
	namespace vkcore {

		static bool CheckDeviceExtensionSupport(VkPhysicalDevice device)
		{
			std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

			uint32_t extensionCount;
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

			std::vector<VkExtensionProperties> availableExtensions(extensionCount);
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

			std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

			for (const auto& extension : availableExtensions) {
				requiredExtensions.erase(extension.extensionName);
			}

			return requiredExtensions.empty();
		}

		static bool IsDeviceSuitable(VkPhysicalDevice device)
		{
			VulkanGPU::QueueFamilies indices = VulkanGPU::QueryQueueIndices(device);

			bool extensionsSupported = CheckDeviceExtensionSupport(device);

			bool swapChainAdequate = false;
			if (extensionsSupported) {
				SwapChainSupportDetails swapChainSupport = VulkanSwapChain::QuerySwapChainSupport(device);
				swapChainAdequate = !swapChainSupport.Formats.empty() && !swapChainSupport.PresentModes.empty();
			}

			return indices.IsComplete() && extensionsSupported && swapChainAdequate;
		}

		VulkanGPU::VulkanGPU()
		{
			VkInstance& instance = VulkanRenderer::GetVulkanRendererData().Instance->GetInstance();

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

			m_DeviceQueues = QueryQueueIndices(m_Device);

			vkGetPhysicalDeviceProperties(m_Device, &m_DeviceProperties);
			vkGetPhysicalDeviceFeatures(m_Device, &m_DeviceFeatures);
		}

		VulkanGPU::QueueFamilies VulkanGPU::QueryQueueIndices(VkPhysicalDevice gpu)
		{
			VulkanGPU::QueueFamilies indices;
			VkSurfaceKHR surface = VulkanRenderer::GetVulkanRendererData().Surface->GetVulkanSurface();

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
				vkGetPhysicalDeviceSurfaceSupportKHR(gpu, i, surface, &presentSupport);

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

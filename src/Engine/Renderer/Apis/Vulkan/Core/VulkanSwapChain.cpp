//
//	Created by Sausty on 12. May. 2021
//

#include "Engine/Renderer/Apis/Vulkan/Core/VulkanSwapChain.h"

#include "Engine/Renderer/Apis/Vulkan/Core/DeletionQueue.h"
#include "Engine/Renderer/Apis/Vulkan/Core/VulkanGPU.h"
#include "Engine/Renderer/Apis/Vulkan/Core/VulkanUtils.h"
#include "Engine/Renderer/Apis/Vulkan/VulkanRenderer.h"

namespace gp1 {
	namespace vkcore {

		VulkanSwapChain::VulkanSwapChain(uint32_t frames, uint32_t width, uint32_t height)
			: m_FramesInFlight(frames)
		{
			VkSurfaceKHR& surface = VulkanRenderer::GetVulkanRendererData().Surface->GetVulkanSurface();
			VkDevice& device = VulkanRenderer::GetVulkanRendererData().Device->GetVulkanDevice();
			VkPhysicalDevice& gpu = VulkanRenderer::GetVulkanRendererData().GPU->GetVulkanGPU();

			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(gpu);

			VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.Formats);
			VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.PresentModes);
			VkExtent2D extent = ChooseSwapExtent(swapChainSupport.Capabilities, width, height);

			uint32_t imageCount = swapChainSupport.Capabilities.minImageCount + 1;
			if (swapChainSupport.Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.Capabilities.maxImageCount) {
				imageCount = swapChainSupport.Capabilities.maxImageCount;
			}

			VkSwapchainCreateInfoKHR createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			createInfo.surface = surface;
			createInfo.minImageCount = imageCount;
			createInfo.imageFormat = surfaceFormat.format;
			createInfo.imageColorSpace = surfaceFormat.colorSpace;
			createInfo.imageExtent = extent;
			createInfo.imageArrayLayers = 1;
			createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			VulkanGPU::QueueFamilies indices = VulkanGPU::QueryQueueIndices(gpu);
			uint32_t queueFamilyIndices[] = { indices.GraphicsQueueIndex.value(), indices.PresentQueueIndex.value() };

			if (indices.GraphicsQueueIndex != indices.PresentQueueIndex) {
				createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
				createInfo.queueFamilyIndexCount = 2;
				createInfo.pQueueFamilyIndices = queueFamilyIndices;
			}
			else {
				createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			}

			createInfo.preTransform = swapChainSupport.Capabilities.currentTransform;
			createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			createInfo.presentMode = presentMode;
			createInfo.clipped = VK_TRUE;

			VkCheckError(vkCreateSwapchainKHR(device, &createInfo, nullptr, &m_SwapChain));

			vkGetSwapchainImagesKHR(device, m_SwapChain, &imageCount, nullptr);
			m_BackBuffer.Images.resize(imageCount);
			m_BackBuffer.ImageViews.resize(imageCount);
			vkGetSwapchainImagesKHR(device, m_SwapChain, &imageCount, m_BackBuffer.Images.data());

			m_ImageFormat = surfaceFormat.format;
			m_Extent = extent;
			m_FramesInFlight = frames;

			for (uint32_t i = 0; i < m_BackBuffer.Images.size(); i++) {
				VkImageViewCreateInfo ivCreateInfo{};
				ivCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				ivCreateInfo.image = m_BackBuffer.Images[i];
				ivCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				ivCreateInfo.format = m_ImageFormat;
				ivCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
				ivCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
				ivCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
				ivCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
				ivCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				ivCreateInfo.subresourceRange.baseMipLevel = 0;
				ivCreateInfo.subresourceRange.levelCount = 1;
				ivCreateInfo.subresourceRange.baseArrayLayer = 0;
				ivCreateInfo.subresourceRange.layerCount = 1;

				VkCheckError(vkCreateImageView(device, &ivCreateInfo, nullptr, &m_BackBuffer.ImageViews[i]));
			}

			VulkanSwapChain inst = *this;

			DeletionQueue::PushFunction([inst, &device]() mutable {
				BackBufferData& data = inst.GetBackBuffers();

				for (uint32_t i = 0; i < data.Images.size(); i++) {
					vkDestroyImageView(device, data.ImageViews[i], nullptr);
				}

				vkDestroySwapchainKHR(device, inst.GetVulkanSwapChain(), nullptr);
			});
		}

		SwapChainSupportDetails VulkanSwapChain::QuerySwapChainSupport(VkPhysicalDevice device)
		{
			SwapChainSupportDetails details;

			VkSurfaceKHR& surface = VulkanRenderer::GetVulkanRendererData().Surface->GetVulkanSurface();

			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.Capabilities);

			uint32_t formatCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

			if (formatCount != 0) {
				details.Formats.resize(formatCount);
				vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.Formats.data());
			}

			uint32_t presentModeCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

			if (presentModeCount != 0) {
				details.PresentModes.resize(presentModeCount);
				vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.PresentModes.data());
			}

			return details;
		}

		VkSurfaceFormatKHR VulkanSwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats)
		{
			for (const auto& availableFormat : formats) {
				if (availableFormat.format == VK_FORMAT_R8G8B8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
					return availableFormat;
				}
			}

			return formats[0];
		}

		VkPresentModeKHR VulkanSwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& modes)
		{
			for (const auto& availablePresentMode : modes) {
				if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
					return availablePresentMode;
				}
			}

			return VK_PRESENT_MODE_FIFO_KHR;
		}

		VkExtent2D VulkanSwapChain::ChooseSwapExtent(VkSurfaceCapabilitiesKHR capabilities, uint32_t width, uint32_t height)
		{
			if (capabilities.currentExtent.width != UINT32_MAX) {
				return capabilities.currentExtent;
			}
			else {
				VkExtent2D actualExtent = {
					static_cast<uint32_t>(width),
					static_cast<uint32_t>(height)
				};

				actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
				actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

				return actualExtent;
			}
		}
	}
}

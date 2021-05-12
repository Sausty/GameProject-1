//
//	Created by Sausty on 12. May. 2021
//

#pragma once

#include <vector>
#include <volk.h>

namespace gp1 {
	namespace vkcore {

		struct SwapChainSupportDetails
		{
			VkSurfaceCapabilitiesKHR Capabilities;
			std::vector<VkSurfaceFormatKHR> Formats;
			std::vector<VkPresentModeKHR> PresentModes;
		};

		struct BackBufferData
		{
			std::vector<VkImage> Images;
			std::vector<VkImageView> ImageViews;
			// TODO: Framebuffers
		};

		class VulkanSwapChain
		{
		public:
			VulkanSwapChain(uint32_t frames, uint32_t width, uint32_t height);

			static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
			static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
			static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& modes);
			static VkExtent2D ChooseSwapExtent(VkSurfaceCapabilitiesKHR capabilities, uint32_t width, uint32_t height);

			VkSwapchainKHR& GetVulkanSwapChain() { return m_SwapChain; }
			VkExtent2D GetSwapChainExtent() { return m_Extent; }
			VkFormat GetSwapChainImageFormat() { return m_ImageFormat; }
			BackBufferData& GetBackBuffers() { return m_BackBuffer; }
			uint32_t GetFramesInFlight() { return m_FramesInFlight; }
		private:
			VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;
			uint32_t m_FramesInFlight;
			VkExtent2D m_Extent;
			VkFormat m_ImageFormat;
			BackBufferData m_BackBuffer;
		};
	}
}

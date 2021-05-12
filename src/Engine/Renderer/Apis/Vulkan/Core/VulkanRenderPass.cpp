//
//	Created by Sausty on 12. May. 2020
//

#include "Engine/Renderer/Apis/Vulkan/Core/VulkanRenderPass.h"
#include "Engine/Renderer/Apis/Vulkan/Core/DeletionQueue.h"
#include "Engine/Renderer/Apis/Vulkan/Core/VulkanUtils.h"
#include "Engine/Renderer/Apis/Vulkan/VulkanRenderer.h"

namespace gp1 {
	namespace vkcore {

		VulkanRenderPass::VulkanRenderPass(const std::string& name)
			: m_Name(name)
		{
			VkFormat format = VulkanRenderer::GetVulkanRendererData().SwapChain->GetSwapChainImageFormat();
			VkDevice& device = VulkanRenderer::GetVulkanRendererData().Device->GetVulkanDevice();

			VkAttachmentDescription colorAttachment{};
			colorAttachment.format = format;
			colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
			colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			VkAttachmentReference colorAttachmentRef{};
			colorAttachmentRef.attachment = 0;
			colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			VkSubpassDescription subpass{};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &colorAttachmentRef;

			VkSubpassDependency dependency{};
			dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			dependency.dstSubpass = 0;
			dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.srcAccessMask = 0;
			dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			VkRenderPassCreateInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			renderPassInfo.attachmentCount = 1;
			renderPassInfo.pAttachments = &colorAttachment;
			renderPassInfo.subpassCount = 1;
			renderPassInfo.pSubpasses = &subpass;
			renderPassInfo.dependencyCount = 1;
			renderPassInfo.pDependencies = &dependency;

			VkCheckError(vkCreateRenderPass(device, &renderPassInfo, nullptr, &m_RenderPass));

			VulkanRenderPass pass = *this;
			DeletionQueue::PushFunction([pass, &device]() mutable {
				vkDestroyRenderPass(device, pass.GetRenderPass(), nullptr);
			});
		}
	}
}

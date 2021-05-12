//
//	Created by Sausty on 12. May. 2020
//

#pragma once

#include <volk.h>
#include <string>

namespace gp1 {
	namespace vkcore {

		class VulkanRenderPass
		{
		public:
			VulkanRenderPass(const std::string& name);

			// TODO: Attach to framebuffer
			// TODO: Begin
			// TODO: End

			VkRenderPass& GetRenderPass() { return m_RenderPass; }
			const std::string& GetName() const { return m_Name; }
		private:
			VkRenderPass m_RenderPass;
			std::string m_Name;
			// TODO: Subpasses for deferred rendering
		};

	}
}

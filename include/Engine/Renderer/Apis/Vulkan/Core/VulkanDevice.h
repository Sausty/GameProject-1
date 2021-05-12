//
//	Created by Sausty on 12. May. 2021
//

#pragma once

#include <volk.h>

namespace gp1 {
	namespace vkcore {

		class VulkanDevice
		{
		public:
			VulkanDevice();

			VkDevice& GetVulkanDevice()		   { return m_Device;        }
			VkQueue&  GetVulkanGraphicsQueue() { return m_GraphicsQueue; }
			VkQueue&  GetVulkanPresentQueue()  { return m_PresentQueue;  }
		private:
			VkDevice m_Device;
			VkQueue  m_GraphicsQueue;
			VkQueue  m_PresentQueue;
		};

	}
}

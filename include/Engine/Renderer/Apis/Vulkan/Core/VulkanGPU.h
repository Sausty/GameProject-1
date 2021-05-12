//
//  Created by Sausty on 12. May. 2021
//

#pragma once

#include <volk.h>
#include <optional>

namespace gp1 {
	namespace vkcore {

		class VulkanGPU
		{
		public:
			struct QueueFamilies
			{
				std::optional<uint32_t> GraphicsQueueIndex;
				std::optional<uint32_t> PresentQueueIndex;

				bool IsComplete() {
					return GraphicsQueueIndex.has_value() && PresentQueueIndex.has_value();
				}
			};

			VulkanGPU();

			static QueueFamilies       QueryQueueIndices(VkPhysicalDevice gpu);

			QueueFamilies			   GetDeviceQueues()  { return m_DeviceQueues;     }
			VkPhysicalDevice&          GetVulkanGPU()     { return m_Device;	       }
			VkPhysicalDeviceFeatures   GetGPUFeatures()   { return m_DeviceFeatures;   }
			VkPhysicalDeviceProperties GetGPUProperties() { return m_DeviceProperties; }
		private:
			VkPhysicalDevice m_Device = VK_NULL_HANDLE;
			QueueFamilies    m_DeviceQueues;

			VkPhysicalDeviceFeatures   m_DeviceFeatures;
			VkPhysicalDeviceProperties m_DeviceProperties;
		};

	}
}

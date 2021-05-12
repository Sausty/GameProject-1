//
//  Created by Sausty on 12. May. 2021.
//

#pragma once

#include <volk.h>
#include <vector>

namespace gp1 {
	namespace vkcore {	

		class VulkanInstance
		{
		public:
			VulkanInstance();

			const std::vector<const char*>& GetInstanceLayers() const { return InstanceLayers; }
			const std::vector<const char*>& GetInstanceExtensions() const { return InstanceExtensions; }
			VkInstance& GetInstance() { return m_Instance; }
			const VkInstance& GetInstance() const { return m_Instance; }
		private:
			std::vector<const char*> InstanceLayers;
			std::vector<const char*> InstanceExtensions;
			VkInstance m_Instance;
		};

	}
}

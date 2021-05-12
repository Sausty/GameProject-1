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

			VkInstance&				 GetInstance()			 { return m_Instance; }
			std::vector<const char*> GetInstanceLayers()	 { return InstanceLayers; }
			std::vector<const char*> GetInstanceExtensions() { return InstanceExtensions; }
		private:
			std::vector<const char*> InstanceLayers;
			std::vector<const char*> InstanceExtensions;
			VkInstance			     m_Instance;
		};

	}
}

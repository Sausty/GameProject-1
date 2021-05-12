//
//  Created by Sausty on 12. May. 2021.
//

#include "Engine/Renderer/Apis/Vulkan/Core/DeletionQueue.h"

namespace gp1 {
	namespace vkcore {

		std::deque<std::function<void()>> DeletionQueue::m_Deletors;

		void DeletionQueue::PushFunction(std::function<void()>&& function)
		{
			m_Deletors.push_back(function);
		}

		void DeletionQueue::Flush()
		{
			for (auto it = m_Deletors.rbegin(); it != m_Deletors.rend(); it++) {
				(*it)();
			}

			m_Deletors.clear();
		}
	}
}

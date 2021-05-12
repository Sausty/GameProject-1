//
//  Created by Sausty on 12. May. 2021.
//

#pragma once

#include <deque>
#include <functional>

namespace gp1 {
	namespace vkcore {

		class DeletionQueue
		{
		public:
			static void PushFunction(std::function<void()>&& function);
			static void Flush();
		private:
			static std::deque<std::function<void()>> m_Deletors;
		};

	}
}

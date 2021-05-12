//
//  Created by Sausty on 12. May. 2021.
//

#pragma once

#include <volk.h>
#include <stdlib.h>
#include "Engine/Utility/Logger.h"
#include "Engine/Renderer/Apis/Vulkan/VulkanRenderer.h"

namespace gp1 {
	namespace vkcore {

		static void VkCheckError(VkResult result) {
			if (result != VK_SUCCESS) {
				VulkanRenderer::VulkanOutputLogger.LogError("Vulkan Error found!");
				exit(EXIT_FAILURE);
			}
		}

	}
}

//
//  Created by Sausty on 12. May. 2021.
//

#pragma once

#include "Engine/Window/Window.h"
#include <volk.h>

namespace gp1 {
	namespace vkcore {

		enum class SurfacePlatform
		{
			None,
			Win32,
			Wayland,
			XLIB
		};

		class VulkanSurface
		{
		public:
			VulkanSurface(Window* window);

			SurfacePlatform GetSurfacePlatform() { return m_Platform; }
			VkSurfaceKHR& GetVulkanSurface() { return m_Surface; }
			const VkSurfaceKHR& GetVulkanSurface() const { return m_Surface; }
		private:
			SurfacePlatform m_Platform;
			VkSurfaceKHR m_Surface;
		};
	}
}

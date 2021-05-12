//
//  Created by Sausty on 12. May. 2021.
//

#include "Engine/Renderer/Apis/Vulkan/Core/VulkanSurface.h"
#include "Engine/Renderer/Apis/Vulkan/Core/DeletionQueue.h"
#include "Engine/Renderer/Apis/Vulkan/Core/VulkanUtils.h"
#include "Engine/Renderer/Apis/Vulkan/VulkanRenderer.h"

#if defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#elif defined(__linux__)
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_WAYLAND
#include <vulkan/vulkan_xcb.h>
#include <vulkan/vulkan_wayland.h>
#endif
#include <GLFW/glfw3native.h>

namespace gp1 {
	namespace vkcore {

		VulkanSurface::VulkanSurface(Window* window)
			: m_Platform(SurfacePlatform::None)
		{
			VkInstance& instance = VulkanRenderer::GetVulkanRendererData().Instance->GetInstance();
			std::vector<const char*> extensions = VulkanRenderer::GetVulkanRendererData().Instance->GetInstanceExtensions();

			GLFWwindow* handle = window->GetNativeHandle();

			for (const char* extension : extensions)
			{
				if (extension == "VK_KHR_win32_surface")
					m_Platform = SurfacePlatform::Win32;
				if (extension == "VK_KHR_wayland_surface")
					m_Platform = SurfacePlatform::Wayland;
				if (extension == "VK_KHR_xlib_surface")
					m_Platform = SurfacePlatform::XLIB;
			}

			#ifdef _WIN32
			VkWin32SurfaceCreateInfoKHR surfaceCreateInfo{};
			surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			surfaceCreateInfo.hinstance = GetModuleHandle(NULL);
			surfaceCreateInfo.hwnd = glfwGetWin32Window(handle);

			PFN_vkCreateWin32SurfaceKHR ptr = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(instance, "vkCreateWin32SurfaceKHR");

			VkCheckError(ptr(instance, &surfaceCreateInfo, NULL, &m_Surface));
			#elif defined(__linux__)

			if (m_Platform == SurfacePlatform::Wayland)
			{
				wl_display* display = glfwGetWaylandDisplay();
				wl_window* window = glfwGetWaylandWindow(handle);

				VkWaylandSurfaceCreateInfoKHR createInfo{};
				createInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
				createInfo.pNext = NULL;
				createInfo.flags = 0;
				createInfo.display = display;
				createInfo.surface = window;

				PFN_vkCreateWaylandSurfaceKHR ptr = (PFN_vkCreateWaylandSurfaceKHR)vkGetInstanceProcAddr(instance, "vkCreateWaylandSurfaceKHR");

				VkCheckError(ptr(instance, &createInfo, NULL, &m_Surface));
			}

			if (m_Platform == SurfacePlatform::XLIB)
			{
				Display* display = glfwGetX11Display();
				Window xlib_window = glfwGetX11Window(handle);

				VkXlibSurfaceCreateInfoKHR createInfo;
				createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
				createInfo.pNext = NULL;
				createInfo.flags = 0;
				createInfo.dpy = display;
				createInfo.window = xlib_window;

				PFN_vkCreateXlibSurfaceKHR ptr = (PFN_vkCreateXlibSurfaceKHR)vkGetInstanceProcAddr(instance, "vkCreateXlibSurfaceKHR");

				VkCheckError(ptr(instance, &createInfo, NULL, &m_Surface));
			}
			#endif

			VulkanSurface inst = *this;
			DeletionQueue::PushFunction([inst, &instance]() mutable {
				vkDestroySurfaceKHR(instance, inst.GetVulkanSurface(), NULL);
			});
		}

	}
}

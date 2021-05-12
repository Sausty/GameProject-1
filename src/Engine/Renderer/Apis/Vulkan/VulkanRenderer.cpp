//	
//	Created by MarcasRealAccount on 31. Oct. 2020
//  Modified by Sausty on 12. May. 2021
//

#include "Engine/Renderer/Apis/Vulkan/VulkanRenderer.h"
#include "Engine/Renderer/Apis/Vulkan/VulkanDebugRenderer.h"
#include "Engine/Renderer/Apis/Vulkan/Mesh/VulkanMeshData.h"
#include "Engine/Renderer/Apis/Vulkan/Mesh/VulkanSkeletalMeshData.h"
#include "Engine/Renderer/Apis/Vulkan/Mesh/VulkanStaticMeshData.h"
#include "Engine/Renderer/Apis/Vulkan/Mesh/VulkanStaticVoxelMeshData.h"
#include "Engine/Renderer/Apis/Vulkan/Shader/VulkanMaterialData.h"
#include "Engine/Renderer/Apis/Vulkan/Shader/VulkanShaderData.h"

#include "Engine/Renderer/Apis/Vulkan/Core/DeletionQueue.h"

namespace gp1 {

	static VulkanRendererData s_RendererData;

	Logger VulkanRenderer::VulkanOutputLogger = Logger("Vulkan Output Logger");

	VulkanRenderer::VulkanRenderer(Window* window)
		: Renderer(window) {}

	RendererType VulkanRenderer::GetRendererType() const {
		return RendererType::VULKAN;
	}

	DebugRenderer* VulkanRenderer::CreateDebugRenderer() {
		return new VulkanDebugRenderer(this);
	}

	MeshData* VulkanRenderer::CreateSkeletalMeshData(Mesh* mesh) {
		return new VulkanSkeletalMeshData(mesh);
	}

	MeshData* VulkanRenderer::CreateStaticMeshData(Mesh* mesh) {
		return new VulkanStaticMeshData(mesh);
	}

	MeshData* VulkanRenderer::CreateStaticVoxelMeshData(Mesh* mesh) {
		return new VulkanStaticVoxelMeshData(mesh);
	}

	ShaderData* VulkanRenderer::CreateShaderData(Shader* shader) {
		return new VulkanShaderData(shader);
	}

	MaterialData* VulkanRenderer::CreateMaterialData(Material* material) {
		return new VulkanMaterialData(material);
	}

	VulkanRendererData VulkanRenderer::GetVulkanRendererData()
	{
		return s_RendererData;
	}

	void VulkanRenderer::InitRenderer() {
		s_RendererData.Instance = std::make_shared<vkcore::VulkanInstance>();
		s_RendererData.Surface = std::make_shared<vkcore::VulkanSurface>(m_Window);
		s_RendererData.GPU = std::make_shared<vkcore::VulkanGPU>();
		s_RendererData.Device = std::make_shared<vkcore::VulkanDevice>();
		s_RendererData.SwapChain = std::make_shared<vkcore::VulkanSwapChain>(2, m_Window->GetWindowData().Width, m_Window->GetWindowData().Height);

		VulkanOutputLogger.LogDebug("Created swap chain with 2 buffers");
	}

	void VulkanRenderer::DeInitRenderer() {
		vkcore::DeletionQueue::Flush();
	}

	void VulkanRenderer::RenderScene(Scene* scene, uint32_t width, uint32_t height) {
		_CRT_UNUSED(scene);
		_CRT_UNUSED(width);
		_CRT_UNUSED(height);
	}

} // namespace gp1

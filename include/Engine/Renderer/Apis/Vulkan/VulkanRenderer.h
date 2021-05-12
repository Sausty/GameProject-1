//	
//	Created by MarcasRealAccount on 31. Oct. 2020
//	

#pragma once
#include "Engine/Renderer/Renderer.h"
#include "Engine/Utility/Logger.h"

#include "Engine/Renderer/Apis/Vulkan/Core/VulkanInstance.h"
#include "Engine/Renderer/Apis/Vulkan/Core/VulkanSurface.h"
#include <memory>

namespace gp1 {

	struct VulkanRendererData
	{
		vkcore::VulkanInstance Instance;
		vkcore::VulkanSurface* Surface;
	};

	class VulkanRenderer : public Renderer {
	public:
		VulkanRenderer(Window* window);

		virtual RendererType GetRendererType() const override;

		virtual DebugRenderer* CreateDebugRenderer() override;

		virtual MeshData* CreateSkeletalMeshData(Mesh* mesh) override;
		virtual MeshData* CreateStaticMeshData(Mesh* mesh) override;
		virtual MeshData* CreateStaticVoxelMeshData(Mesh* mesh) override;

		virtual ShaderData* CreateShaderData(Shader* shader) override;

		virtual MaterialData* CreateMaterialData(Material* material) override;

		static Logger VulkanOutputLogger;
		static VulkanRendererData* GetVulkanRendererData();
	protected:
		virtual void InitRenderer() override;
		virtual void DeInitRenderer() override;
		virtual void RenderScene(Scene* scene, uint32_t width, uint32_t height) override;
	};
}

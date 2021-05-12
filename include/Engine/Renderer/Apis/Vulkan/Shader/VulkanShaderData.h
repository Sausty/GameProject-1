//	
//	Created by MarcasRealAccount on 31. Oct. 2020
//	

#pragma once
#include "Engine/Renderer/Shader/Shader.h"
#include "Engine/Utility/Logger.h"

#include <volk.h>

namespace gp1 {

	class VulkanRenderer;

	class VulkanShaderData : public ShaderData {
	public:
		VulkanShaderData(Shader* shader);

		virtual RendererType GetRendererType() const override;
		virtual void CleanUp() override;

		VkShaderModule& GetShaderModule(ShaderType type) { return m_ShaderModules[type]; }

		friend VulkanRenderer;
	private:
		VkShaderModule LoadShader(ShaderType type);
		std::unordered_map<ShaderType, bool> m_Shaders;
		std::unordered_map<ShaderType, VkShaderModule> m_ShaderModules;
		static Logger m_Logger;	// The shader's logger.
	};

} // namespace gp1

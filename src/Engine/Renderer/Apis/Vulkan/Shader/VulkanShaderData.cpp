//	
//	Created by MarcasRealAccount on 31. Oct. 2020
//	

#include "Engine/Renderer/Apis/Vulkan/Shader/VulkanShaderData.h"
#include "Engine/Renderer/Apis/Vulkan/VulkanRenderer.h"
#include "Engine/Renderer/Apis/Vulkan/Core/VulkanUtils.h"

#include <spirv/1.2/spirv.h>
#include <Include/ShHandle.h>
#include <MachineIndependent/iomapper.h>
#include <MachineIndependent/reflection.h>
#include <StandAlone/ResourceLimits.h>
#include <spirv/1.2/spirv.h>
#include <SPIRV/GlslangToSpv.h>

namespace gp1 {

	Logger VulkanShaderData::m_Logger("Vulkan Shader");

	static const char* GetShaderTypeExtensionName(ShaderType type) {
		switch (type) {
		case ShaderType::COMPUTE:
			return ".comp";
		case ShaderType::VERTEX:
			return ".vert";
		case ShaderType::TESS_CONTROL:
			return ".tessc";
		case ShaderType::TESS_EVAL:
			return ".tessv";
		case ShaderType::GEOMETRY:
			return ".geom";
		case ShaderType::FRAGMENT:
			return ".frag";
		default:
			return "Unknown Shader";
		}
	}

	static EShLanguage GetGLSLangType(ShaderType type)
	{
		switch (type) {
		case ShaderType::COMPUTE:
			return EShLanguage::EShLangCompute;
		case ShaderType::VERTEX:
			return EShLanguage::EShLangVertex;
		case ShaderType::TESS_CONTROL:
			return EShLanguage::EShLangTessControl;
		case ShaderType::TESS_EVAL:
			return EShLanguage::EShLangTessEvaluation;
		case ShaderType::GEOMETRY:
			return EShLanguage::EShLangGeometry;
		case ShaderType::FRAGMENT:
			return EShLanguage::EShLangFragment;
		}
	}

	VulkanShaderData::VulkanShaderData(Shader* shader)
		: ShaderData(shader)
	{
		ShaderType types[]{ ShaderType::COMPUTE, ShaderType::VERTEX, ShaderType::TESS_CONTROL, ShaderType::TESS_EVAL, ShaderType::GEOMETRY, ShaderType::FRAGMENT };
		for (ShaderType type : types) {
			FILE* file = fopen(("Shaders/" + GetShader<Shader>()->GetId() + GetShaderTypeExtensionName(type)).c_str(), "r");
			if (file) {
				this->m_Shaders.insert({ type, false });
				fclose(file);
			}
		}

		for (auto& shader : this->m_Shaders) {
			VkShaderModule shaderId = LoadShader(shader.first);
			if (shaderId > 0) {
				shader.second = true;
				m_ShaderModules.insert({ shader.first, shaderId });
			}
		}
	}

	RendererType VulkanShaderData::GetRendererType() const {
		return RendererType::VULKAN;
	}

	void VulkanShaderData::CleanUp() {
		VkDevice& device = VulkanRenderer::GetVulkanRendererData().Device->GetVulkanDevice();

		for (auto pair : m_ShaderModules) {
			if (pair.second != VK_NULL_HANDLE) {
				vkDestroyShaderModule(device, pair.second, nullptr);
			}
		}
	}

	VkShaderModule VulkanShaderData::LoadShader(ShaderType type)
	{	
		VkDevice& device = VulkanRenderer::GetVulkanRendererData().Device->GetVulkanDevice();

		FILE* file = fopen(("Shaders/" + GetShader<Shader>()->GetId() + GetShaderTypeExtensionName(type)).c_str(), "r");
		if (!file)
			return 0;

		fseek(file, 0, SEEK_END);
		uint32_t len = ftell(file);
		fseek(file, 0, SEEK_SET);
		char* buf = new char[len];
		len = static_cast<uint32_t>(fread(buf, sizeof(char), len, file));
		buf[len] = '\0';
		fclose(file);

		glslang::TShader shader(GetGLSLangType(type));

		shader.setEnvInput(glslang::EShSourceGlsl, shader.getStage(), glslang::EShClientVulkan, 430);
		shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_2);
		shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_2);

		EShMessages messages = EShMessages::EShMsgVulkanRules;
		shader.setStringsWithLengths(&buf, (const int*)&len, 1);
		shader.setEntryPoint("main");
		bool success = shader.parse(&glslang::DefaultTBuiltInResource, 430, true, messages);

		if (!success) {
			m_Logger.LogError("Failed to compile shader!");
			exit(EXIT_FAILURE);
		}

		glslang::TProgram program;
		program.addShader(&shader);
		program.link(messages);

		glslang::SpvOptions options;
#ifdef _DEBUG
		options.disableOptimizer = true;
#else
		options.disableOptimizer = false;
#endif

		std::vector<uint32_t> spirv_binary;
		glslang::GlslangToSpv(*program.getIntermediate(shader.getStage()),
			spirv_binary, NULL, &options);

		VkShaderModuleCreateInfo vertCreateInfo{};
		vertCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		vertCreateInfo.codeSize = spirv_binary.size() * sizeof(uint32_t);
		vertCreateInfo.pCode = spirv_binary.data();

		VkShaderModule module;
		vkcore::VkCheckError(vkCreateShaderModule(device, &vertCreateInfo, nullptr, &module));

		return module;
	}

} // namespace gp1

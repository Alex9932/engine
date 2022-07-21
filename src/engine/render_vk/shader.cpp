/*
 * shader.cpp
 *
 *  Created on: Jul 9, 2022
 *      Author: alex9932
 */

#include <engine/render_vk/shader.h>
#include <engine/render_vk/vk_utils.h>

#ifdef R_VULKAN_RENDERER

#include <engine/core/filesystem.h>

namespace Engine {
	namespace Render {

		static VkShaderModule readShader(String path) {
			Filesystem::Resource* data = Engine::Filesystem::GetResource(path);
			VkShaderModuleCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = data->length;
			createInfo.pCode = reinterpret_cast<const uint32_t*>(data->data);
			VkShaderModule shaderModule;
			if (vkCreateShaderModule(GetVkDevice(), &createInfo, GetVkAllocator(), &shaderModule) != VK_SUCCESS) {
				printf("failed to create shader module! %s\n", path);
			}

			Filesystem::FreeResource(data);
			return shaderModule;
		}

		VKShader::VKShader(const ShaderInfo& info) : Shader(info) {
			RG_ASSERT(info.vertex_path);
			RG_ASSERT(info.fragment_path);

			this->vs = readShader(info.vertex_path);
			this->fs = readShader(info.fragment_path);

			this->vsStageInfo = {};
			this->vsStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			this->vsStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			this->vsStageInfo.module = this->vs;
			this->vsStageInfo.pName = info.vsName;
			this->fsStageInfo = {};
			this->fsStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			this->fsStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			this->fsStageInfo.module = this->fs;
			this->fsStageInfo.pName = info.fsName;

		}

		VKShader::~VKShader() {
			vkDestroyShaderModule(GetVkDevice(), this->vs, GetVkAllocator());
			vkDestroyShaderModule(GetVkDevice(), this->fs, GetVkAllocator());
		}

		void VKShader::Start() {
		}

	}
}

#endif

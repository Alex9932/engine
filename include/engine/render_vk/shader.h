/*
 * shader.h
 *
 *  Created on: Jul 9, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_VK_SHADER_H_
#define ENGINE_RENDER_VK_SHADER_H_

#include <engine/engine.h>

#include <engine/render/r_shader.h>
#include <vulkan/vulkan.h>

namespace Engine {
	namespace Render {

		class RG_DECLSPEC VKShader : public Shader {

			protected:
				VkShaderModule vs;
				VkShaderModule fs;
				VkPipelineShaderStageCreateInfo vsStageInfo;
				VkPipelineShaderStageCreateInfo fsStageInfo;

			public:
				VKShader(const ShaderInfo& info);
				~VKShader();

				void Start();

				RG_INLINE VkShaderModule GetVertexModule() { return this->vs; }
				RG_INLINE VkShaderModule GetFragmentModule() { return this->fs; }
				RG_INLINE VkPipelineShaderStageCreateInfo& GetVertexInfo() { return this->fsStageInfo; }
				RG_INLINE VkPipelineShaderStageCreateInfo& GetFragmentInfo() { return this->fsStageInfo; }

		};

	}
}


#endif /* ENGINE_RENDER_GL_SHADER_H_ */

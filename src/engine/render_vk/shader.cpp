/*
 * shader.cpp
 *
 *  Created on: Jul 9, 2022
 *      Author: alex9932
 */

#include <engine/render_vk/shader.h>

#include <engine/core/filesystem.h>

namespace Engine {
	namespace Render {

		VKShader::VKShader(const ShaderInfo& info) : Shader(info) {
			RG_ASSERT(info.vertex_path);
			RG_ASSERT(info.fragment_path);

		}

		VKShader::~VKShader() {
		}

		void VKShader::Start() {
		}

	}
}

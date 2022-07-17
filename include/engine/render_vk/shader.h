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

namespace Engine {
	namespace Render {

		class RG_DECLSPEC VKShader : public Shader {

			protected:

			public:
				VKShader(const ShaderInfo& info);
				~VKShader();

				void Start();

		};

	}
}


#endif /* ENGINE_RENDER_GL_SHADER_H_ */

/*
 * shader.h
 *
 *  Created on: Jul 9, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_GL_SHADER_H_
#define ENGINE_RENDER_GL_SHADER_H_

#include <engine/engine.h>
#include <GL/glew.h>

#include <engine/render/r_shader.h>

namespace Engine {
	namespace Render {

		class RG_DECLSPEC GLShader : public Shader {

			private:
				GLuint shader;

			public:
				GLShader(const ShaderInfo& info);
				~GLShader();

				void Start() { glUseProgram(this->shader); }

		};

	}
}


#endif /* ENGINE_RENDER_GL_SHADER_H_ */

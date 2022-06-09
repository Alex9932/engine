/*
 * shader.h
 *
 *  Created on: Feb 13, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_SHADER_H_
#define ENGINE_RENDER_SHADER_H_

#include <engine/engine.h>
#include <engine/core/math/vector.h>
#include <engine/core/math/matrix.h>
#include <GL/glew.h>

namespace Engine {
	namespace Render {

		class RG_DECLSPEC Shader {
			private:
				GLuint shader;

			public:
				Shader(String vs, String fs, String gs);
				~Shader();
				void Start();
				void Stop();

				GLuint GetProgram();

				GLuint GetUniformLocation(String name);
				GLuint GetUniformBlockIndex(String name);
				void SetFloat(GLuint location, float value);
				void SetInt  (GLuint location, int value);
				void SetVec2 (GLuint location, const Engine::Math::vec2& value);
				void SetVec3 (GLuint location, const Engine::Math::vec3& value);
				void SetVec4 (GLuint location, const Engine::Math::vec4& value);
				void SetIVec2(GLuint location, const Engine::Math::ivec2& value);
				void SetIVec3(GLuint location, const Engine::Math::ivec3& value);
				void SetIVec4(GLuint location, const Engine::Math::ivec4& value);
				void SetMat4 (GLuint location, Engine::Math::mat4* value);

				void SetUniformBlock(GLuint location, GLuint buffer_index);
		};

	}
}



#endif /* ENGINE_RENDER_SHADER_H_ */

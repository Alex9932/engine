/*
 * shader.cpp
 *
 *  Created on: Jul 9, 2022
 *      Author: alex9932
 */

#include <engine/render_gl/shader.h>

#include <engine/core/filesystem.h>

namespace Engine {
	namespace Render {

		static void __make_program(String path, String name, GLuint program, GLuint shader) {
			Engine::Filesystem::Resource* data = Engine::Filesystem::GetResource(path);
			const GLchar* str = (GLchar*)data->data;
			GLint len = data->length;
			glShaderSource(program, 1, &str, &len);
			glCompileShader(program);

			int  success;
			char error[512];
			glGetShaderiv(program, GL_COMPILE_STATUS, &success);

			if (!success) {
				glGetShaderInfoLog(program, 512, NULL, error);
				rgLogInfo(RG_LOG_RENDER, "Shader error(s) [%d] in %s\n\n%s\n", success, name, error);
				RG_ASSERT_MSG(success, "SHADER COMPILE ERROR (see logs for more information)");
			}

			glAttachShader(shader, program);
			Engine::Filesystem::FreeResource(data);
		}

		GLShader::GLShader(const ShaderInfo& info) : Shader(info) {
			RG_ASSERT(info.vertex_path);
			RG_ASSERT(info.fragment_path);

			GLuint s_vs;
			GLuint s_fs;

			this->shader = glCreateProgram();
			rgLogInfo(RG_LOG_RENDER, "Loading shaders: %s %s", info.vertex_path, info.fragment_path);
			s_vs = glCreateShader(GL_VERTEX_SHADER);
			s_fs = glCreateShader(GL_FRAGMENT_SHADER);

			__make_program(info.vertex_path, "vertex", s_vs, shader);
			__make_program(info.fragment_path, "fragment", s_fs, shader);

			glLinkProgram(this->shader);

			int  success;
			char infoLog[512];
			glGetProgramiv(this->shader, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(this->shader, 512, NULL, infoLog);
				rgLogInfo(RG_LOG_RENDER, "Shader linking failed(%d)!\n\n%s\n", success, infoLog);
				RG_ASSERT_MSG(success, "SHADER LINK ERROR (see logs for more information)");
			}

			glValidateProgram(this->shader);
			glGetProgramiv(this->shader, GL_VALIDATE_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(this->shader, 512, NULL, infoLog);
				rgLogInfo(RG_LOG_RENDER, "Shader validation failed(%d)!\n\n%s\n", success, infoLog);
				RG_ASSERT_MSG(success, "SHADER VALIDATE ERROR (see logs for more information)");
			}

			glDeleteShader(s_vs);
			glDeleteShader(s_fs);

		}

		GLShader::~GLShader() {
			glDeleteProgram(this->shader);
		}

		void GLShader::Start() {
			glUseProgram(this->shader);
		}

	}
}

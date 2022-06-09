/*
 * shader.cpp
 *
 *  Created on: Feb 13, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/render/shader.h>
#include <engine/core/filesystem.h>

namespace Engine {
	namespace Render {

		static void __make_program(String path, String name, GLuint program, GLuint shader) {
			Engine::Filesystem::Resource* data = Engine::Filesystem::GetResource(path);
			const GLchar* str = (GLchar*)data->data;
			GLint len = data->length;
//			char* source = (char*)malloc(data->length + 1);
//			memset(source, 0, data->length + 1);
//			memcpy(source, str, data->length);
//			rgLogInfo(RG_LOG_RENDER, "Shader source: \n%s\n", source);
//			glShaderSource(program, 1, &source, &len);
//
//			rgLogInfo(RG_LOG_RENDER, "Shader source: \n%s\n", str);
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

//			free(source);
			Engine::Filesystem::FreeResource(data);
		}

		Shader::Shader(String vs, String fs, String gs) {
			RG_ASSERT(vs);
			RG_ASSERT(fs);

			GLuint s_vs;
			GLuint s_fs;
			GLuint s_gs;
			this->shader = glCreateProgram();
			rgLogInfo(RG_LOG_RENDER, "Loading shaders: %s %s", vs, fs);
			s_vs = glCreateShader(GL_VERTEX_SHADER);
			s_fs = glCreateShader(GL_FRAGMENT_SHADER);
			if (gs != NULL) {
				s_gs = glCreateShader(GL_GEOMETRY_SHADER);
				rgLogInfo(RG_LOG_RENDER, "|___GEOMETRY: %s", gs);
			} else {
				s_gs = 0xFFFFFFFF;
			}

			// load shaders
			__make_program(vs, "vertex", s_vs, shader);
			__make_program(fs, "fragment", s_fs, shader);
			if (s_gs != 0xFFFFFFFF) {
				rgLogInfo(RG_LOG_RENDER, "SHADER USES GEOMETRY PROGRAM!");
				__make_program(gs, "geometry", s_gs, shader);
			}

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
			if (s_gs != 0xFFFFFFFF) {
				glDeleteShader(s_gs);
			}
		}

		Shader::~Shader() {
			glDeleteProgram(this->shader);
		}

		void Shader::Start() {
			glUseProgram(this->shader);
		}

		void Shader::Stop() {
			glUseProgram(0);
		}

		GLuint Shader::GetProgram() {
			return this->shader;
		}

		GLuint Shader::GetUniformLocation(String name) {
			return glGetUniformLocation(this->shader, name);
		}

		GLuint Shader::GetUniformBlockIndex(String name) {
			return glGetUniformBlockIndex(this->shader, name);
		}

		void Shader::SetFloat(GLuint location, float value) {
			glUniform1f(location, value);
		}

		void Shader::SetInt(GLuint location, int value) {
			glUniform1i(location, value);
		}

		void Shader::SetVec2(GLuint location, const Engine::Math::vec2& value) {
			glUniform2f(location, value.x, value.y);
		}

		void Shader::SetVec3(GLuint location, const Engine::Math::vec3& value) {
			glUniform3f(location, value.x, value.y, value.z);
		}

		void Shader::SetVec4(GLuint location, const Engine::Math::vec4& value) {
			glUniform4f(location, value.x, value.y, value.x, value.w);
		}

		void Shader::SetIVec2(GLuint location, const Engine::Math::ivec2& value) {
			glUniform2i(location, value.x, value.y);
		}

		void Shader::SetIVec3(GLuint location, const Engine::Math::ivec3& value) {
			glUniform3i(location, value.x, value.y, value.z);
		}

		void Shader::SetIVec4(GLuint location, const Engine::Math::ivec4& value) {
			glUniform4i(location, value.x, value.y, value.x, value.w);
		}

		void Shader::SetMat4(GLuint location, Engine::Math::mat4* value) {
			glUniformMatrix4fv(location, 1, GL_FALSE, (float*)value);
		}

		void Shader::SetUniformBlock(GLuint location, GLuint buffer_index) {
			glUniformBlockBinding(this->shader, location, buffer_index);
		}

	}
}

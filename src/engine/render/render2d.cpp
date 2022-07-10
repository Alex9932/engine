/*
 * render2d.cpp
 *
 *  Created on: Feb 22, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/render/render2d.h>
#include <engine/render/shader.h>
#include <engine/render/window.h>

#define RG_R2D_MAX_VERTECES 1024

namespace Engine {
	namespace Render {

		static SShader* r2d_shader;
		static GLuint vao;
		static GLuint vbo;
		static Uint32 vertex = 0;
		static Engine::Math::vec2 size = {0, 0};
		static Engine::Math::mat4 matrix;
		static Engine::Math::mat4 proj;
		static GLuint binded_texture = 0;
		static GLuint DEFAULT_TEXTURE;
		static Engine::Math::vec4 color = {1, 1, 1, 1};
		static R2D_VERTEX* local_buffer;

		void InitializeR2D() {
			local_buffer = (R2D_VERTEX*)malloc(sizeof(R2D_VERTEX) * RG_R2D_MAX_VERTECES);
			r2d_shader = new SShader("platform/shaders/r2d.vs", "platform/shaders/r2d.fs", NULL);
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(R2D_VERTEX) * RG_R2D_MAX_VERTECES, NULL, GL_STREAM_DRAW);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(R2D_VERTEX), (GLvoid*)(sizeof(float) * 0)); // vertex
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(R2D_VERTEX), (GLvoid*)(sizeof(float) * 2)); // uv
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(R2D_VERTEX), (GLvoid*)(sizeof(float) * 4)); // color
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

			glGenTextures(1, &DEFAULT_TEXTURE);
			glBindTexture(GL_TEXTURE_2D, DEFAULT_TEXTURE);
			int data = 0xFFFFFFFF;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			Engine::Math::mat4_identity(&matrix);
		}

		void DestroyR2D() {
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &vbo);
			delete r2d_shader;
			free(local_buffer);
		}

		void R2D_BindTexture(GLuint tex) {
			if(tex == 0) {
				binded_texture = DEFAULT_TEXTURE;
			} else {
				binded_texture = tex;
			}
		}

		void R2D_SetColor(const Engine::Math::vec4& c) {
			color = c;
		}

		void R2D_SetMatrix(const Engine::Math::mat4& mat) {
			matrix = mat;
		}

		void R2D_PushVertex(const R2D_VERTEX& _vertex) {
			local_buffer[vertex] = _vertex;
			vertex++;
		}

		static void _R2D_Draw(GLenum mode) {
			Engine::Window::GetWindowSize(&size);

			Engine::Math::mat4_ortho(&proj, size.x, 0, size.y, 0, 1, -1);

			r2d_shader->Start();
			r2d_shader->SetInt(r2d_shader->GetUniformLocation("t_unit0"), 0);
			r2d_shader->SetVec2(r2d_shader->GetUniformLocation("size"), size);
			r2d_shader->SetMat4(r2d_shader->GetUniformLocation("proj"), &proj);
			r2d_shader->SetMat4(r2d_shader->GetUniformLocation("matrix"), &matrix);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, binded_texture);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(R2D_VERTEX) * RG_R2D_MAX_VERTECES, local_buffer);
			glBindVertexArray(vao);
			glDrawArrays(mode, 0, vertex);
			r2d_shader->Stop();
			vertex = 0;
			color.x = 1;
			color.y = 1;
			color.z = 1;
			color.w = 1;
		}

		void R2D_DrawMesh() {
			_R2D_Draw(GL_TRIANGLES);
		}

		void R2D_DrawLines() {
			_R2D_Draw(GL_LINES);
		}

		void R2D_DrawQuad(const Engine::Math::vec2& pos, const Engine::Math::vec2& size) {
			float x = pos.x;
			float y = pos.y;
			float w = size.x;
			float h = size.y;
			R2D_PushVertex({    x,     y, 0, 1, color.x, color.y, color.z, color.w});
			R2D_PushVertex({x + w,     y, 1, 1, color.x, color.y, color.z, color.w});
			R2D_PushVertex({x + w, y + h, 1, 0, color.x, color.y, color.z, color.w});
			R2D_PushVertex({x + w, y + h, 1, 0, color.x, color.y, color.z, color.w});
			R2D_PushVertex({    x, y + h, 0, 0, color.x, color.y, color.z, color.w});
			R2D_PushVertex({    x,     y, 0, 1, color.x, color.y, color.z, color.w});
			R2D_DrawMesh();
		}

		void R2D_DrawCircle(const Engine::Math::vec2& pos, float radius, Uint32 n) {
			float t = 2.0f * 3.1415926f / float(n);
			float ox = radius * cosf(t);
			float oy = radius * sinf(t);
			float ou = cosf(t);
			float ov = sinf(t);

			for (Uint32 i = 1; i < n + 2; ++i) {
				float theta = 2.0f * 3.1415926f * float(i) / float(n);
				float x = radius * cosf(theta);
				float y = radius * sinf(theta);
				float u = (1 + cosf(theta)) * 0.5;
				float v = 1 - (1 + sinf(theta)) * 0.5;
				R2D_PushVertex({     pos.x,      pos.y, 0.5f, 0.5f, color.x, color.y, color.z, color.w});
				R2D_PushVertex({ox + pos.x, oy + pos.y,   ou,   ov, color.x, color.y, color.z, color.w});
				R2D_PushVertex({ x + pos.x,  y + pos.y,    u,    v, color.x, color.y, color.z, color.w});
				ox = x;
				oy = y;
				ou = u;
				ov = v;
			}

			R2D_DrawMesh();
		}
	}
}


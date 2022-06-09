/*
 * fontrenderer.cpp
 *
 *  Created on: Feb 21, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/render/fontrenderer.h>
#include <engine/core/math/matrix.h>
#include <engine/core/utils.h>
#include <engine/core/utf8.h>
#include <engine/render/window.h>
#include <engine/render/shader.h>
#include <GL/glew.h>

#define RG_FR_MAX_VERTECES 1024

namespace Engine {
	namespace Render {

		struct FR_VERTEX {
			Engine::Math::vec2 pos;
			Engine::Math::vec2 coord;
			Engine::Math::vec4 color;
		};

		static Shader* font_shader;
		static GLuint vao;
		static GLuint vbo;
		static Uint32 vertex = 0;
		static Engine::Math::vec2 size = {0, 0};
		static Engine::Math::mat4 matrix;

		static FR_VERTEX* local_buffer;
		static UTF8Decoder utf_decoder;

		void InitFontRenderer() {
			local_buffer = (FR_VERTEX*)malloc(sizeof(FR_VERTEX) * RG_FR_MAX_VERTECES);
			font_shader = new Shader("platform/shaders/font.vs", "platform/shaders/font.fs", NULL);
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(FR_VERTEX) * RG_FR_MAX_VERTECES, NULL, GL_STREAM_DRAW);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(FR_VERTEX), (GLvoid*)(sizeof(float) * 0)); // vertex
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(FR_VERTEX), (GLvoid*)(sizeof(float) * 2)); // uv
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(FR_VERTEX), (GLvoid*)(sizeof(float) * 4)); // color
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

			Engine::Math::mat4_identity(&matrix);
		}

		void DestroyFontRenderer() {
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &vbo);
			delete font_shader;
			free(local_buffer);
		}

		static void PushVertex(const FR_VERTEX& _vertex) {
			local_buffer[vertex] = _vertex;
			vertex++;
		}

		static void PushChar(Font* font, Uint16 c, const Engine::Math::vec2& pos, const Engine::Math::vec4& color, float scale, float* x) {
			Glyph g = font->GetGlyphs()[c];
			float xpos = *x + g.bearing_x * scale;
			float ypos = pos.y - (g.size_y - g.bearing_y) * scale;
			float w = g.size_x * scale;
			float h = g.size_y * scale;
			float u = (float)(c % F_ATLAS_WIDTH) / (float)F_ATLAS_WIDTH;
			float v = (float)(c / F_ATLAS_WIDTH) / (float)F_ATLAS_HEIGHT;
			float su = (float)g.size_x / (float)(F_ATLAS_WIDTH  * font->GetScale());
			float sv = (float)g.size_y / (float)(F_ATLAS_HEIGHT * font->GetScale());
			PushVertex({xpos    , ypos + h, u     , v     , color.x, color.y, color.z, color.w});
			PushVertex({xpos    , ypos    , u     , v + sv, color.x, color.y, color.z, color.w});
			PushVertex({xpos + w, ypos    , u + su, v + sv, color.x, color.y, color.z, color.w});
			PushVertex({xpos    , ypos + h, u     , v     , color.x, color.y, color.z, color.w});
			PushVertex({xpos + w, ypos    , u + su, v + sv, color.x, color.y, color.z, color.w});
			PushVertex({xpos + w, ypos + h, u + su, v     , color.x, color.y, color.z, color.w});
			*x += (g.advance_x >> 6) * scale;
		}

		void RenderRawString(Font* font, Uint16* str, const Engine::Math::vec2& pos, const Engine::Math::vec4& color, float scale) {
			vertex = 0;

			float x = pos.x;
			Uint16 c = 0;

			for(Uint32 i = 0; ; i++) {
				c = str[i];
				if(c == 0 || c == 10) { break; }
				PushChar(font, c, pos, color, scale, &x);
			}

			Engine::Window::GetWindowSize(&size);
			font_shader->Start();
			font_shader->SetIVec2(font_shader->GetUniformLocation("size"), {(int)size.x, (int)size.y});
			font_shader->SetInt(font_shader->GetUniformLocation("t_unit0"), 0);
			float sw = 1/size.x*2;
			float sh = 1/size.y*2;
			Engine::Math::mat4_identity(&matrix, {sw, sh, 1});
			matrix.m03 = -1;
			matrix.m13 = -1;

			Engine::Math::mat4_ortho(&matrix, size.x, 0, size.y, 0, -1, 1);

			font_shader->SetMat4(font_shader->GetUniformLocation("matrix"), &matrix);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, font->GetAtlas());
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(FR_VERTEX) * RG_FR_MAX_VERTECES, local_buffer);
			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, vertex);
			font_shader->Stop();
		}

		void RenderString(Font* font, String str, const Engine::Math::vec2& pos, const Engine::Math::vec4& color, float scale) {
			utf_decoder.DecodeString(str);
			RenderRawString(font, utf_decoder.GetResult(), pos, color, scale);
		}

	}
}


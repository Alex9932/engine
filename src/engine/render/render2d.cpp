/*
 * render2d.cpp
 *
 *  Created on: Feb 22, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/core/allocator.h>

#include <engine/render/render2d.h>
#include <engine/render/shader.h>
#include <engine/render/window.h>

#include <engine/render/r_backend.h>

// TODO: !!! Rewrite this !!!
#include <engine/render_gl/shader.h>

#define RG_R2D_MAX_VERTECES 1024

namespace Engine {
	namespace Render {

		static STDAllocator* allocator;

//		static SShader* r2d_shader;
//		static GLuint vao;
//		static GLuint vbo;
		static Uint32 vertex = 0;
		static Engine::Math::vec2 size = {0, 0};
		static Engine::Math::mat4 matrix;
		static Engine::Math::mat4 proj;

		static Engine::Math::vec4 color = {1, 1, 1, 1};

		static Pipeline* pipeline;
		static Shader* shader;

		static Texture* DEFAULT_TEXTURE;
		static Texture* binded_texture;

		// Local vertices storage
		static R2D_VERTEX* local_buffer;
		static Uint16*     local_ibuffer;

		static Vertexbuffer* vertexbuffer;

		void InitializeR2D() {

			allocator = new STDAllocator("RG_R2D");
			RegisterAllocator(allocator);

			local_buffer = (R2D_VERTEX*)allocator->Allocate(sizeof(R2D_VERTEX) * RG_R2D_MAX_VERTECES);
			local_ibuffer = (Uint16*)allocator->Allocate(sizeof(Uint16) * RG_R2D_MAX_VERTECES);

			VertexAttribDescriptor descriptors[3];
			descriptors[0].elementsPerLocation = 2;
			descriptors[0].stride = sizeof(R2D_VERTEX);
			descriptors[0].offset = offsetof(R2D_VERTEX, pos);
			descriptors[0].type   = ATTRIB_FLOAT;
			descriptors[1].elementsPerLocation = 2;
			descriptors[1].stride = sizeof(R2D_VERTEX);
			descriptors[1].offset = offsetof(R2D_VERTEX, coord);
			descriptors[1].type   = ATTRIB_FLOAT;
			descriptors[2].elementsPerLocation = 4;
			descriptors[2].stride = sizeof(R2D_VERTEX);
			descriptors[2].offset = offsetof(R2D_VERTEX, color);
			descriptors[2].type   = ATTRIB_FLOAT;

			VertexbufferInfo info;
			info.vertex_size = sizeof(R2D_VERTEX);
			info.type     = INDEX_UINT16;
			info.v_count  = RG_R2D_MAX_VERTECES;
			info.i_count  = RG_R2D_MAX_VERTECES;
			info.vertices = local_buffer;
			info.indices  = local_ibuffer;
			info.attrib_count = 3;
			info.attrib_descriptors = descriptors;
			vertexbuffer = MakeVertexbuffer(info, allocator);

			// TODO Use Pipeline

			ShaderInfo s_info;

#if defined(R_OPENGL_RENDERER)
			s_info.vertex_path = "platform/shaders_gl/r2d.vs";
			s_info.fragment_path = "platform/shaders_gl/r2d.fs";
#elif defined(R_VULKAN_RENDERER)
			s_info.vertex_path = "platform/shaders_vk/r2d.vspv";
			s_info.fragment_path = "platform/shaders_vk/r2d.fspv";
#endif
			shader = MakeShader(s_info, allocator);

			PipelineInfo pl_info;
			pl_info.shader = shader;
			pl_info.framebuffer = GetDefaultFramebuffer();
			pl_info.polygonMode = RG_PM_FILL;
			pl_info.cullFace = RG_CF_NONE;
			pipeline = MakePipeline(pl_info, allocator);


//			r2d_shader = new SShader("platform/shaders/r2d.vs", "platform/shaders/r2d.fs", NULL);


			int data = 0xFFFFFFFF;
			TextureInfo tex_info;
			tex_info.width  = 1;
			tex_info.height = 1;
			tex_info.type   = DATATYPE_UINT8;
			tex_info.format = FORMAT_RGBA;
			tex_info.filter = FILTER_NEAREST;
			tex_info.data   = &data;
			DEFAULT_TEXTURE = MakeTexture(tex_info, allocator);
			binded_texture  = DEFAULT_TEXTURE;

			matrix = Math::IDENTITY_MAT4;
		}

		void DestroyR2D() {
			DeleteVertexbuffer(vertexbuffer, allocator);
			allocator->Deallocate(local_buffer);
			allocator->Deallocate(local_ibuffer);
			DeleteShader(shader, allocator);
			DeletePipeline(pipeline, allocator);
//			delete r2d_shader;
		}

		void R2D_BindTexture(Texture* tex) {
			if(tex == NULL) {
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

		void R2D_PushVertex(const R2D_VERTEX& vtx) {
			local_buffer[vertex] = vtx;
			local_ibuffer[vertex] = vertex;
			vertex++;
		}

		static void _R2D_Draw(GLenum mode) {
			Window::GetWindowSize(&size);
			Math::mat4_ortho(&proj, size.x, 0, size.y, 0, 1, -1);

			Math::mat4 s_mat;
			Math::mat4_mul(&s_mat, proj, matrix);
			matrix = Math::IDENTITY_MAT4;

			pipeline->Bind();
			pipeline->AttachTexture(0, binded_texture);

//			r2d_shader->Start();

#if defined(R_OPENGL_RENDERER)

			#define GLSHADER_UNIFORMI(a, b)  glUniform1f(glGetUniformLocation(((GLShader*)shader)->GetGLProgram(), a), b)
			#define GLSHADER_UNIFORM4M(a, b) glUniformMatrix4fv(glGetUniformLocation(((GLShader*)shader)->GetGLProgram(), a), 1, GL_FALSE, (float*)b)
			GLSHADER_UNIFORMI("t_unit0", 0);
			GLSHADER_UNIFORM4M("matrix", &s_mat);
#elif defined(R_VULKAN_RENDERER)
			// TODO Vulkan renderer
#endif

			vertexbuffer->WriteVertexbuffer(local_buffer, 0, sizeof(R2D_VERTEX) * vertex);
			vertexbuffer->WriteIndexbuffer(local_ibuffer, 0, sizeof(Uint16) * vertex);
			vertexbuffer->Draw(0, vertex);

//			r2d_shader->Stop();
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


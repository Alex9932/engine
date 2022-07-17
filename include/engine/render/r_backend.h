/*
 * r_backend.h
 *
 *  Created on: Jul 7, 2022
 *      Author: alex9932
 */

#ifndef ENGINE_RENDER_R_BACKEND_H_
#define ENGINE_RENDER_R_BACKEND_H_

#include <engine/engine.h>
#include <engine/core/allocator.h>
#include <engine/render/r_vertexbuffer.h>
#include <engine/render/r_framebuffer.h>
#include <engine/render/r_pipeline.h>
#include <engine/render/r_shader.h>
#include <engine/render/r_texture.h>

#define R_OPENGL_RENDERER
//#define R_VULKAN_RENDERER // TODO

namespace Engine {
	namespace Render {

		RG_DECLSPEC void SetupBackend();
		RG_DECLSPEC void InitializeBackend();
		RG_DECLSPEC void DestroyBackend();
		RG_DECLSPEC void Swapbuffers();
		RG_DECLSPEC void ViewportBackend(float x, float y, float w, float h);
		RG_DECLSPEC void MakeImguiBackend();

		RG_DECLSPEC String GetRendererName();

		// Objects
		RG_DECLSPEC Vertexbuffer* MakeVertexbuffer(const VertexbufferInfo& info, Allocator* allocator);
		RG_DECLSPEC void DeleteVertexbuffer(Vertexbuffer* vertexbuffer, Allocator* allocator);
		RG_DECLSPEC Framebuffer* MakeFramebuffer(const FramebufferInfo& info, Allocator* allocator);
		RG_DECLSPEC void DeleteFramebuffer(Framebuffer* framebuffer, Allocator* allocator);
		RG_DECLSPEC Pipeline* MakePipeline(const PipelineInfo& info, Allocator* allocator);
		RG_DECLSPEC void DeletePipeline(Pipeline* pipeline, Allocator* allocator);
		RG_DECLSPEC Shader* MakeShader(const ShaderInfo& info, Allocator* allocator);
		RG_DECLSPEC void DeleteShader(Shader* shader, Allocator* allocator);
		RG_DECLSPEC Texture* MakeTexture(const TextureInfo& info, Allocator* allocator);
		RG_DECLSPEC void DeleteTexture(Texture* texture, Allocator* allocator);

	}
}


#endif /* ENGINE_RENDER_R_BACKEND_H_ */

/*
 * renderer.cpp
 *
 *  Created on: Jul 7, 2022
 *      Author: alex9932
 */

#include <engine/render/r_backend.h>

#ifdef R_OPENGL_RENDERER

#include <engine/render/window.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include <engine/render_gl/vertexbuffer.h>
#include <engine/render_gl/framebuffer.h>
#include <engine/render_gl/shader.h>
#include <engine/render_gl/pipeline.h>
#include <engine/render_gl/texture.h>

namespace Engine {
	namespace Render {

		static char description[256];

		static SDL_GLContext w_glcontext = NULL;
		static Uint32 swap_int = 0;

		static Framebuffer* framebuffer;

		static void MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
		    rgLogError(RG_LOG_RENDER, "GL: %d => %s", type, message);
		}

		void SetupBackend() {
			// Use OpenGL 3.3
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
			SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
			SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
			SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
		}

		void InitializeBackend() {
			w_glcontext = SDL_GL_CreateContext(Window::GetWindow());
			if (!w_glcontext) {
				rgLogInfo(RG_LOG_RENDER, "OpenGL context error!");
				RG_ERROR_MSG("OpenGL context error!");
			}

			glewExperimental = GL_TRUE;
			GLenum glewError = glewInit();
			if (glewError != GLEW_OK) {
				rgLogInfo(RG_LOG_RENDER, "Error initializing GLEW! %s", glewGetErrorString(glewError));
				RG_ERROR_MSG("GLEW Initialization failed!");
			}

			if (!glewIsSupported("GL_VERSION_3_3")) {
				RG_ERROR_MSG("OpenGL 3.3 is not supported!");
			}

			Math::vec2 size;
			Window::GetWindowSize(&size);
			glClearColor(0, 0, 0, 1);
			ViewportBackend(0, 0, size.x, size.y);

#ifdef RG_GRAPHICSAPI_DEBUG
			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback(MessageCallback, NULL);
#endif

			framebuffer = NULL;

//			sprintf(description, "%s %s", glGetString(GL_VERSION), glGetString(GL_RENDERER));
			sprintf(description, "%s", glGetString(GL_RENDERER));
			rgLogInfo(RG_LOG_RENDER, "OpenGL: %s", description);
			rgLogInfo(RG_LOG_RENDER, "Initialized OpenGL rendering backend");
		}

		void DestroyBackend() {
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplSDL2_Shutdown(); // @suppress("Invalid arguments")
			SDL_GL_DeleteContext(w_glcontext);
		}

		void Swapbuffers() {
			SDL_GL_SetSwapInterval(swap_int);
			SDL_GL_SwapWindow(Window::GetWindow());
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void ViewportBackend(float x, float y, float w, float h) {
			glViewport(x, y, w, h);
		}

		void MakeImguiBackend() {
			ImGui_ImplSDL2_InitForOpenGL(Window::GetWindow(), w_glcontext); // @suppress("Invalid arguments")
			ImGui_ImplOpenGL3_Init("#version 150");
		}

		String GetRendererName() {
			return "OpenGL";
		}

		String GetRendererDescription() {
			return description;
		}

		Framebuffer* GetDefaultFramebuffer() {
			return framebuffer;
		}

		// Objects
		Vertexbuffer* MakeVertexbuffer(const VertexbufferInfo& info, Allocator* allocator) {
			return new(allocator->Allocate(sizeof(GLVertexbuffer))) GLVertexbuffer(info);
		}

		void DeleteVertexbuffer(Vertexbuffer* vertexbuffer, Allocator* allocator) {
			vertexbuffer->~Vertexbuffer();
			allocator->Deallocate(vertexbuffer);
		}

		Framebuffer* MakeFramebuffer(const FramebufferInfo& info, Allocator* allocator) {
			return new(allocator->Allocate(sizeof(GLFramebuffer))) GLFramebuffer(info);
		}

		void DeleteFramebuffer(Framebuffer* framebuffer, Allocator* allocator) {
			framebuffer->~Framebuffer();
			allocator->Deallocate(framebuffer);
		}

		Pipeline* MakePipeline(const PipelineInfo& info, Allocator* allocator) {
			return new(allocator->Allocate(sizeof(GLPipeline))) GLPipeline(info);
		}

		void DeletePipeline(Pipeline* pipeline, Allocator* allocator) {
			pipeline->~Pipeline();
			allocator->Deallocate(pipeline);
		}

		Shader* MakeShader(const ShaderInfo& info, Allocator* allocator) {
			return new(allocator->Allocate(sizeof(GLShader))) GLShader(info);
		}

		void DeleteShader(Shader* shader, Allocator* allocator) {
			shader->~Shader();
			allocator->Deallocate(shader);
		}

		Texture* MakeTexture(const TextureInfo& info, Allocator* allocator) {
			return new(allocator->Allocate(sizeof(GLTexture))) GLTexture(info);
		}

		void DeleteTexture(Texture* texture, Allocator* allocator) {
			texture->~Texture();
			allocator->Deallocate(texture);
		}

	}
}

#endif

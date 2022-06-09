/*
 * window.cpp
 *
 *  Created on: Feb 11, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/render/window.h>
#include <engine/core/logger.h>
#include <engine/core/timer.h>
#include <GL/glew.h>

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

namespace Engine {
	namespace Window {

		static Timer timer;
		static SDL_Window* w_startup      = NULL;
		static SDL_Window* w_window_ptr   = NULL;
		static SDL_GLContext w_glcontext  = NULL;
		static Uint32 w_init_width        = 1280;
		static Uint32 w_init_height       = 720;
		static Uint32 w_current_width     = 0;
		static Uint32 w_current_height    = 0;
		static Uint32 swap_int            = 0;

		static Sint32 limit               = 0; // Set 0 to disable frame-rate limiter

		static bool w_fullscreen = false;

		void Initialize() {
			// Use OpenGL 3.3
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
			SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
			SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
			SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

			if(!Engine::IsDebug()) {
				w_startup = SDL_CreateWindow("rgEngine - init", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 300, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
				SDL_Renderer *ren = SDL_CreateRenderer(w_startup, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
				SDL_Surface *bmp = SDL_LoadBMP("platform/logo.bmp");
				SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
				SDL_FreeSurface(bmp);
				SDL_RenderClear(ren);
				SDL_RenderCopy(ren, tex, NULL, NULL);
				SDL_RenderPresent(ren);
			}

			timer.Update();
		}

		void Destroy() {

			ImGui_ImplOpenGL3_Shutdown(); // @suppress("Invalid arguments")
			ImGui_ImplSDL2_Shutdown();    // @suppress("Invalid arguments")
			ImGui::DestroyContext();

			SDL_GL_DeleteContext(w_glcontext);
			SDL_DestroyWindow(w_window_ptr);
		}

		void Show() {
			SDL_Delay(1000);
			w_current_width = w_init_width;
			w_current_height = w_init_height;

			w_window_ptr = SDL_CreateWindow("rgEngine", 5, 5, 650, 500, SDL_WINDOW_OPENGL);
			SDL_assert(w_window_ptr);
			w_glcontext = SDL_GL_CreateContext(w_window_ptr);
			if (!w_glcontext) {
				rgLogInfo(RG_LOG_RENDER, "OpenGL context error!");
				RG_ERROR_MSG("OpenGL context error!");
			}
			SDL_GL_SwapWindow(w_window_ptr);

			SDL_Delay(500);
			if(!Engine::IsDebug()) {
				SDL_DestroyWindow(w_startup);
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

//			String gl_v = (String)glGetString(GL_VERSION);

			SDL_SetWindowSize(w_window_ptr, w_current_width, w_current_height);
			SDL_SetWindowPosition(w_window_ptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
			glViewport(0, 0, w_current_width, w_current_height);
			SDL_GL_SetSwapInterval(swap_int);
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			SDL_GL_SwapWindow(w_window_ptr);

			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			ImGui::StyleColorsLight();

			ImGui_ImplSDL2_InitForOpenGL(w_window_ptr, w_glcontext); // @suppress("Invalid arguments")
			ImGui_ImplOpenGL3_Init("#version 150");                  // @suppress("Invalid arguments")
		}

		void Update() {
			SDL_GL_SetSwapInterval(swap_int);
//			SDL_bool b = _m_grabbed ? SDL_TRUE : SDL_FALSE;
//			SDL_SetWindowGrab(w_window_ptr, b);
//			SDL_CaptureMouse(b);
//			SDL_SetRelativeMouseMode(b);
			SDL_GL_SwapWindow(w_window_ptr);
			glClear(GL_COLOR_BUFFER_BIT);

			if(limit > 0) {
				double etime = timer.GetElapsedTime();
				double frame_time = 1.0 / (double)limit;
				if(etime < frame_time) {
					double delta = frame_time - etime;
					SDL_Delay(delta * 1000);
				}
			}

			timer.Update();
		}

		void ToggleFullscreen() {
			w_fullscreen = !w_fullscreen;
			if (w_fullscreen) {
				SDL_DisplayMode DM;
				SDL_GetDesktopDisplayMode(0, &DM);
				w_current_width = DM.w;
				w_current_height = DM.h;
			} else {
				w_current_width = w_init_width;
				w_current_height = w_init_height;
			}

			SDL_SetWindowSize(w_window_ptr, w_current_width, w_current_height);
			SDL_SetWindowFullscreen(w_window_ptr, w_fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
			glViewport(0, 0, w_current_width, w_current_height);
		}

		bool IsFullscreen() {
			return w_fullscreen;
		}

		SDL_Window* GetWindow() {
			return w_window_ptr;
		}

		void GetWindowSize(Engine::Math::vec2* size) {
			size->x = w_current_width;
			size->y = w_current_height;
		}

		void SetFpsLimit(Sint32 fps) {
			limit = fps;
		}

	}
}

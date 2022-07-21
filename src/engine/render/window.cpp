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

#include <engine/render/r_backend.h>

#include <imgui.h>

namespace Engine {
	namespace Window {

		static Timer timer;
		static SDL_Window* w_startup      = NULL;
		static SDL_Window* w_window_ptr   = NULL;
		static Uint32 w_init_width        = 1280;
		static Uint32 w_init_height       = 720;
		static Uint32 w_current_width     = 0;
		static Uint32 w_current_height    = 0;

		static Sint32 limit               = 0; // Set 0 to disable frame-rate limiter

		static bool w_fullscreen = false;

		void Initialize() {

			Render::SetupBackend();

			if(!Engine::IsDebug()) {
				w_startup = SDL_CreateWindow("rgEngine - init", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 300, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
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
			Render::DestroyBackend();
			ImGui::DestroyContext();
			SDL_DestroyWindow(w_window_ptr);
		}

		void Show() {
			SDL_Delay(1000);
			w_current_width = w_init_width;
			w_current_height = w_init_height;

#if defined(R_OPENGL_RENDERER)
			w_window_ptr = SDL_CreateWindow("rgEngine", 5, 5, 650, 500, SDL_WINDOW_OPENGL);
#elif defined(R_VULKAN_RENDERER)
			w_window_ptr = SDL_CreateWindow("rgEngine", 5, 5, w_current_width, w_current_height, SDL_WINDOW_VULKAN);
#endif
			SDL_assert(w_window_ptr);

			Render::InitializeBackend();

			Render::Swapbuffers();

			SDL_Delay(500);
			if(!Engine::IsDebug()) {
				SDL_DestroyWindow(w_startup);
			}

//			String gl_v = (String)glGetString(GL_VERSION);
#if defined(R_OPENGL_RENDERER)
			SDL_SetWindowSize(w_window_ptr, w_current_width, w_current_height);
#endif
			SDL_SetWindowPosition(w_window_ptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
			Render::ViewportBackend(0, 0, w_current_width, w_current_height);
			Render::Swapbuffers();

			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			ImGui::StyleColorsLight();
			Render::MakeImguiBackend();
		}

		void Update() {
			Render::Swapbuffers();

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
			Render::ViewportBackend(0, 0, w_current_width, w_current_height);
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

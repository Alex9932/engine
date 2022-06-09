/*
 * render.cpp
 *
 *  Created on: Feb 13, 2022
 *      Author: alex9932
 */

#define DLL_EXPORT

#include <engine/render/render.h>
#include <engine/render/fontrenderer.h>
#include <engine/render/overlay.h>
#include <engine/render/console.h>
#include <engine/render/render2d.h>
#include <GL/glew.h>

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#define RG_RENDER_MAX_MODELS 1024

namespace Engine {
	namespace Render {

		// Allocators
		static Engine::STDAllocator* geom_allocator;

		// Other
		static Math::vec4 clear_color = {0, 0, 0, 1};

		static Model* models[RG_RENDER_MAX_MODELS];
		static RiggedModel* rmodels[RG_RENDER_MAX_MODELS];
		static Uint32 model_id = 0;
		static Uint32 rmodel_id = 0;

		static bool profiler = false;
		static bool console = false;

		void Initialize() {
			geom_allocator = new Engine::STDAllocator("geometry_allocator");
			Engine::RegisterAllocator(geom_allocator);

			InitializeR2D();
			InitFontRenderer();
			InitOverlay();
			InitConsole();
		}

		void Destroy() {
			DestroyConsole();
			DestroyOverlay();
			DestroyFontRenderer();
			DestroyR2D();
		}

		void Clear() {
			glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void Render() {
			if(profiler)
				RenderOverlay();

			if(console)
				RenderConsole();
		}

		void ShowProfiler() {
			profiler = true;
		}

		void HideProfiler() {
			profiler = false;
		}

		void ShowConsole() {
			console = true;
			SetConsoleShown(console);
		}

		void HideConsole() {
			console = false;
			SetConsoleShown(console);
		}

		void ImGuiBegin() {
			ImGui_ImplOpenGL3_NewFrame(); // @suppress("Invalid arguments")
			ImGui_ImplSDL2_NewFrame();    // @suppress("Invalid arguments")
			ImGui::NewFrame();
		}

		void ImGuiEnd() {
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // @suppress("Invalid arguments")
		}

		Allocator* GetGeomAllocator() {
			return geom_allocator;
		}

		void ClearColor(const Math::vec4& color) {
			clear_color = color;
		}

		void EnableDepthTest() {
			glEnable(GL_DEPTH_TEST);
		}

		void DisableDepthTest() {
			glDisable(GL_DEPTH_TEST);
		}

		void EnableBlend(GLenum sfactor, GLenum dfactor) {
			glEnable(GL_BLEND);
			glBlendFunc(sfactor, dfactor);
		}

		void DisableBlend() {
			glDisable(GL_BLEND);
		}

		Uint32 RegisterModel(Model* model) {
			model_id++;
			models[model_id] = model;
			return model_id;
		}

		Model* GetRegisteredModel(Uint32 id) {
			return models[id];
		}

		Uint32 RegisterRiggedModel(RiggedModel* model) {
			rmodel_id++;
			rmodels[rmodel_id] = model;
			return rmodel_id;
		}

		RiggedModel* GetRegisteredRiggedModel(Uint32 id) {
			return rmodels[id];
		}

	}
}

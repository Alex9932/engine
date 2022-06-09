/*
 * overlay.cpp
 *
 *  Created on: Feb 21, 2022
 *      Author: alex9932
 */

#define _CRT_SECURE_NO_WARNINGS
#define DLL_EXPORT

#include <engine/render/overlay.h>
#include <engine/render/fontrenderer.h>
#include <engine/render/shader.h>
#include <engine/render/render2d.h>
#include <engine/core/loader.h>
#include <engine/core/math/math.h>
#include <cstdio>

#define FPS_ARRAY 160

namespace Engine {
	namespace Render {
		static Font* font;
		static double fps[FPS_ARRAY];

		static double GetAvgFps() {
			double avg = 0;
			for (Uint32 i = 0; i < FPS_ARRAY; ++i) {
				avg += fps[i];
			}
			return avg / FPS_ARRAY;
		}

		static void UpdateAvgFps() {
			for (Uint32 i = 0; i < FPS_ARRAY - 1; ++i) {
				fps[i] = fps[i + 1];
			}

			fps[FPS_ARRAY - 1] = 1.0 / Engine::GetDeltaTime();
		}

		void InitOverlay() {
			font = new Font("platform/msyh.ttf", 64);
			SDL_memset(fps, 0, sizeof(double) * FPS_ARRAY);
		}

		void RenderOverlay() {
			UpdateAvgFps();

//			Created 21.02.22
//			Commented 03.04.22
//			Engine::Math::mat4 proj;
//			Engine::Math::mat4 model;
//			Engine::Math::mat4 matrix;
//			Engine::Math::mat4_frustum(&proj, 1.77, Engine::Math::toradians(70), 0.01, 1000);
//			Engine::Math::mat4_model(&model, {0.5f, 0, -3}, {(float)Engine::GetUptime() + 2, (float)Engine::GetUptime() + 1, (float)Engine::GetUptime() + 3.14f});
//			Engine::Math::mat4_mul(&matrix, &proj, &model);
//			R2D_SetMatrix(matrix);
//			R2D_BindTexture(0);
//			R2D_PushVertex({-0.5, -0.5, 0, 0, 1, 0, 0, 1});
//			R2D_PushVertex({ 0.5, -0.5, 0, 0, 0, 1, 0, 1});
//			R2D_PushVertex({ 0.0,  0.5, 0, 0, 0, 0, 1, 1});
//			R2D_DrawMesh();
//			Engine::Math::mat4_model(&model, {-0.5, 0, -1}, {(float)Engine::GetUptime() + 1, (float)Engine::GetUptime() + 2, (float)Engine::GetUptime()});
//			Engine::Math::mat4_mul(&matrix, &proj, &model);
//			R2D_SetMatrix(matrix);
//			R2D_BindTexture(0);
//			R2D_PushVertex({-0.5, -0.5, 0, 0, 0, 1, 1, 1});
//			R2D_PushVertex({ 0.5, -0.5, 0, 0, 1, 0, 1, 1});
//			R2D_PushVertex({ 0.0,  0.5, 0, 0, 1, 1, 0, 1});
//			R2D_DrawMesh();

			// Set matrix
			Engine::Math::mat4 identity;
			Engine::Math::mat4_identity(&identity);
			R2D_SetMatrix(identity);

			R2D_BindTexture(0);
			R2D_SetColor({0.1f, 0.1f, 0.1f, 0.8f});
			R2D_DrawQuad({0, 0}, {175, 60});

			double MULT = 100;

			R2D_BindTexture(0);
			double oft = 1.0 / fps[0];
			float ox = 5 + (1);
			float oy = 40 + (oft * MULT);
			if(oft * MULT > 10) { oft = 10.0 / MULT; }

			for (Uint32 i = 1; i < FPS_ARRAY - 1; ++i) {
				double ft = 1.0 / fps[i];
				if(ft * MULT > 10) { ft = 10.0 / MULT; }
				float x = 5 + i + (1);
				float y = 40 + (ft * MULT);
				R2D_PushVertex({ox    , oy, 0, 0, 1, 1, 1, 1});
				R2D_PushVertex({ x + 1,  y, 0, 0, 1, 1, 1, 1});
				oft = ft;
				ox = x;
				oy = y;
			}
			R2D_DrawLines();

			// Draw strings
			char l0[128];
			char l1[128];
			sprintf(l0, "Uptime: %lf s", Engine::GetUptime());
			double avg_fps = GetAvgFps();
			sprintf(l1, "Fps: %d, Frametime: %lf", (Uint32)avg_fps, Engine::GetDeltaTime());
			RenderString(font, l0, {5, 5}, {1, 1, 1, 1}, 0.18);

			Engine::Math::vec4 color = {1, 0, 0, 1};
			if(avg_fps > 30 && avg_fps < 60) { color.y = 1; }
			if(avg_fps > 60) {
				color.x = 0;
				color.y = 1;
			}

			RenderString(font, l1, {5, 17}, color, 0.18);
		}

		void DestroyOverlay() {
			delete font;
		}

		Font* GetDefaultFont() {
			return font;
		}
	}
}
